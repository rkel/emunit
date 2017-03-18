/**
 * @file
 * @brief String assertions xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains string part of the xml output test.
 * @sa emunit_test_xml
 */
#include "test.h"
#include <emunit.h>

static void suite_init(void)
{
	test_expect_sinit_default("test_str_suite");
}

static void suite_cleanup(void)
{
	test_expect_scleanup_default();
}

static void test_all_passed(void)
{
	test_expect_success_test();

	static const __flash char m_test_f[] = "First string to test";
	static const char m_test_m[] = "Second string to test";
	UT_ASSERT_EQUAL_STR("First string to test"                  , m_test_f);
	UT_ASSERT_EQUAL_STR(EMUNIT_FLASHSTR("First string to test") , m_test_f);
	UT_ASSERT_EQUAL_STR("Second string to test"                 , m_test_m);
	UT_ASSERT_EQUAL_STR(EMUNIT_FLASHSTR("Second string to test"), m_test_m);

	UT_ASSERT_EQUAL_STR_MSG("First string to test"                  , m_test_f, "Dummy message");
	UT_ASSERT_EQUAL_STR_MSG(EMUNIT_FLASHSTR("First string to test") , m_test_f, "Dummy message");
	UT_ASSERT_EQUAL_STR_MSG("Second string to test"                 , m_test_m, "Dummy message");
	UT_ASSERT_EQUAL_STR_MSG(EMUNIT_FLASHSTR("Second string to test"), m_test_m, "Dummy message");
}


static void test_parameters(void)
{
	test_expect_success_test();
	/* If this changes the rest of the tests may be invalid */
	UT_ASSERT_EQUAL(32, EMUNIT_CONF_STRLEN_LIMIT);
}

static void test_fail_str(void)
{
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"STRING",
		NULL,
		"%s",
		"[[:space:]]*<err_idx>10</err_idx>"
		"[[:space:]]*<expected><length>14</length>"
		"[[:space:]]*<val>0123456789<err>a</err>bcd</val>"
		"[[:space:]]*</expected>"
		"[[:space:]]*<actual><length>14</length>"
		"[[:space:]]*<val>0123456789<err>b</err>acd</val>"
		"[[:space:]]*</actual>"
		);
	UT_ASSERT_EQUAL_STR("0123456789abcd", "0123456789bacd");
}

static void test_fail_str_msg(void)
{
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"STRING",
		"SomeMessage",
		"%s",
		"[[:space:]]*<err_idx>10</err_idx>"
		"[[:space:]]*<expected><length>14</length>"
		"[[:space:]]*<val>0123456789<err>a</err>bcd</val>"
		"[[:space:]]*</expected>"
		"[[:space:]]*<actual><length>14</length>"
		"[[:space:]]*<val>0123456789<err>b</err>acd</val>"
		"[[:space:]]*</actual>"
		);
	UT_ASSERT_EQUAL_STR_MSG("0123456789abcd", "0123456789bacd", "%s%s", "Some", "Message");
}

static void test_fail_str_lenlim(void)
{
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"STRING",
		NULL,
		"%s",
		"[[:space:]]*<err_idx>34</err_idx>"
		"[[:space:]]*<expected><length>34</length>"
		"[[:space:]]*<val><skip cnt=\"3\" />3456789abcdefghijklmnoprstuwxyz<err></err></val>"
		"[[:space:]]*</expected>"
		"[[:space:]]*<actual><length>35</length>"
		"[[:space:]]*<val><skip cnt=\"3\" />3456789abcdefghijklmnoprstuwxyz<err>a</err></val>"
		"[[:space:]]*</actual>"
		);
	UT_ASSERT_EQUAL_STR(
		"0123456789abcdefghijklmnoprstuwxyz",
		"0123456789abcdefghijklmnoprstuwxyza");
}

static void test_fail_str_lenlimend(void)
{
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"STRING",
		NULL,
		"%s",
		"[[:space:]]*<err_idx>34</err_idx>"
		"[[:space:]]*<expected><length>43</length>"
		"[[:space:]]*<val><skip cnt=\"3\" />3456789abcdefghijklmnoprstuwxyz<err>1</err><skip cnt=\"8\" /></val>"
		"[[:space:]]*</expected>"
		"[[:space:]]*<actual><length>38</length>"
		"[[:space:]]*<val><skip cnt=\"3\" />3456789abcdefghijklmnoprstuwxyz<err>a</err><skip cnt=\"3\" /></val>"
		"[[:space:]]*</actual>"
		);
	UT_ASSERT_EQUAL_STR(
		"0123456789abcdefghijklmnoprstuwxyz123456789",
		"0123456789abcdefghijklmnoprstuwxyzabcd");
}


UT_DESC_TS_BEGIN(test_str_suite, suite_init, suite_cleanup, NULL, NULL)
	UT_DESC_TC(test_all_passed)
	UT_DESC_TC(test_parameters)
	UT_DESC_TC(test_fail_str)
	UT_DESC_TC(test_fail_str_msg)
	UT_DESC_TC(test_fail_str_lenlim)
	UT_DESC_TC(test_fail_str_lenlimend)
UT_DESC_TS_END();
