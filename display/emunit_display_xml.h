#ifndef EMUNIT_DISPLAY_XML_H_INCLUDED
#define EMUNIT_DISPLAY_XML_H_INCLUDED
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
 * @brief EMUnit XML presentation module
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 *
 * @sa emunit_display_xml_group
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include "emunit_types.h"

/**
 * @defgroup emunit_display_xml_group <emunit_display_xml> XML presentation module
 * @{
 * @ingroup emunit_display_group
 */

/** See @ref emunit_display_show_panic */
void emunit_display_xml_show_panic(
	const __flash char * str_file,
	const __flash char * str_line);

/** See @ref emunit_display_test_start */
void emunit_display_xml_test_start(void);

/** See @ref emunit_display_test_end */
void emunit_display_xml_test_end(void);

/** See @ref emunit_display_ts_start */
void emunit_display_xml_ts_start(void);

/** See @ref emunit_display_ts_end */
void emunit_display_xml_ts_end(void);

/** See @ref emunit_display_ts_end */
void emunit_display_xml_tc_start(void);

/** See @ref emunit_display_tc_end */
void emunit_display_xml_tc_end(void);


/** See @ref emunit_display_failed_assert documentation */
void emunit_display_xml_failed_assert(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * str_expr);

/** See @ref emunit_display_failed_assert_msg documentation */
void emunit_display_xml_failed_assert_msg(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * str_expr,
	const __flash char * fmt,
	va_list va_args);

/** See @ref emunit_display_failed_equal documentation */
void emunit_display_xml_failed_equal(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t expected,
	emunit_num_t actual);

/** See @ref emunit_display_failed_equal_msg documentation */
void emunit_display_xml_failed_equal_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t expected,
	emunit_num_t actual,
	const __flash char * fmt,
	va_list va_args);

/** See @ref emunit_display_failed_range documentation */
void emunit_display_xml_failed_range(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t min,
	emunit_num_t max,
	emunit_num_t actual);

/** See @ref emunit_display_failed_range_msg documentation */
void emunit_display_xml_failed_range_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t min,
	emunit_num_t max,
	emunit_num_t actual,
	const __flash char * fmt,
	va_list args);

/** See @ref emunit_display_failed_delta documentation */
void emunit_display_xml_failed_delta(
	const __flash emunit_assert_head_t * p_head,
	emunit_unum_t delta,
	emunit_num_t expected,
	emunit_num_t actual);

/** See @ref emunit_display_failed_delta_msg documentation */
void emunit_display_xml_failed_delta_msg(
	const __flash emunit_assert_head_t * p_head,
	emunit_unum_t delta,
	emunit_num_t expected,
	emunit_num_t actual,
	const __flash char * fmt,
	va_list va_args);

/** See @ref emunit_display_failed_str documentation */
void emunit_display_xml_failed_str(
	const __flash emunit_assert_head_t * p_head,
	const __memx char * str_expected,
	const __memx char * str_actual,
	size_t err_pos);

/** See @ref emunit_display_failed_str_msg documentation */
void emunit_display_xml_failed_str_msg(
	const __flash emunit_assert_head_t * p_head,
	const __memx char * str_expected,
	const __memx char * str_actual,
	size_t err_pos,
	const __flash char * fmt,
	va_list va_args);

/** See @ref emunit_display_failed_array documentation */
void emunit_display_xml_failed_array(
	const __flash emunit_assert_array_head_t * p_head,
	const emunit_assert_array_cmp_t            p_cmp,
	size_t                                     err_pos);

/** @} */
#endif /* EMUNIT_DISPLAY_XML_H_INCLUDED */
