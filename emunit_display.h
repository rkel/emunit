#ifndef EMUNIT_DISPLAY_H_INCLUDED
#define EMUNIT_DISPLAY_H_INCLUDED
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
 * @brief EMUnit general presentation module
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * @sa emunit_display_group
 */
#include <stdint.h>
#include "emunit_macros.h"
#include "emunit_types.h"
#include EMUNIT_DISPLAY_FILE(, h)

/**
 * @defgroup emunit_display_group <emunit_display> Presentation module
 *
 * The module that presents current test status.
 * This is the default presentation layer.
 * It is gives high flexibility for printing any text output format, by
 * using template strings.
 *
 * All the presented data are buffered into internal buffer and pushed out
 * just after reset.
 * @{
 */

/**
 * @brief Function called on every restart
 *
 * Clear the display data.
 */
void emunit_display_clear(void);

/**
 * @brief Send the whole buffer to be displayed
 *
 * Function called after system is restarted to show all messages registered
 * till now.
 *
 * This function leaves the buffer cleared.
 */
void emunit_display_present(void);

/**
 * @brief Display internal panic error message and die
 *
 * This function is called by internal asserts.
 * It is kind of message with "display always" functionality.
 *
 * The internal buffer would be cleared and filled by the given message
 * and system would be restarted.
 *
 * All the input data is given by flash strings to
 * minimise processing requirements.
 *
 * @param[in] str_file The file name where the problem occurs.
 * @param[in] str_line The string with decimal line number where the problem occurs.
 */
void emunit_display_panic(
	const __flash char * str_file,
	const __flash char * str_line);

/**
 * @name Displaying functions
 *
 * This are functions that displays the messages about current test state.
 * All of this functions generates messages into internal buffer.
 * @{
 */

/**
 * @fn emunit_display_show_panic
 * @brief Show panic message
 *
 * This function only displays it.
 * The buffer is cleared before.
 *
 * @param[in] str_file File name.
 * @param[in] str_line Line number as a string.
 */
EMUNIT_DISPLAY_MAP_VFUNC(show_panic,
		(
			const __flash char * str_file,
			const __flash char * str_line
		),
		(
			str_file,
			str_line
		)
	)

/**
 * @brief Print test start information
 *
 * Function called at at the beginning of the whole test.
 */
EMUNIT_DISPLAY_MAP_VFUNC(test_start, (void), ())

/**
 * @brief Print test finalise information
 *
 * Function called at the end of the whole test.
 */
EMUNIT_DISPLAY_MAP_VFUNC(test_end, (void), ())

/**
 * @brief Print test suite start information
 *
 * Function called before the first test in a suite
 */
EMUNIT_DISPLAY_MAP_VFUNC(ts_start, (void), ())

/**
 * @brief Print test suite finalise information
 *
 * Function called after the last test in a suite
 */
EMUNIT_DISPLAY_MAP_VFUNC(ts_end, (void), ())

/**
 * @brief Print test case start information
 *
 * Function called at the beginning of the test
 */
EMUNIT_DISPLAY_MAP_VFUNC(tc_start, (void), ())

/**
 * @brief Print test case finalise information
 *
 * Function called at the end of the test
 */
EMUNIT_DISPLAY_MAP_VFUNC(tc_end, (void), ())

/**
 * @brief Display assertion failed
 *
 * @param[in] p_head   Assertion header.
 * @param[in] str_expr String representing the expression.
 */
EMUNIT_DISPLAY_MAP_ASSERT(failed_assert,
		(
			const __flash emunit_assert_head_t * p_head,
			const __flash char * str_expr
		),
		(
			p_head,
			str_expr
		)
	)

/**
 * @brief Display equal assertion failed
 *
 * @param p_head   Assertion header.
 * @param expected Expected value.
 * @param actual   Actual value.
 */
EMUNIT_DISPLAY_MAP_ASSERT(failed_equal,
		(
			const __flash emunit_assert_head_t * p_head,
			emunit_num_t expected,
			emunit_num_t actual
		),
		(
			p_head,
			expected,
			actual
		)
	)

/**
 * @brief Display range assertion failed
 *
 * @param p_head Assertion header.
 * @param min    Minimal expected value.
 * @param max    Maximal expected value.
 * @param actual Actual value.
 */
EMUNIT_DISPLAY_MAP_ASSERT(failed_range,
		(
			const __flash emunit_assert_head_t * p_head,
			emunit_num_t min,
			emunit_num_t max,
			emunit_num_t actual
		),
		(
			p_head,
			min,
			max,
			actual
		)
	)

/**
 * @brief Display delta range assertion failed
 *
 * @param p_head   Assertion header.
 * @param delta    Maximum delta allowed.
 * @param expected The middle of the expected range.
 * @param actual   Actual value.
 */
EMUNIT_DISPLAY_MAP_ASSERT(failed_delta,
		(
			const __flash emunit_assert_head_t * p_head,
			emunit_unum_t delta,
			emunit_num_t expected,
			emunit_num_t actual
		),
		(
			p_head,
			delta,
			expected,
			actual
		)
	)

/**
 * @brief Display string comparison assertion failed
 *
 * @param p_head       Assertion header.
 * @param n            Maximum number of printed characters.
 *                     For maximum value see @ref EMUNIT_STRLEN_MAX.
 * @param str_expected Expected string.
 * @param str_actual   Actual string.
 * @param err_pos      Index of the first mismatched character.
 */
EMUNIT_DISPLAY_MAP_ASSERT(failed_nstr,
		(
			const __flash emunit_assert_head_t * p_head,
			size_t n,
			const __memx char * str_expected,
			const __memx char * str_actual,
			size_t err_pos
		),
		(
			p_head,
			n,
			str_expected,
			str_actual,
			err_pos
		)
	)
/** @} */

/** @} */
#endif /* EMUNIT_DISPLAY_H_INCLUDED */
