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
 */
EMUNIT_NOINIT_VAR(emunit_status_t, emunit_status);

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
	memset(&emunit_status, 0, sizeof(emunit_status));
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
 * @brief Switch to the next test case
 *
 * Function changes current test case in the current test suite.
 * Test index is incremented only if current test index does not mark
 * End of List.
 */
static void emunit_tc_next_switch(void)
{
	emunit_test_desc_t const __flash * p_tc = emunit_tc_current_get();

	if(!emunit_tc_eol_check(p_tc))
	{
		++(emunit_status.tc_n_current);
	}
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
 * @brief Early test initialisation
 *
 * Function that has to be called at the very beginning of the program.
 * In some ports there may be time limit to run this function because of
 * WDT constants.
 *
 * Run it at least just before @ref emunit_run.
 */
static inline void emunit_early_init(void)
{
	emunit_port_early_init(&emunit_status.key_valid);
}

/**
 * @brief Run the EMUnit
 *
 * This function should be called when program is started.
 * It could be written directly in main, but separated function
 * created for code cleanes.
 */
static int emunit_run(void)
{
#if 0
	static char buffer[64];
	snprintf(buffer, sizeof(buffer), "Started, key: %x\n", emunit_status.key_valid);
	emunit_port_out_init();
	emunit_port_out_write(buffer, strlen(buffer));
	emunit_port_out_deinit();
#endif
	if(EMUNIT_STATUS_KEY_VALID != emunit_status.key_valid)
	{
		emunit_status_clear();
		emunit_display_clear();

	}
	else
	{
		emunit_display_present();
		emunit_display_clear();
	}

	switch(emunit_status.rr)
	{
		case EMUNIT_RR_INIT:
			emunit_display_test_start();
			emunit_restart(EMUNIT_RR_RUN);
			break;

		case EMUNIT_RR_FINISH:
			return 0;

		case EMUNIT_RR_PANIC:
			return -1;

		case EMUNIT_RR_TIMEOUT:
			/** @todo Show timeout message */
			// if()

			break;
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
				emunit_status.tc_n_current = EMUNIT_TS_IDX_FIRST;
				emunit_restart(EMUNIT_RR_RUN);
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
				emunit_display_ts_end();
				emunit_ts_next_switch();
				emunit_restart(EMUNIT_RR_RUN);
			}
			else
			{
				/* Normal test run */
				emunit_display_tc_start();
				p_tc->p_fnc();
				++(emunit_status.tc_n_passed);
				emunit_display_tc_end();
				emunit_tc_next_switch();
				emunit_restart(EMUNIT_RR_RUN);
			}

			break;
		default:
			/* Should never happen */
			EMUNIT_IASSERT_MGS(false, "Unexpected reset reason.");
			break;
	}

	return -1;
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
		if(emunit_numtype_signed_check(p_head->numtype))
		{
			emunit_snum_t min, max;
			min = expected.s - delta;
			if(min > expected.s)
			{
				/* Overload */
				min = EMUNIT_SNUM_MIN;
			}
			max = expected.u + delta;
			if(max < expected.u)
			{
				/* Overload */
				max = EMUNIT_SNUM_MAX;
			}
			return (min <= actual.s) && (actual.s <= max);
		}
		else
		{
			emunit_unum_t min, max;
			min = expected.u - delta;
			if(min > expected.u)
			{
				/* Overload */
				min = 0;
			}
			max = expected.u + delta;
			if(max < expected.u)
			{
				/* Overload */
				max = EMUNIT_UNUM_MAX;
			}
			return (min <= actual.u) && (actual.u <= max);
		}
	}

	static bool  ut_assert_range_check(
		const __flash emunit_assert_head_t * p_head,
		emunit_num_t min,
		emunit_num_t max,
		emunit_num_t actual)
	{
		if(emunit_numtype_signed_check(p_head->numtype))
		{
			return (min.s <= actual.s) && (actual.s <= max.s);
		}
		else
		{
			return (min.u <= actual.u) && (actual.u <= max.u);
		}
	}


/** @} */

/** @} */

/* ----------------------------------------------------------------------------
 * Private functions
 */
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
	return emunit_status.tc_n_current - EMUNIT_TS_IDX_FIRST;
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
	emunit_display_tc_end();
	emunit_tc_next_switch();
	emunit_restart(EMUNIT_RR_RUN);
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
			(emunit_num_t){.u = delta},
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
			(emunit_num_t){.u = delta},
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

void ut_assert_str(
	const __flash emunit_assert_head_t * p_head,
	const __memx char * expected,
	const __memx char * actual)
{
	/** @todo Implement */
	EMUNIT_IASSERT_MGS(0, "Not implemented yet.");
}

void ut_assert_str_msg(
	const __flash emunit_assert_head_t * p_head,
	const __memx char * expected,
	const __memx char * actual,
	const __flash char * fmt,
	...)
{
	/** @todo Implement */
	EMUNIT_IASSERT_MGS(0, "Not implemented yet.");
}

void ut_assert_array(
	const __flash emunit_assert_array_head_t * p_head,
	const emunit_assert_array_cmp_t            p_cmp)
{
	/** @todo Implement */
	EMUNIT_IASSERT_MGS(0, "Not implemented yet.");
}

/**
 * @brief The main function that starts the whole test
 */
#include <avr/io.h>
#include <avr/wdt.h>
int main(void)
{
#if 0
	MCUCSR = 0U;
	wdt_disable();
	static char buffer[64];
	sprintf(buffer, "Reset %.2x\n", MCUCSR);
	emunit_port_out_init();
	emunit_port_out_write(buffer, strlen(buffer));
	emunit_port_out_deinit();
#endif
#if 0
	emunit_port_out_init();
	while(true)
	{
		static const char text[] = "Hello\n";
		emunit_port_out_write(text, strlen(text));
	}
#endif
	emunit_run();
}

void before_main(void) __attribute__ ((naked)) __attribute__((used)) __attribute__ ((section (".init3")));

void before_main(void)
{
	emunit_early_init();
}