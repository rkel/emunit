#include <emunit.h>



void test1(void)
{
	do{
		static const __flash char emunit_my_file_name[] = "D:\\Dokumenty-firmy\\Termoprodukt\\firmware_2\\firmware\\test\\emunit\\example\\simple\\main.c";
			static const __flash emunit_assert_head_t
				emunit_assert_head_internal = {
		.p_file = emunit_my_file_name,
		.p_msg  = ((void *)0),
		.line   = 7,
		.numtype = EMUNIT_NUMTYPE_BOOL
	};
			ut_assert(&emunit_assert_head_internal, "1", (1));
		}while(0);
	UT_ASSERT(true);
	UT_ASSERT_EQUAL(7, 4+3);
	UT_ASSERT_EQUAL(7, 4);
	UT_ASSERT(false);
}

void test2(void)
{

}

static const __flash emunit_assert_head_t emunit_assert_head_global = {
		.p_file = (const __flash char[]){"D:\\Dokumenty-firmy\\Termoprodukt\\firmware_2\\firmware\\test\\emunit\\example\\simple\\main.c"},
		.p_msg  = ((void *)0),
		.line   = 7,
		.numtype = EMUNIT_NUMTYPE_BOOL};

UT_DESC_TS_BEGIN(my_suite, NULL, NULL, NULL)
	UT_DESC_TC(test1)
	UT_DESC_TC(test2)
UT_DESC_TS_END();



UT_MAIN_TS_BEGIN()
	UT_MAIN_TS_ENTRY(my_suite)
UT_MAIN_TS_END();

