/* EMUnit embedded unit test engine - Copyright (C) 2017 Radoslaw Koppel
 *
 * This program is free software: you can redistribute it and/or modify
 * in under the terms of the GNU General Public license (version 3)
 * as published by the Free Software Foundation AND MODIFIED BY the
 * EMUnit exception.
 *
 * NOTE: The exception was added to the GPL to ensure
 * that you can test any kind of software without being
 * obligated to release the whole source code under the terms of GPL.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and the EMUnit license exception along with this program.
 * If not, it can be viewed in the original EMunit repository:
 * <https://github.com/rkel/emunit>.
 */
/**
 * @file
 * @brief EMUnit implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * @sa emunit_group
 */
#include "emunit.h"
#include "emunit_private.h"
#include "emunit_port.h"
#include "emunit_display.h"
#include <string.h>
#include <stdarg.h>

/**
 * @defgroup emunit_group_internal EMUnit internals
 * @internal
 * @ingroup emunit_group
 *
 * @{
 */

/**
 * @var emunit_status
 * @brief The status of the whole test
 *
 * The variable with the status of all the tests.
 * It has to be retained during system restart.
 *
 * This variable has to be declared in a port file this way
 * that it holds its value between test restarts.
 */
extern emunit_status_t emunit_status;

/* The externally defined array of the suites */
extern emunit_test_desc_t const __flash * const __flash emunit_main_ts[];


/**
 * @brief Function called on the program beginning when status have
 *
 * This function is called only if the program was restarted because of
 * test initialisation.
 * It would init all internal structures.
 */
static void emunit_status_clear(void)
{
	EMUNIT_STATIC_ASSERT((EMUNIT_IDX_INVALID + 1U) == 0U,
		"EMUNIT_IDX_INVALID have to be 1 before first suite for the engine to work correctly");
	memset(&emunit_status, 0, sizeof(emunit_status));
	emunit_status.ts_n_current = EMUNIT_IDX_INVALID;
	emunit_status.rr = EMUNIT_RR_INIT;
	emunit_status.key_valid = EMUNIT_STATUS_KEY_VALID;
}

/**
 * @brief Check if given test suite index is End of List
 *
 *
 * @param suite_idx Suite index
 *
 * @retval true  End of List detected
 * @retval false No End of List detected
 */
static inline bool emunit_ts_eol_check(size_t suite_idx)
{
	return NULL == emunit_main_ts[suite_idx];
}

/**
 * @brief Check if given test case descriptio pointer is End of List
 *
 * Use this function during list iteration to check if we are on the end.
 *
 * @param[in] p_tc test case descriptor pointer
 *
 * @retval true  End of List detected
 * @retval false No End of List detected
 */
static inline bool emunit_tc_eol_check(const __flash emunit_test_desc_t * p_tc)
{
	return (NULL == p_tc->p_fnc);
}


/**
 * @brief Get selected test case
 *
 * @param suite_idx Index of the suite to check
 * @param test_idx  Index of the test to check
 *
 * @return The pointer to the test case descriptor.
 */
const __flash emunit_test_desc_t * emunit_tc_get(size_t suite_idx, size_t test_idx)
{
	emunit_test_desc_t const __flash * p_ts = emunit_main_ts[suite_idx];
	return (p_ts + EMUNIT_TS_IDX_FIRST + test_idx);
}

/**
 * @brief Get current test case descriptor
 *
 * @return Descriptor pointer to the task case
 */
const __flash emunit_test_desc_t * emunit_tc_current_get(void)
{
	return emunit_tc_get(emunit_ts_current_index_get(), emunit_tc_current_index_get());
}

/**
 * @brief Prepare next test suite
 *
 * Test case is set to the first one.
 * Test suite is incremented.
 *
 * @note
 * Test indexes are changed only if current test case is not the end of test
 * suites mark.
 */
static void emunit_ts_next_switch(void)
{
	emunit_test_desc_t const __flash * p_ts = emunit_main_ts[emunit_ts_current_index_get()];
	if(NULL != p_ts)
	{
		emunit_status.ts_current_failed = 0;
		emunit_status.tc_n_current = 0;
		++(emunit_status.ts_n_current);
	}
}

/**
 * @brief Switch to the next test case
 *
 * Function changes current test case in the current test suite.
 * Test index is incremented only if current test index does not mark
 * End of List.
 *
 * @note
 * Suite is switched if current suite has been finished
 */
static void emunit_tc_next_switch(void)
{
	if(EMUNIT_IDX_INVALID == emunit_ts_current_index_get())
	{
		emunit_status.ts_n_current = 0;
		emunit_status.tc_n_current = 0;
	}
	else if(EMUNIT_IDX_INVALID == emunit_tc_current_index_get())
	{
		emunit_status.tc_n_current = EMUNIT_TS_IDX_FIRST;
	}
	else
	{
		emunit_test_desc_t const __flash * p_tc = emunit_tc_current_get();

		if(!emunit_tc_eol_check(p_tc))
		{
			++(emunit_status.tc_n_current);
		}
		else
		{
			emunit_ts_next_switch();
		}
	}
}

/**
 * @brief Run special function from the test suite
 *
 * @param sidx Suite index
 * @param fidx Function index may be one of the values:
 *             - @ref EMUNIT_TS_IDX_SUITEINIT
 *             - @ref EMUNIT_TS_IDX_SUITECLEANUP
 *             - @ref EMUNIT_TS_IDX_INIT
 *             - @ref EMUNIT_TS_IDX_CLEANUP
 */
static void emunit_ts_special_run(size_t sidx, size_t fidx)
{
	emunit_test_desc_t const __flash * p_ts = emunit_main_ts[sidx];
	emunit_test_fnc fnc = p_ts[fidx].p_fnc;
	if(NULL != fnc)
	{
		fnc();
	}
}

/**
 * @brief Run suite init function of the current suite
 *
 * Function that should be called once when suite starts.
 */
static void emunit_current_suiteinit_run(void)
{
	emunit_ts_special_run(emunit_status.ts_n_current, EMUNIT_TS_IDX_SUITEINIT);
}

/**
 * @brief Run suite cleanup function of the current suite
 *
 * Function that should be called once when suite is finished.
 */
static void emunit_current_suitecleanup_run(void)
{
	emunit_ts_special_run(emunit_status.ts_n_current, EMUNIT_TS_IDX_SUITECLEANUP);
}


/**
 * @brief Run test init function
 *
 * Runs the init function that should run before every test in the suite.
 */
static void emunit_current_init_run(void)
{
	EMUNIT_IASSERT_MSG(!emunit_status.tc_current_cleanup_required, "New initialisation without cleanup");
	emunit_ts_special_run(emunit_status.ts_n_current, EMUNIT_TS_IDX_INIT);
	emunit_status.tc_current_cleanup_required = true;
}

/**
 * @brief Run cleanup function
 *
 * Runs the cleanup function that should run after every test in the suite
 */
static void emunit_current_cleanup_run(void)
{
	EMUNIT_IASSERT_MSG(emunit_status.tc_current_cleanup_required, "Cleanup called when no test has been initialised");
	emunit_status.tc_current_cleanup_required = false;
	emunit_ts_special_run(emunit_status.ts_n_current, EMUNIT_TS_IDX_CLEANUP);
}

/**
 * @name Condition checks
 *
 * Functions that checks if asserts of named type should fail or not.
 * All returns true if contition is passed and false if it is not.
 * @{
 */
	/**
	 * @brief Check simple condition
	 *
	 * @param condition value
	 * @retval true  Assert passed
	 * @retval false Assert failed
	 */
	static bool ut_assert_check(const bool condition)
	{
		return condition;
	}

	/**
	 * @brief Check if given values are equal
	 *
	 * @param expected Expected value
	 * @param actual   Actual value
	 * @retval true  Values are equal
	 * @retval false Values are not equal
	 */
	static bool ut_assert_equal_check(
		const emunit_num_t expected,
		const emunit_num_t actual)
	{
		return (expected.u == actual.u);
	}


	static bool ut_assert_delta_check(
		const __flash emunit_assert_head_t * p_head,
		const emunit_unum_t delta,
		const emunit_num_t expected,
		const emunit_num_t actual)
	{
		emunit_num_t min, max;
		min.u = expected.u - delta;
		max.u = expected.u + delta;
		if(emunit_numtype_signed_check(p_head->numtype))
		{
			if(min.s > expected.s)
			{
				/* Overload */
				min.s = EMUNIT_SNUM_MIN;
			}
			if(max.s < expected.s)
			{
				/* Overload */
				max.s = EMUNIT_SNUM_MAX;
			}
			return (min.s <= actual.s) && (actual.s <= max.s);
		}
		else
		{
			if(min.u > expected.u)
			{
				/* Overload */
				min.u = 0;
			}
			if(max.u < expected.u)
			{
				/* Overload */
				max.u = EMUNIT_UNUM_MAX;
			}
			return (min.u <= actual.u) && (actual.u <= max.u);
		}
	}

	static bool ut_assert_range_check(
		const __flash emunit_assert_head_t * p_head,
		emunit_num_t min,
		emunit_num_t max,
		emunit_num_t actual)
	{
		if(emunit_numtype_signed_check(p_head->numtype))
		{
			if(min.s <= max.s)
				return (min.s <= actual.s) && (actual.s <= max.s);
			else
				return (min.s <= actual.s) || (actual.s <= max.s);
		}
		else
		{
			if(min.u <= max.u)
				return (min.u <= actual.u) && (actual.u <= max.u);
			else
				return (min.u <= actual.u) || (actual.u <= max.u);
		}
	}

	static bool ut_assert_nstr_check(
		size_t n,
		const __memx char * expected,
		const __memx char * actual,
		size_t * p_index)
	{
		bool result = true;
		size_t i = 0;
		char ce, ca;
		while( (0 != ((ce = *expected++) | (ca = *actual++))) && (0 != n--) )
		{
			if(ce != ca)
			{
				*p_index = i;
				result = false;
				break;
			}
			++i;
		}
		return result;
	}

/** @} */

/** @} */

/* ----------------------------------------------------------------------------
 * Private functions
 */
void emunit_early_init(void)
{
	emunit_port_early_init(&emunit_status.key_valid);
	if(EMUNIT_STATUS_KEY_VALID != emunit_status.key_valid)
	{
		emunit_display_clear();
		/* Status clear as the last one, because it marks key_valid */
		emunit_status_clear();
	}
}

void emunit_flush(void)
{
	emunit_display_present();
}

int emunit_run(void)
{
	switch(emunit_status.rr)
	{
		case EMUNIT_RR_INIT:
			emunit_display_test_start();
			emunit_restart(EMUNIT_RR_RUNNEXT);
			break;

		case EMUNIT_RR_FINISH:
			EMUNIT_IASSERT_MSG(
				(emunit_ts_failed_get() + emunit_ts_passed_get()) != 0,
				"Unexpected test suites final status");
			return (int)emunit_ts_failed_get();

		case EMUNIT_RR_PANIC:
			return -1;

		case EMUNIT_RR_TIMEOUT:
			/** @todo Show timeout message */
			// if()

			break;
		case EMUNIT_RR_RUNNEXT:
			emunit_tc_next_switch();
			/* Intentional fallthrough */
		case EMUNIT_RR_RUN:
			/* It test suite points to NULL the whole test has been finished */
			if(emunit_ts_eol_check(emunit_ts_current_index_get()))
			{
				emunit_display_test_end();
				emunit_restart(EMUNIT_RR_FINISH);
			}

			if(0 == emunit_status.tc_n_current)
			{
				/* If tc_n_current is 0, the test suite header should be generated */
				emunit_display_ts_start();
				/* First test index */
				emunit_status.tc_n_current = EMUNIT_TS_IDX_FIRST-1;
				emunit_current_suiteinit_run();
				emunit_restart(EMUNIT_RR_RUNNEXT);
			}
			const __flash emunit_test_desc_t * p_tc;
			p_tc = emunit_tc_current_get();
			if(emunit_tc_eol_check(p_tc))
			{
				/* If tc_n_current points to NULL function, suite should be changed */
				if(emunit_status.ts_current_failed)
				{
					++emunit_status.ts_n_failed;
				}
				else
				{
					++emunit_status.ts_n_passed;
				}
				emunit_current_suitecleanup_run();
				emunit_display_ts_end();
				emunit_restart(EMUNIT_RR_RUNNEXT);
			}
			else
			{
				/* Normal test run */
				emunit_display_tc_start();
				emunit_current_init_run();
				p_tc->p_fnc();
				++(emunit_status.tc_n_passed);
				emunit_current_cleanup_run();
				emunit_display_tc_end();
				emunit_restart(EMUNIT_RR_RUNNEXT);
			}

			break;
		default:
			/* Should never happen */
			EMUNIT_IASSERT_MSG(false, "Unexpected reset reason.");
			break;
	}

	return -1;
}

void emunit_restart(emunit_rr_t rr)
{
	emunit_status.rr = rr;
	emunit_port_restart();
}

const __flash char * emunit_test_name_get(void)
{
	/** @todo Temporary solution */
	static const __flash char name[] = "EMUnit";
	return name;
}

size_t emunit_ts_current_index_get(void)
{
	return emunit_status.ts_n_current;
}

const __flash char * emunit_ts_name_get(size_t suite_idx)
{
	emunit_test_desc_t const __flash * p_ts = emunit_main_ts[suite_idx];
	return p_ts[EMUNIT_TS_IDX_NAME].p_name;
}

size_t emunit_ts_total_count(void)
{
	size_t n = 0;
	emunit_test_desc_t const __flash * const __flash * pp_ts = emunit_main_ts;
	while(NULL != (*pp_ts++))
	{
		++n;
	}
	return n;
}

size_t emunit_ts_failed_get(void)
{
	return emunit_status.ts_n_failed;
}

size_t emunit_ts_passed_get(void)
{
	return emunit_status.ts_n_passed;
}

size_t emunit_tc_current_index_get(void)
{
	size_t tc_idx = emunit_status.tc_n_current;
	if(tc_idx < EMUNIT_TS_IDX_FIRST)
		return EMUNIT_IDX_INVALID;
	return tc_idx - EMUNIT_TS_IDX_FIRST;
}

const __flash char * emunit_tc_name_get(size_t suite_idx, size_t test_idx)
{
	emunit_test_desc_t const __flash * p_ts = emunit_main_ts[suite_idx];
	return p_ts[EMUNIT_TS_IDX_FIRST + test_idx].p_name;
}

size_t emunit_tc_count(size_t suite_idx)
{
	size_t n = 0;
	emunit_test_desc_t const __flash * p_tc = emunit_main_ts[suite_idx];
	p_tc += EMUNIT_TS_IDX_FIRST;
	while(NULL != (p_tc++)->p_fnc)
	{
		++n;
	}
	return n;
}

size_t emunit_tc_failed_get(void)
{
	return emunit_status.tc_n_failed;
}

size_t emunit_tc_passed_get(void)
{
	return emunit_status.tc_n_passed;
}

size_t emunit_tc_total_count(void)
{
	size_t n = 0;
	emunit_test_desc_t const __flash * const __flash * pp_ts = emunit_main_ts;
	emunit_test_desc_t const __flash * p_tc;
	while(NULL != (p_tc = *pp_ts++))
	{
		p_tc += EMUNIT_TS_IDX_FIRST;

		while(!emunit_tc_eol_check(p_tc++))
		{
			++n;
		}
	}
	return n;
}

/* ----------------------------------------------------------------------------
 * Public API
 */

void emunit_assert_passed(void)
{
	/* Nothing to do now */
}

void emunit_assert_failed(void)
{
	/* Mark current suite has failed test */
	emunit_status.ts_current_failed = true;
	/* Count failed test, switch to next and restart */
	++(emunit_status.tc_n_failed);
	if(emunit_status.tc_current_cleanup_required)
		emunit_current_cleanup_run();
	emunit_display_tc_end();
	emunit_restart(EMUNIT_RR_RUNNEXT);
}

void ut_assert(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * p_exp,
	bool condition)
{
	if(!ut_assert_check(condition))
	{
		emunit_display_failed_assert(p_head, p_exp);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_msg(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * p_exp,
	bool condition,
	const __flash char * fmt,
	...)
{
	if(!ut_assert_check(condition))
	{
		va_list va;
		va_start(va, fmt);
		emunit_display_failed_assert_msg(p_head, p_exp, fmt, va);
		va_end(va);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_equal(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t expected,
	emunit_num_t actual)
{
	if(!ut_assert_equal_check(expected, actual))
	{
		emunit_display_failed_equal(p_head, expected, actual);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_equal_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t expected,
	emunit_num_t actual,
	const __flash char * fmt,
	...)
{
	if(!ut_assert_equal_check(expected, actual))
	{
		va_list va;
		va_start(va, fmt);
		emunit_display_failed_equal_msg(p_head, expected, actual, fmt, va);
		va_end(va);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}


void ut_assert_delta(
	const __flash emunit_assert_head_t * p_head,
	emunit_unum_t delta,
	emunit_num_t expected,
	emunit_num_t actual)
{
	if(!ut_assert_delta_check(p_head, delta, expected, actual))
	{
		emunit_display_failed_delta(
			p_head,
			delta,
			expected,
			actual);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_delta_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_unum_t delta,
	emunit_num_t expected,
	emunit_num_t actual,
	const __flash char * fmt,
	...)
{
	if(!ut_assert_delta_check(p_head, delta, expected, actual))
	{
		va_list va;
		va_start(va, fmt);
		emunit_display_failed_delta_msg(
			p_head,
			delta,
			expected,
			actual,
			fmt,
			va);
		va_end(va);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_range(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t min,
	emunit_num_t max,
	emunit_num_t actual)
{
	if(!ut_assert_range_check(p_head, min, max, actual))
	{
		emunit_display_failed_range(p_head, min, max, actual);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_range_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t min,
	emunit_num_t max,
	emunit_num_t actual,
	const __flash char * fmt,
	...)
{
	if(!ut_assert_range_check(p_head, min, max, actual))
	{
		va_list va;
		va_start(va, fmt);
		emunit_display_failed_range_msg(p_head, min, max, actual, fmt, va);
		va_end(va);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_nstr(
	const __flash emunit_assert_head_t * p_head,
	size_t n,
	const __memx char * expected,
	const __memx char * actual)
{
	size_t err_pos;
	if(!ut_assert_nstr_check(n, expected, actual, &err_pos))
	{
		emunit_display_failed_nstr(p_head, n, expected, actual, err_pos);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}

void ut_assert_nstr_msg(
	const __flash emunit_assert_head_t * p_head,
	size_t n,
	const __memx char * expected,
	const __memx char * actual,
	const __flash char * fmt,
	...)
{
	size_t err_pos;
	if(!ut_assert_nstr_check(n, expected, actual, &err_pos))
	{
		va_list va;
		va_start(va, fmt);
		emunit_display_failed_nstr_msg(p_head, n, expected, actual, err_pos, fmt, va);
		va_end(va);
		emunit_assert_failed();
	}
	else
	{
		emunit_assert_passed();
	}
}
