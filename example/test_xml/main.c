/**
 * @file
 * @brief Main xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains main part of the xml output test.
 * @sa emunit_test_xml
 */
#include "test.h"
#include <emunit.h>
#include <emunit_port.h>


/**
 * @internal
 * @defgroup emunit_test_xml_internals Test XML output internals
 * {
 * @ingroup emunit_test_xml
 *
 * Internal functions and variables of the main body of XML output test.
 */
/**
 * @brief Number of suite init calls
 */
static int suite_init_calls = 0;
/**
 * @brief Number of suite cleanup calls
 */
static int suite_cleanup_calls = 0;

/**
 * @brief Number of second suite init calls
 */
static int suite2_init_calls = 0;

/**
 * @brief Number of test init calls
 */
static int init_calls    = 0;
/**
 * @brief Number of test cleanup calls
 */
static int cleanup_calls = 0;

/**
 * @brief Count tests that should fail
 */
static int test_cases_success;
/**
 * @brief Count tests that should success
 */
static int test_cases_failed;
/** @} <!-- emunit_test_xml_internals --> */


void test_expect_success(char const * pattern)
{
	++test_cases_success;
	emunit_pctest_expected_set(pattern);
}

void test_expect_fail(char const * pattern)
{
	++test_cases_failed;
	emunit_pctest_expected_set(pattern);
}

/**
 * @defgroup emunit_test_xml_tests General tests of the XML output
 * @ingroup emunit_test_xml
 *
 * Group of general tests performed.
 */

/**
 * @defgroup emunit_test_xml_base_tests base_tests Base functionality tests
 * @{
 * @ingroup emunit_test_xml_tests
 *
 * Very basic bunch of tests that checks it all the required functions are called.
 */

/**
 * @defgroup emunit_test_xml_base_tests_init base_tests_init Test initialisation
 * @{
 *
 * Initialisation functions for base functionality tests
 */

/**
 * @brief Suite initialisation
 */
static void base_tests_sinit(void)
{
	const char pattern[] =
		"^[[:space:]]*<testsuite name=\"base_tests\">";
	emunit_pctest_expected_set(pattern);
	++suite_init_calls;
}

/**
 * @brief Suite cleanup
 */
static void base_tests_scleanup(void)
{
	const char pattern[] =
		"^[[:space:]]*</testsuite>[[:space:]]*$";
	emunit_pctest_expected_set(pattern);
	++suite_cleanup_calls;
}

/**
 * @brief Test initialisation
 */
static void test_init(void)
{
	++init_calls;
}

/**
 * @brief Test cleanup
 */
static void test_cleanup(void)
{
	++cleanup_calls;
}
/** @} <!-- emunit_test_xml_base_tests_init --> */

/**
 * @brief First test that finishes with success
 *
 * This test checks init and cleanup counters so it has to be the first one called.
 */
static void test1(void)
{
	const char pattern[] =
		"^[[:space:]]*<testcase name=\"test1\">[[:space:]]*</testcase>[[:space:]]*$";
	test_expect_success(pattern);

	UT_ASSERT_EQUAL(1, suite_init_calls);
	UT_ASSERT_EQUAL(0, suite_cleanup_calls);
	UT_ASSERT_EQUAL(1, init_calls);
	UT_ASSERT_EQUAL(0, cleanup_calls);
}

/**
 * @brief Second test that finishes with success
 *
 * This test checks init and cleanup counters so it has to be the second one called.
 */
static void test2(void)
{
	const char pattern[] =
		"^[[:space:]]*<testcase name=\"test2\">[[:space:]]*</testcase>[[:space:]]*$";
	test_expect_success(pattern);

	UT_ASSERT_EQUAL(1, suite_init_calls);
	UT_ASSERT_EQUAL(0, suite_cleanup_calls);
	UT_ASSERT_EQUAL(2, init_calls);
	UT_ASSERT_EQUAL(1, cleanup_calls);
}

/**
 * @brief First test that fails
 *
 * This test checks init and cleanup counters so it has to be the third one called.
 */
static void test3(void)
{

}

/**
 * @brief Second test that fails
 *
 * This test checks init and cleanup counters - if cleanup is called properly after assertion fails.
 * So it has to be the fourth one called.
 */
static void test4(void)
{

}

/** @} <!-- emunit_test_xml_base_tests --> */


/**
 * @defgroup emunit_test_xml_base_tests2 base_tests2 Base functionality tests 2
 * @{
 * @ingroup emunit_test_xml_tests
 *
 * Tests that checks if all the functions runs like expected in @ref emunit_test_xml_base_tests.
 */

/**
 * @defgroup emunit_test_xml_base_tests2_init base_tests2_init Test initialisation
 * @{
 *
 * Initialisation functions for base functionality tests 2
 */

/**
 * @brief Suite initialisation
 */
static void base_tests2_sinit(void)
{
	const char pattern[] =
		"^[[:space:]]*<testsuite name=\"base_tests2\">";
	emunit_pctest_expected_set(pattern);
	++suite2_init_calls;
}

/**
 * @brief Suite cleanup
 */
static void base_tests2_scleanup(void)
{
	const char pattern[] =
		"^[[:space:]]*</testsuite>[[:space:]]*$";
	emunit_pctest_expected_set(pattern);
}

/** @} <!-- emunit_test_xml_base_tests2_init --> */

/**
 * @brief Test in second suite
 *
 * This tests checks all variables that counts number of calls from suite above.
 */
static void base_tests2_test1(void)
{
	const char pattern[] =
		"^[[:space:]]*<testcase name=\"base_tests2_test1\">[[:space:]]*</testcase>[[:space:]]*$";
	test_expect_success(pattern);
	UT_ASSERT_EQUAL(1, suite_init_calls);
	UT_ASSERT_EQUAL(1, suite2_init_calls);
	UT_ASSERT_EQUAL(1, suite_cleanup_calls);
	UT_ASSERT_EQUAL(4, init_calls);
	UT_ASSERT_EQUAL(4, cleanup_calls);
}

/** @} <!-- emunit_test_xml_base_tests2 --> */


/* Test suite 1 */
UT_DESC_TS_BEGIN(base_tests, base_tests_sinit, base_tests_scleanup, test_init, test_cleanup)
	UT_DESC_TC(test1)
	UT_DESC_TC(test2)
	UT_DESC_TC(test3)
	UT_DESC_TC(test4)
UT_DESC_TS_END();

/* Test suite 2 */
UT_DESC_TS_BEGIN(base_tests2, base_tests2_sinit, base_tests2_scleanup, NULL, NULL)
	UT_DESC_TC(base_tests2_test1)
UT_DESC_TS_END();



/* Whole test description */
UT_MAIN_TS_BEGIN()
	UT_MAIN_TS_ENTRY(base_tests)
	UT_MAIN_TS_ENTRY(base_tests2)
UT_MAIN_TS_END();
