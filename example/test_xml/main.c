#include <emunit.h>
#include <emunit_port.h>



static void test1(void)
{
	const char pattern[] =
		"^[[:space:]]*<testcase name=\"test1\">[[:space:]]*</testcase>[[:space:]]*$";
	emunit_pctest_expected_set(pattern);
}






UT_DESC_TS_BEGIN(base_tests, NULL, NULL, NULL)
	UT_DESC_TC(test1)
UT_DESC_TS_END();



UT_MAIN_TS_BEGIN()
	UT_MAIN_TS_ENTRY(base_tests)
UT_MAIN_TS_END();
