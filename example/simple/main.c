#include <emunit.h>


void test_first_init(void)
{

}

void test_init(void)
{

}

void test_cleanup(void)
{

}

void test1(void)
{
	UT_ASSERT(true);
	UT_ASSERT_EQUAL(7, 4+3);
	UT_ASSERT_EQUAL(7, 4);
	UT_ASSERT(false);
}

void test2(void)
{
	UT_ASSERT_MSG(false, "Test entities: <&> may be failed %u times", 10u);
}

#define SOME_NON_ZERO_VALUE 13

void test3(void)
{
	UT_ASSERT_MSG(SOME_NON_ZERO_VALUE == 0, "Simple message");
}

void test_str(void)
{
	char some_buffer[] = "Test string 2";
	UT_ASSERT_EQUAL_STR(EMUNIT_FLASHSTR("Test string"), some_buffer);
}

void test_long_string(void)
{
	char some_buffer[] = "1234567890abcdefghijklmnoprstuwvxyz";
	UT_ASSERT_EQUAL_STR(EMUNIT_FLASHSTR("1234567890abcdefghijklmnoprstuwvxyZ"), some_buffer);
}

void test_very_long_string(void)
{
	char some_buffer[] = "1234567890abcdefghijklmnoprstuwvxyzABCDEFGHIJKLMNOPRSTUWVXYZ";
	UT_ASSERT_EQUAL_STR(
		EMUNIT_FLASHSTR("1234567890abcdefghijklmnoprstuwvxyzABCDEfGHIJKLMNOPRSTUWVXYZ"),
		some_buffer);
}

void test_very_bad_string(void)
{
	char some_buffer[] = "1]]>2]]>3]]>4]]>5]]>6]]>7]]>8]]>9]]>0]]>";
	UT_ASSERT_EQUAL_STR(
		EMUNIT_FLASHSTR("Totally different string"),
		some_buffer);
}


void test_all_passed(void)
{
	static const char some_buffer[] = "Test string that should pass";
	UT_ASSERT_EQUAL_STR(EMUNIT_FLASHSTR("Test string that should pass"), some_buffer);
}

void test_delta_passed(void)
{
	UT_ASSERT_DELTA(8, 12, 4);
	UT_ASSERT_DELTA(8, 12, 20);
}

void test_delta_failed(void)
{
	UT_ASSERT_DELTA(8, 300, 309);
}

void test_range_passed(void)
{
	UT_ASSERT_RANGE(100, 200, 150);
	UT_ASSERT_RANGE(100, 200, 100);
	UT_ASSERT_RANGE(100, 200, 200);
	UT_ASSERT_RANGE(200, 100, 0);
	UT_ASSERT_RANGE(200, 100, 6000);
}

void test_range_failed1(void)
{
	UT_ASSERT_RANGE(100, 200, 201);
}

void test_range_failed2(void)
{
	UT_ASSERT_RANGE(200, 100, 150);
}


UT_DESC_TS_BEGIN(my_suite, NULL, NULL, NULL)
	UT_DESC_TC(test1)
	UT_DESC_TC(test2)
	UT_DESC_TC(test3)
	UT_DESC_TC(test_str)
	UT_DESC_TC(test_long_string)
	UT_DESC_TC(test_very_long_string)
	UT_DESC_TC(test_very_bad_string)
	UT_DESC_TC(test_all_passed)
	UT_DESC_TC(test_delta_passed)
	UT_DESC_TC(test_delta_failed)
	UT_DESC_TC(test_range_passed)
	UT_DESC_TC(test_range_failed1)
	UT_DESC_TC(test_range_failed2)
UT_DESC_TS_END();



UT_MAIN_TS_BEGIN()
	UT_MAIN_TS_ENTRY(my_suite)
UT_MAIN_TS_END();

