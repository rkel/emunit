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
 * @brief String assertions xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains string part of the xml output test.
 * @sa emunit_test_xml
 */
#include "test.h"
#include <emunit.h>

#define REPEAT_1(a)  a
#define REPEAT_2(a)  REPEAT_1(a) REPEAT_1(a)
#define REPEAT_4(a)  REPEAT_2(a) REPEAT_2(a)
#define REPEAT_8(a)  REPEAT_4(a) REPEAT_4(a)
#define REPEAT_16(a) REPEAT_8(a) REPEAT_8(a)
#define REPEAT_32(a) REPEAT_16(a) REPEAT_16(a)

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
	test_expect_fail_assert_here(
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

static void test_fail_str_max_escape(void)
{
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"STRING",
		NULL,
		"%s",
		"[[:space:]]*<err_idx>40</err_idx>"
		"[[:space:]]*<expected><length>40</length>"
		"[[:space:]]*<val><skip cnt=\"9\" />(&amp;){31}<err></err></val>"
		"[[:space:]]*</expected>"
		"[[:space:]]*<actual><length>48</length>"
		"[[:space:]]*<val><skip cnt=\"9\" />(&amp;){31}<err>&amp;</err><skip cnt=\"7\" /></val>"
		"[[:space:]]*</actual>"
		);
	UT_ASSERT_EQUAL_STR(
		REPEAT_32("&") REPEAT_8("&"),
		REPEAT_32("&") REPEAT_16("&"));
}

static void test_pass_nstr(void)
{
	static const char p_pattern[] = "12345678abcd";

	test_expect_success_test();
	UT_ASSERT_EQUAL_NSTR(4, "1234", p_pattern);
	UT_ASSERT_EQUAL_NSTR(4, "5678", p_pattern+4);
	UT_ASSERT_EQUAL_NSTR(6, "abcd", p_pattern+8);
}

static void test_fail_nstr(void)
{
	static const char p_pattern[] = "12345678abcd";

	UT_ASSERT_EQUAL_NSTR(4, "1234", p_pattern);
	UT_ASSERT_EQUAL_NSTR(4, "5678", p_pattern+4);
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"STRING",
		NULL,
		"%s",
		"[[:space:]]*<err_idx>5</err_idx>"
		"[[:space:]]*<expected><length>5</length>"
		"[[:space:]]*<val>abcd<err>e</err></val>"
		"[[:space:]]*</expected>"
		"[[:space:]]*<actual><length>4</length>"
		"[[:space:]]*<val>abcd<err></err></val>"
		"[[:space:]]*</actual>"
		);
	UT_ASSERT_EQUAL_NSTR(6, "abcde", p_pattern+8);
}

static void test_fail_nstr_msg(void)
{
	static const char p_pattern[] = "12345678abcd";

	UT_ASSERT_EQUAL_NSTR_MSG(4, "1234", p_pattern  , "Dummy message");
	UT_ASSERT_EQUAL_NSTR_MSG(4, "5678", p_pattern+4, "Dummy message");
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"STRING",
		"Some message",
		"%s",
		"[[:space:]]*<err_idx>5</err_idx>"
		"[[:space:]]*<expected><length>5</length>"
		"[[:space:]]*<val>abcd<err>e</err></val>"
		"[[:space:]]*</expected>"
		"[[:space:]]*<actual><length>4</length>"
		"[[:space:]]*<val>abcd<err></err></val>"
		"[[:space:]]*</actual>"
		);
	UT_ASSERT_EQUAL_NSTR_MSG(6, "abcde", p_pattern+8, "Some %s", "message");
}

UT_DESC_TS_BEGIN(test_str_suite, suite_init, suite_cleanup, NULL, NULL)
	UT_DESC_TC(test_all_passed)
	UT_DESC_TC(test_parameters)
	UT_DESC_TC(test_fail_str)
	UT_DESC_TC(test_fail_str_msg)
	UT_DESC_TC(test_fail_str_lenlim)
	UT_DESC_TC(test_fail_str_lenlimend)
	UT_DESC_TC(test_fail_str_max_escape)
	UT_DESC_TC(test_pass_nstr)
	UT_DESC_TC(test_fail_nstr)
	UT_DESC_TC(test_fail_nstr_msg)
UT_DESC_TS_END();
