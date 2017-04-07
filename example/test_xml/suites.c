/**
 * @file
 * @brief All suites list for xml test
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains the list of all suites for the XML output test.
 * @sa emunit_test_xml
 */
#include <emunit.h>

/* External declaration of the tests */
/* From main */
UT_DESC_TS_EXTERN(base_tests);
UT_DESC_TS_EXTERN(base_tests2);
UT_DESC_TS_EXTERN(base_tests_last);

UT_DESC_TS_EXTERN(test_boolean_suite);
UT_DESC_TS_EXTERN(test_str_suite);
UT_DESC_TS_EXTERN(test_delta_suite);
UT_DESC_TS_EXTERN(test_equal_suite);
UT_DESC_TS_EXTERN(test_range_suite);


/* Whole test description */
UT_MAIN_TS_BEGIN()
	UT_MAIN_TS_ENTRY(base_tests)
	UT_MAIN_TS_ENTRY(base_tests2)

	UT_MAIN_TS_ENTRY(test_boolean_suite)
	UT_MAIN_TS_ENTRY(test_str_suite)
	UT_MAIN_TS_ENTRY(test_delta_suite)
	UT_MAIN_TS_ENTRY(test_equal_suite)
	UT_MAIN_TS_ENTRY(test_range_suite)


	/* This test suite has to be the last one */
	UT_MAIN_TS_ENTRY(base_tests_last)
UT_MAIN_TS_END();

