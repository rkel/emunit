/**
 * @file
 * @brief Boolean assertions xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains main part of the xml output test.
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
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"ASSERT",
		NULL,
		"%s",
		"[[:space:]]*<expression>1 == 2</expression>");

	UT_ASSERT(1 == 2);
}



UT_DESC_TS_BEGIN(test_boolean_suite, suite_init, suite_cleanup, NULL, NULL)
	UT_DESC_TC(test_all_passed)
	UT_DESC_TC(test_all_passed_msg)
	UT_DESC_TC(test_fail_assert)
UT_DESC_TS_END();
