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

