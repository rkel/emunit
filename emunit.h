#ifndef EMUNIT_H_INCLUDED
#define EMUNIT_H_INCLUDED
/**
 * @file
 * @brief EMUnit header
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * @sa emunit_group
 */
#include "emunit_assertions.h"
#include "emunit_macros.h"
#include "emunit_types.h"
/**
 * @defgroup emunit_group <emunit> EMUnit interface
 *
 * The main interface of EMUnit.
 * @{
 */

/**
 * @brief Mark the fact that assertion test has passed
 *
 * This function should be called when assertion test has passed.
 * It is used for internal statistic.
 */
void emunit_assert_passed(void);

/**
 * @brief React on the fact that assertion test has failed
 *
 * This function is used for internal statistic.
 * @note
 * This function usually just restarts the program.
 * It should be the last function called in assert checking.
 */
void emunit_assert_failed(void);

/**
 * @name Assertion functions
 *
 * Functions used for assertions in the unit tests.
 * All functions here have ut_ prefix.
 * @{
 */

/**
 * @brief Boolean type assertion function
 *
 * @param p_head    Assertion header
 * @param p_exp     String representation of the expression
 * @param condition Boolean value to be validated
 */
void ut_assert(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * p_exp,
	bool condition);

/**
 * @brief Boolean type assertion function
 *
 * @param p_head    Assertion header
 * @param p_exp     String representation of the expression
 * @param condition Boolean value to be validated
 * @param fmt       Message format string
 * @param ...       Message parameters
 */
void ut_assert_msg(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * p_exp,
	bool condition,
	const __flash char * fmt,
	...);

/**
 * @brief Numeric type comparison
 *
 * @param p_head   Assertion header
 * @param expected Expected value
 * @param actual   Actual value
 */
void ut_assert_equal(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t expected,
	emunit_num_t actual);

/**
 * @brief Numeric type comparison
 *
 * @param p_head   Assertion header
 * @param expected Expected value
 * @param actual   Actual value
 * @param fmt      Message format string
 * @param ...      Message parameters
 */
void ut_assert_equal_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t expected,
	emunit_num_t actual,
	const __flash char * fmt,
	...);

/**
 * @brief Check if specified value is in defined delta
 *
 * Checks if (expected - delta) <= actual <= (expected + delta).
 *
 * @param p_head   Assertion header
 * @param delta    Specified delta
 * @param expected Expected value
 * @param actual   Actual value
 */
void ut_assert_delta(
	const __flash emunit_assert_head_t * p_head,
	emunit_unum_t delta,
	emunit_num_t  expected,
	emunit_num_t  actual);

/**
 * @brief Check if specified value is in defined delta
 *
 * Checks if (expected - delta) <= actual <= (expected + delta).
 *
 * @param p_head   Assertion header
 * @param delta    Specified delta
 * @param expected Expected value
 * @param actual   Actual value
 * @param fmt      Message format string
 * @param ...      Message parameters
 */
void ut_assert_delta_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_unum_t delta,
	emunit_num_t  expected,
	emunit_num_t  actual,
	const __flash char * fmt,
	...);

/**
 * @brief Check if given value is in selected range
 *
 * Function checks if:
 * (min <= actual <= max)
 *
 * If min > max the actual value is checked to be outside selected range:
 * (min <= actual <= RANGE_MAX) || (RANGE_MIN <= actual <= max)
 *
 * @param p_head Assertion header
 * @param min    Minimal value
 * @param max    Maximal value
 * @param actual Actual value
 */
void ut_assert_range(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t min,
	emunit_num_t max,
	emunit_num_t actual);

/**
 * @brief Check if given value is in selected range
 *
 * Function checks if:
 * (min <= actual <= max)
 *
 * If min > max the actual value is checked to be outside selected range:
 * (min <= actual <= RANGE_MAX) || (RANGE_MIN <= actual <= max)
 *
 * @param p_head Assertion header
 * @param min    Minimal value
 * @param max    Maximal value
 * @param actual Actual value
 * @param fmt    Message format string
 * @param ...    Message parameters
 */
void ut_assert_range_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t min,
	emunit_num_t max,
	emunit_num_t actual,
	const __flash char * fmt,
	...);

/**
 * @brief Check if given strings are the same
 *
 * Function checks if selected strings equals.
 *
 * @param p_head   Assertion header
 * @param expected Expected string
 * @param actual   Actual string
 */
void ut_assert_str(
	const __flash emunit_assert_head_t * p_head,
	const __memx char * expected,
	const __memx char * actual);

/**
 * @brief Check if given strings are the same
 *
 * Function checks if selected strings equals.
 *
 * @param p_head   Assertion header
 * @param expected Expected string
 * @param actual   Actual string
 * @param fmt      Message format string
 * @param ...      Message parameters
 */
void ut_assert_str_msg(
	const __flash emunit_assert_head_t * p_head,
	const __memx char * expected,
	const __memx char * actual,
	const __flash char * fmt,
	...);

/**
 * @brief Assert arrays
 *
 * Complex assertion that uses comparator defined in array head.
 * It gives an possibility to compare any kind of structurized data.
 *
 * @param p_head  Array assertion header
 * @param p_cmp   Structure with the data about compared arrays.
 */
void ut_assert_array(
	const __flash emunit_assert_array_head_t * p_head,
	const emunit_assert_array_cmp_t            p_cmp);

/** @} */

/** @} */
#endif /* EMUNIT_H_INCLUDED */