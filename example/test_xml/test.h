#ifndef TEST_H_INCLUDED
#define TEST_XML_H_INCLUDED
/**
 * @file
 * @brief Main xml test header file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * The file that contains the declaration of the interface for the main xml test
 * application.
 * @sa emunit_test_xml
 */

/**
 * @defgroup emunit_test_xml Test XML output main functions
 *
 * Group of the functions in XML output test that may be used in
 * files that declares different suites.
 * @{
 */

/**
 * @brief Current test should finish successfully
 *
 * Function sets expected pattern and marks that current test should finish
 * successfully.
 *
 * @param[in] pattern The expected output pattern regular expression.
 */
void test_expect_success(char const * pattern);

/**
 * @brief Current test should fail
 *
 * Function sets expected pattern and marks that current test should fail.
 *
 * @param[in] pattern The expected output pattern regular expression.
 */
void test_expect_fail(char const * pattern);

/**
 * @brief The string for any valid id matching
 *
 * This string may be used in @ref test_expect_fail_assert for any id matching.
 */
#define TEST_STR_ID_ANY "[[:digit:]]+"

/**
 * @brief Current test should fail formatted pattern
 *
 * Function sets expected pattern and marks that current test should fail.
 *
 * The pattern is created this way that the expected header and footer is
 * builded internally.
 * Only the details part should be defined.
 *
 * @note
 * It uses @ref test_expect_fail_assert_x function internally.
 *
 * @note
 * This macro sets expected file to fail and expected test based on the
 * current file and current function name.
 *
 * @param str_id   Expected failure id.
 *                 The string @ref TEST_STR_ID_ANY may be used for any valid ID
 *                 matching.
 * @param str_type Type of the assertion that fails.
 *                 Used in <failure type="str_type">.
 * @param msg      Message or NULL if none.
 * @param ...      Detailed part format string followed by format arguments.
 */
#define test_expect_fail_assert(str_id, str_type, msg, ...) \
	test_expect_fail_assert_x(__func__, __FILE__, str_id, str_type, msg, __VA_ARGS__)

/**
 * @brief Current should fail formatted pattern internal function
 *
 * @note
 * Do not use this function directly.
 * Use @ref test_expect_fail_assert instead.
 *
 * @param str_func The test name
 * @param str_file The file where failure should be expected
 * @param str_id   The failure identifer.
 * @param str_type The type of the assertion that fails
 * @param msg      The message or NULL if none.
 * @param fmt      Details format string.
 * @param ...      Details format arguments.
 */
void test_expect_fail_assert_x(
	char const * str_func,
	char const * str_file,
	char const * str_id,
	char const * str_type,
	char const * msg,
	char const * fmt,
	...);

/** @} */
#endif /* TEST_XML_H_INCLUDED */
