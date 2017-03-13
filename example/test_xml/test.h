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

/** @} */
#endif /* TEST_XML_H_INCLUDED */
