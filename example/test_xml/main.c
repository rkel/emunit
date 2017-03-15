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
#include <stdarg.h>
#include <stdio.h>
/* Note that normal test does not include this header.
 * This test checks emunit engine itself so it needs the access to the private
 * functions. */
#include <emunit_private.h>


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


void test_expect_sinit_default(char const * str_suite)
{
	char buffer[512];

	sprintf(
		buffer,
		"^[[:space:]]*<testsuite name=\"%s\">[[:space:]]*$",
		str_suite);

	emunit_pctest_expected_set(buffer);
}

void test_expect_scleanup_default(void)
{
	emunit_pctest_expected_set("^[[:space:]]*</testsuite>[[:space:]]*$");
}

void test_expect_success(char const * pattern)
{
	++test_cases_success;
	emunit_pctest_expected_set(pattern);
}

void test_expect_success_test_x(char const * str_name)
{
	char buffer[512];

	sprintf(
		buffer,
		"^[[:space:]]*<testcase name=\"%s\">[[:space:]]*</testcase>[[:space:]]*$",
		str_name);
	test_expect_success(buffer);
}

void test_expect_fail(char const * pattern)
{
	++test_cases_failed;
	emunit_pctest_expected_set(pattern);
}

void test_expect_fail_assert_x(
	char const * str_test,
	char const * str_file,
	char const * str_id,
	char const * str_type,
	char const * msg,
	char const * fmt,
	...)
{
	char buffer[512];
	char * p_buffer = buffer;

	char esc_filename[256];
	(void)emunit_pctest_regex_esc(esc_filename, str_file);

	p_buffer += sprintf(p_buffer,
		"^"
		"[[:space:]]*<testcase name=\"%s\">"
		"[[:space:]]*<failure type=\"%s\" id=\"%s\">"
		"[[:space:]]*<file>%s</file>"
		"[[:space:]]*<line>[[:digit:]]+</line>"
		,
		str_test,
		str_type,
		str_id,
		esc_filename
		);
	if(NULL != msg)
	{
		p_buffer += sprintf(p_buffer,
			"[[:space:]]*<msg>%s</msg>",
			msg
			);
	}

	p_buffer += sprintf(p_buffer,
		"[[:space:]]*<details>");

	va_list vargs;
	va_start(vargs, fmt);
	p_buffer += vsprintf(p_buffer, fmt, vargs);
	va_end(vargs);

	p_buffer += sprintf(p_buffer,
		"[[:space:]]*</details>"
		"[[:space:]]*</failure>"
		"[[:space:]]*</testcase>"
		"[[:space:]]*"
		"$");

	test_expect_fail(buffer);
}

/**
 * @defgroup emunit_test_xml_tests General tests of the XML output
 * @ingroup emunit_test_xml
 *
 * Group of general tests performed.
 */

/* Prepare for the test header. Function documented in the port header. */
void emunit_test_prepare(void)
{
	const char pattern[] =
		"^<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?>[[:space:]]*"
		"<test name=\"EMUnit\">[[:space:]]*$";
	emunit_pctest_expected_set(pattern);
}

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
	test_expect_sinit_default("base_tests");
	++suite_init_calls;
}

/**
 * @brief Suite cleanup
 */
static void base_tests_scleanup(void)
{
	test_expect_scleanup_default();
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
	test_expect_success_test();

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
	test_expect_success_test();

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
	test_expect_fail_assert(
		"1",
		"ASSERT",
		NULL,
		"%s",
		"[[:space:]]*<expression>false == true</expression>");

	UT_ASSERT_EQUAL(1, suite_init_calls);
	UT_ASSERT_EQUAL(0, suite_cleanup_calls);
	UT_ASSERT_EQUAL(3, init_calls);
	UT_ASSERT_EQUAL(2, cleanup_calls);

	UT_ASSERT(false == true);
}

/**
 * @brief Second test that fails
 *
 * This test checks init and cleanup counters - if cleanup is called properly after assertion fails.
 * So it has to be the fourth one called.
 */
static void test4(void)
{
	void * p_null = NULL;

	test_expect_fail_assert(
		"2",
		"ASSERT",
		NULL,
		"[[:space:]]*<expression>\\(p_null\\) != NULL</expression>");

	UT_ASSERT_EQUAL(1, suite_init_calls);
	UT_ASSERT_EQUAL(0, suite_cleanup_calls);
	UT_ASSERT_EQUAL(4, init_calls);
	UT_ASSERT_EQUAL(3, cleanup_calls);

	UT_ASSERT_NOT_NULL(p_null);
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
	test_expect_sinit_default("base_tests2");
	++suite2_init_calls;
}

/**
 * @brief Suite cleanup
 */
static void base_tests2_scleanup(void)
{
	test_expect_scleanup_default();
}

/** @} <!-- emunit_test_xml_base_tests2_init --> */

/**
 * @brief Test in second suite
 *
 * This tests checks all variables that counts number of calls from suite above.
 */
static void base_tests2_test1(void)
{
	test_expect_success_test();
	UT_ASSERT_EQUAL(1, suite_init_calls);
	UT_ASSERT_EQUAL(1, suite2_init_calls);
	UT_ASSERT_EQUAL(1, suite_cleanup_calls);
	UT_ASSERT_EQUAL(4, init_calls);
	UT_ASSERT_EQUAL(4, cleanup_calls);
}
/** @} <!-- emunit_test_xml_base_tests2 --> */

/**
 * @defgroup emunit_test_xml_base_tests_last base_tests_last Test suite that has to be executed as a last one
 * @{
 * @ingroup emunit_test_xml_tests
 *
 * This suite is here only to prepare test for the footer.
 * It has to be placed as a last one.
 */

static void base_tests_last_sinit(void)
{
	test_expect_sinit_default("base_tests_last");
	++suite2_init_calls;
}

/**
 * @brief Suite cleanup
 */
static void base_tests_last_scleanup(void)
{
	test_expect_scleanup_default();

	char pattern_footer[512];

	sprintf(pattern_footer,
		"^"
		"[[:space:]]*<testsummary>"
		"[[:space:]]*<testsuite-stat>"
		"[[:space:]]*<total>%u</total>"
		"[[:space:]]*<passed>%u</passed>"
		"[[:space:]]*<failed>%u</failed>"
		"[[:space:]]*</testsuite-stat>"
		"[[:space:]]*<testcase-stat>"
		"[[:space:]]*<total>%u</total>"
		"[[:space:]]*<passed>%u</passed>"
		"[[:space:]]*<failed>%u</failed>"
		"[[:space:]]*</testcase-stat>"
		"[[:space:]]*</testsummary>"
		"[[:space:]]*</test>[[:space:]]*"
		"$"
		,
		emunit_ts_total_count(),
		emunit_ts_passed_get(),
		emunit_ts_failed_get(),
		test_cases_success + test_cases_failed,
		test_cases_success,
		test_cases_failed
	);
	emunit_pctest_expected_footer_set(pattern_footer);
}

/** @} */


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

/* Last test suite */
UT_DESC_TS_BEGIN(base_tests_last, base_tests_last_sinit, base_tests_last_scleanup, NULL, NULL)
UT_DESC_TS_END();
