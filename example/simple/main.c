#include <emunit.h>



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




UT_DESC_TS_BEGIN(my_suite, NULL, NULL, NULL)
	UT_DESC_TC(test1)
	UT_DESC_TC(test2)
	UT_DESC_TC(test3)
UT_DESC_TS_END();



UT_MAIN_TS_BEGIN()
	UT_MAIN_TS_ENTRY(my_suite)
UT_MAIN_TS_END();

