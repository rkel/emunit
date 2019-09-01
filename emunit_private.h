#ifndef EMUNIT_PRIVATE_H_INCLUDED
#define EMUNIT_PRIVATE_H_INCLUDED
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
 * @brief EMUnit private
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * @sa emunit_private_group
 */
#include "emunit.h"
#include "emunit_types.h"
#include "emunit_display.h"

/**
 * @defgroup emunit_private_group <emunit_private> EMUnit private
 *
 * Private parts of EMUnit.
 *
 * @note
 * This module would be included by EMUnit files internally.
 * My be used by architecture, display or port implementations
 * but should not be used inside the tests.
 *
 * @{
 */

/**
 * @name Internal assertions
 *
 *  Internal assertions have highest priority.
 * Assertions would be sent to selected port.
 * If internal assertion fails, the current buffer would be cleared and
 * replaced by assertion text - so probably output format would be broken.
 *
 * They are used mainly to debug during development.
 * @{
 */

/**
 * @brief Internal assertion
 *
 * Macro that is used for internal EMUnit checking.
 * When failed - panic assertion would be presented.
 *
 * @param exp The expression to be evaluated
 */
#define EMUNIT_IASSERT(exp) do{                         \
		if(!(exp)) {                                    \
			emunit_display_panic(                       \
				EMUNIT_FLASHSTR(__FILE__),              \
				__LINE__);                              \
			emunit_restart(EMUNIT_RR_PANIC);            \
		}                                               \
	}while(0)

/**
 * @brief Internal assertion with message
 *
 * The version of the @ref EMUNIT_IASSERT that accepts message parameter.
 *
 * @param exp The expression to be evaluated
 * @param msg Message string
 *
 * @note Current implementation does not use given message string
 *       in any way.
 *       This message is here for the reader that looks in the code
 *       to search more detail about the failure.
 */
#define EMUNIT_IASSERT_MSG(exp, msg)  EMUNIT_IASSERT(exp)
/** @} */

/**
 * @brief Check if given numeric type is signed or not
 *
 * @param type Type to be checked
 *
 * @retval true  Type is signed
 * @retval false Type is unsigned
 */
static inline bool emunit_numtype_signed_check(emunit_numtypes_t type)
{
	switch(type)
	{
	case EMUNIT_NUMTYPE_S8:
	case EMUNIT_NUMTYPE_S16:
#if (EMUNIT_CONF_NUMBER_SIZE > 16) || defined(__DOXYGEN__)
	case EMUNIT_NUMTYPE_S32:
#endif
#if (EMUNIT_CONF_NUMBER_SIZE > 32) || defined(__DOXYGEN__)
	case EMUNIT_NUMTYPE_S64:
#endif
		return true;
	default:
		return false;
	}
}

/**
 * @brief Check if giben numeric type is hexadecimal or not
 *
 * @param type Type to be checked
 *
 * @retval true  Type is hexadecimal
 * @retval false Type is not hexadecimal
 */
static inline bool emunit_numtype_hex_check(emunit_numtypes_t type)
{
	switch(type)
		{
		case EMUNIT_NUMTYPE_X8:
		case EMUNIT_NUMTYPE_X16:
	#if (EMUNIT_CONF_NUMBER_SIZE > 16) || defined(__DOXYGEN__)
		case EMUNIT_NUMTYPE_X32:
	#endif
	#if (EMUNIT_CONF_NUMBER_SIZE > 32) || defined(__DOXYGEN__)
		case EMUNIT_NUMTYPE_X64:
	#endif
			return true;
		default:
			return false;
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
void emunit_early_init(void);

/**
 * @brief Flush display buffers
 *
 * Function that flushes anything that may be left in the display buffer.
 *
 * Should be called before @ref emunit_run.
 */
void emunit_flush(void);

/**
 * @brief Run the EMUnit
 *
 * This function should be called when program is started.
 * Should be called from main, directly after @ref emunit_early_init.
 *
 * @return If EMUnit testing was finished properly it would return number
 *         of failed suites.
 *         The value (-1) would be returned if test fails internally
 *         (because of internal assertion or unexpected state).
 *         The 0 value always means that everything finishes successfully.
 */
int emunit_run(void);

/**
 * @brief Restart test now
 *
 * Function called when test has to be restarted.
 *
 * @param rr Restart reason
 */
void emunit_restart(emunit_rr_t rr) __attribute__ ((noreturn));

/**
 * @name Internal access interface
 *
 * The interface for accessing the internal informations.
 * This functions would be used mainly in the display module.
 * @{
 */

/**
 * @brief Gets the whole test, global name
 *
 * @return The pointer to a string that contains the name.
 */
const __flash char * emunit_test_name_get(void);

/**
 * @brief Get currently running test suite index
 *
 * @return The zero based index of the current test suite.
 */
size_t emunit_ts_current_index_get(void);
/**
 * @brief Get the name of the test suite
 *
 * @param suite_idx Index of the test suite witch name return.
 *
 * @return The pointer to a string that contains the name.
 */
const __flash char * emunit_ts_name_get(size_t suite_idx);
/**
 * @brief Get total number of test suites
 *
 * This function counts total number of defined test suites.
 * It is constant value, not related to the current test suite index.
 *
 * @return Total number ot test suites.
 */
size_t emunit_ts_total_count(void);
/**
 * @brief Get total number of failed test suites
 *
 * The value of failed test suites is updated before calling the display
 * function.
 *
 * @return Current number of failed test suites
 */
size_t emunit_ts_failed_get(void);
/**
 * @brief Get total number of passed test suites
 *
 * The value of passed test suites is updated before calling the display
 * function.
 *
 * @return Current number of passed test suites
 */
size_t emunit_ts_passed_get(void);

/**
 * @brief Get currently running test case index
 *
 * @return The zero based index of the current test suite.
 *         May return @ref EMUNIT_IDX_INVALID.
 *
 * @sa EMUNIT_IDX_INVALID
 */
size_t emunit_tc_current_index_get(void);
/**
 * @brief Get the name of the test case
 *
 * @param suite_idx Index of the test suite where to search the test case.
 * @param test_idx Index of the test case witch name return.
 *
 * @return The pointer to a string that contains the name.
 */
const __flash char * emunit_tc_name_get(size_t suite_idx, size_t test_idx);
/**
 * @brief Get total number of test cases in the suite
 *
 * This function counts total number of defined test cases in the suite.
 * It is constant value, not related to the current
 * test suite or test case indexes.
 *
 * @param suite_idx Index of the test suite in witch
 *                  test cases should be counted.
 *
 * @return Total number ot test cases.
 */
size_t emunit_tc_count(size_t suite_idx);
/**
 * @brief Get total number of failed test cases
 *
 * The value of failed test cases is updated before calling the display
 * function.
 *
 * @return Current number of failed test cases
 */
size_t emunit_tc_failed_get(void);
/**
 * @brief Get total number of passed test cases
 *
 * The value of passed test cases is updated before calling the display
 * function.
 *
 * @return Current number of passed test cases
 */
size_t emunit_tc_passed_get(void);
/**
 * @brief Count total number of test cases
 *
 * Auxiliary function that travels all test suites to count
 * all tests cases in whole test.
 *
 * @return Total number of test cases
 */
size_t emunit_tc_total_count(void);
/** @} */


/** @} */
#endif /* EMUNIT_PRIVATE_H_INCLUDED */
