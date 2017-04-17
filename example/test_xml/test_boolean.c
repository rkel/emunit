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
 * @brief Boolean assertions xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains boolean part of the xml output test.
 * @sa emunit_test_xml
 */
#include "test.h"
#include <emunit.h>
#include <stdbool.h>


static void suite_init(void)
{
	test_expect_sinit_default("test_boolean_suite");
}

static void suite_cleanup(void)
{
	test_expect_scleanup_default();
}


static void test_all_passed(void)
{
	test_expect_success_test();

	UT_ASSERT         (true    );
	UT_ASSERT_TRUE    (true    );
	UT_ASSERT_FALSE   (false   );
	UT_ASSERT_NULL    (NULL    );
	UT_ASSERT_NOT_NULL((void*)1);
}

static void test_all_passed_msg(void)
{
	test_expect_success_test();

	UT_ASSERT_MSG          (true   , "Dummy message");
	UT_ASSERT_TRUE_MSG     (true   , "Dummy message");
	UT_ASSERT_FALSE_MSG    (false  , "Dummy message");
	UT_ASSERT_NULL_MSG     (NULL   , "Dummy message");
	UT_ASSERT_NOT_NULL_MSG((void*)1, "Dummy message");
}

static void test_fail_assert(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"ASSERT",
		NULL,
		"%s",
		"[[:space:]]*<expression>1 == 2</expression>");
	UT_ASSERT(1 == 2);
}

static void test_fail_assert_msg(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"ASSERT",
		"Some message: 1",
		"%s",
		"[[:space:]]*<expression>17 == 32</expression>");
	UT_ASSERT_MSG(17 == 32, "Some message: %d", 1);
}

static void test_fail_assert_true(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"EQUAL",
		NULL,
		"%s",
		"[[:space:]]*<expected>true</expected>"
		"[[:space:]]*<actual>false</actual>"
		);
	UT_ASSERT_TRUE(1 != 1);
}

static void test_fail_assert_true_msg(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"EQUAL",
		"SuperMessage",
		"%s",
		"[[:space:]]*<expected>true</expected>"
		"[[:space:]]*<actual>false</actual>"
		);
	UT_ASSERT_TRUE_MSG(1 != 1, "%s%s", "Super", "Message");
}

static void test_fail_assert_false(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"EQUAL",
		NULL,
		"%s",
		"[[:space:]]*<expected>false</expected>"
		"[[:space:]]*<actual>true</actual>"
		);
	UT_ASSERT_FALSE(1 == 1);
}

static void test_fail_assert_false_msg(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"EQUAL",
		"SuperFalseMessage",
		"%s",
		"[[:space:]]*<expected>false</expected>"
		"[[:space:]]*<actual>true</actual>"
		);
	UT_ASSERT_FALSE_MSG(1 == 1, "%s%s%s", "Super", "False", "Message");
}

static void test_fail_assert_null(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"EQUAL",
		NULL,
		"%s",
		"[[:space:]]*<expected>0x0</expected>"
		"[[:space:]]*<actual>0x1234</actual>"
		);
	UT_ASSERT_NULL(0x1234);
}

static void test_fail_assert_null_msg(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"EQUAL",
		"Message 0x12",
		"%s",
		"[[:space:]]*<expected>0x0</expected>"
		"[[:space:]]*<actual>0x1234</actual>"
		);
	UT_ASSERT_NULL_MSG(0x1234, "Message 0x%x", 0x12);
}

static void test_fail_assert_not_null(void)
{
	void * ptr_3456 = NULL;
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"ASSERT",
		NULL,
		"%s",
		"[[:space:]]*<expression>\\(ptr_3456\\) != NULL</expression>"
		);
	UT_ASSERT_NOT_NULL(ptr_3456);
}

static void test_fail_assert_not_null_msg(void)
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"ASSERT",
		"Just1Message",
		"%s",
		"[[:space:]]*<expression>\\(NULL\\) != NULL</expression>"
		);
	UT_ASSERT_NOT_NULL_MSG(NULL, "%s%dMessage", "Just", 1);
}



UT_DESC_TS_BEGIN(test_boolean_suite, suite_init, suite_cleanup, NULL, NULL)
	UT_DESC_TC(test_all_passed)
	UT_DESC_TC(test_all_passed_msg)
	UT_DESC_TC(test_fail_assert)
	UT_DESC_TC(test_fail_assert_msg)
	UT_DESC_TC(test_fail_assert_true)
	UT_DESC_TC(test_fail_assert_true_msg)
	UT_DESC_TC(test_fail_assert_false)
	UT_DESC_TC(test_fail_assert_false_msg)
	UT_DESC_TC(test_fail_assert_null)
	UT_DESC_TC(test_fail_assert_null_msg)
	UT_DESC_TC(test_fail_assert_not_null)
	UT_DESC_TC(test_fail_assert_not_null_msg)
UT_DESC_TS_END();
