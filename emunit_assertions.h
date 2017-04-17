#ifndef EMUNIT_ASSERTIONS_H_INCLUDED
#define EMUNIT_ASSERTIONS_H_INCLUDED
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
#include "emunit_macros.h"
#include "emunit_types.h"
#include "emunit_port.h"

#include "emunit_assertions_delta.h"
#include "emunit_assertions_equal.h"
#include "emunit_assertions_range.h"

/**
 * @file
 * @brief EMUnit header with assertions
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * @sa emunit_assertions_group
 */

/**
 * @defgroup emunit_assertions_group <emunit_assertions> EMUnit defined assertions
 *
 * Predefined assertion macros.
 * This macros should be used directly in tests.
 * @{
 */

/**
 * @defgroup emunit_assertions_int_group <emunit_assertions_int> Internal assertion macros
 *
 * Internal macros that should not be used directly in the tests.
 * This macros are used to construct usable assertion macros.
 * @{
 */

/**
 * @brief The auxiliary macro to call assertion function
 *
 * This macro creates assertion header, and passes it as a first
 * argument to called assertion function.
 *
 * @param[in] nt      Numeric type
 * @param[in] func    Assertion function
 * @param[in] params  Arguments for the assertion function in brackets
 */
#define EMUNIT_CALL_ASSERT(nt, func, params)                          \
	do{                                                               \
		static const __flash char emunit_ca_file[] = __FILE__;        \
		static const __flash emunit_assert_head_t                     \
			emunit_ca_head = {                                        \
				.p_file = emunit_ca_file,                             \
				.line   = __LINE__,                                   \
				.numtype = nt                                         \
			};                                                        \
		func(&emunit_ca_head, EMUNIT_DEBRACKET(params));              \
	}while(0)

/**
 * @brief The auxiliary macro to call assertion function with message
 *
 * This macro creates assertion header, and passes it as a first
 * argument to called assertion function.
 *
 * @param[in] nt      Numeric type
 * @param[in] func    Assertion function
 * @param[in] params  Arguments for the assertion function in brackets
 * @param[in] fmt
 * @param[in] ...     Message format string followed by message parameters
 */
#define EMUNIT_CALL_ASSERT_MSG(nt, func, params, ...)                         \
	do{                                                                       \
		static const __flash char emunit_ca_file[] = __FILE__;                \
		static const __flash char emunit_ca_msg[] = EMUNIT_ARG1(__VA_ARGS__); \
		static const __flash emunit_assert_head_t                             \
			emunit_ca_head = {                                                \
				.p_file = emunit_ca_file,                                     \
				.line   = __LINE__,                                           \
				.numtype = nt                                                 \
			};                                                                \
		EMUNIT_IF_ARGCNT1((__VA_ARGS__),                                      \
			func ## _msg(                                                     \
				&emunit_ca_head,                                              \
				EMUNIT_DEBRACKET(params),                                     \
				emunit_ca_msg                                                 \
			)                                                                 \
			,                                                                 \
			func ## _msg(                                                     \
				&emunit_ca_head,                                              \
				EMUNIT_DEBRACKET(params),                                     \
				emunit_ca_msg,                                                \
				EMUNIT_ARG_AFTER1(__VA_ARGS__)                                \
			)                                                                 \
		);                                                                    \
	}while(0)

/** @} <!-- emunit_assertions_int_group --> */


/**
 * @defgroup emunit_assertions_bool_group <emunit_assertions_bool> Boolean assertions
 * @{
 *
 * Base assertions that operate on true and false values.
 */

/**
 * @brief Base assertion
 *
 * This assertions expects the result of expression to be true.
 * In the case of the error it usually prints the informations about the expression itself
 * (but this may depend on the display port).
 *
 * @param exp The expression
 */
#define UT_ASSERT(         exp)  EMUNIT_CALL_ASSERT(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR(#exp), (exp)))
/**
 * @brief Expect true
 *
 * Functionally very similar to the @ref UT_ASSERT.
 * But its printing behaviour is designed to be a candy for UT_ASSERT_EQUAL_BOOL,
 * so it does not print the infromation about the expression but rather something like:
 *
   @verbatim
   Expected: TRUE
   Actual:   FALSE
   @endverbatim
 *
 * @param val The value to be checked
 */
#define UT_ASSERT_TRUE(    val)  UT_ASSERT_EQUAL_BOOL(true,  (val))
/**
 * @brief Expect false
 *
 * Assertion that expects the value to be false.
 * Technically it is a candy for @ref UT_ASSERT_EQUAL_BOOL.
 *
 * @param val The value to be checked
 */
#define UT_ASSERT_FALSE(   val)  UT_ASSERT_EQUAL_BOOL(false, (val))
/**
 * @brief Expect the pointer to be NULL
 *
 * Assertion that expects the pointer to be NULL.
 * It is constructed that way that if it fails, the actual value of the pointer
 * is displayed.
 *
 * @param ptr The pointer to be checked
 */
#define UT_ASSERT_NULL(    ptr)  EMUNIT_CN2(UT_ASSERT_EQUAL_HEX, EMUNIT_PTR_SIZE)((emunit_uintptr_t)NULL, (emunit_uintptr_t)(ptr))
/**
 * @brief Expect the pointer not to be NULL
 *
 * Assertion that expects the pointer not to be NULL.
 * If it fails it does not have to display the actual pointer value - it is obviously NULL.
 *
 * @param ptr The pointer to be checked
 */
#define UT_ASSERT_NOT_NULL(ptr)  EMUNIT_CALL_ASSERT(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR("(" #ptr ") != NULL"), ((ptr) != NULL)))

/**
 * @brief Base assertion with message
 *
 * The message version of @ref UT_ASSERT.
 *
 * @param exp Expression to be checked
 * @param ... Format string followed by the format values.
 *            Standard @c printf format is used.
 */
#define UT_ASSERT_MSG(         exp, ...)  EMUNIT_CALL_ASSERT_MSG(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR(#exp), (exp)), __VA_ARGS__)
/**
 * @brief Expect true with message
 *
 * The message version of @ref UT_ASSERT_TRUE.
 *
 * @param val The value to be checked
 * @param ... Format string followed by the format values.
 *            Standard @c printf format is used.
 */
#define UT_ASSERT_TRUE_MSG(    val, ...)  UT_ASSERT_EQUAL_BOOL_MSG(true,  (val), __VA_ARGS__)
/**
 * @brief Expect false with message
 *
 * The message version of @ref UT_ASSERT_FALSE.
 *
 * @param val The value to be checked
 * @param ... Format string followed by the format values.
 *            Standard @c printf format is used.
 */
#define UT_ASSERT_FALSE_MSG(   val, ...)  UT_ASSERT_EQUAL_BOOL_MSG(false,  (val), __VA_ARGS__)
/**
 * @brief Expect the pointer to be NULL with message
 *
 * The message version of @ref UT_ASSERT_NULL.
 *
 * @param ptr The pointer to be checked
 * @param ... Format string followed by the format values.
 *            Standard @c printf format is used.
 */
#define UT_ASSERT_NULL_MSG(    ptr, ...)  EMUNIT_CN3(UT_ASSERT_EQUAL_HEX, EMUNIT_PTR_SIZE, _MSG)((emunit_uintptr_t)NULL, (emunit_uintptr_t)(ptr), __VA_ARGS__)
/**
 * @brief Expect the pointer not to be NULL with message
 *
 * The message version of @ref UT_ASSERT_NOT_NULL.
 *
 * @param ptr The pointer to be checked
 * @param ... Format string followed by the format values.
 *            Standard @c printf format is used.
 */
#define UT_ASSERT_NOT_NULL_MSG(ptr, ...)  EMUNIT_CALL_ASSERT_MSG(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR("(" #ptr ") != NULL"), ((ptr) != NULL)), __VA_ARGS__)
/** @} <!-- emunit_assertions_bool_group --> */

/**
 * @defgroup emunit_assertions_str_group <emunit_assertions_str> String assertions
 * @{
 *
 * Assertions that operates on strings.
 */

/**
 * @brief Expect two strings to be the same
 *
 * This function checks the value of the strings.
 * Strings may be placed in any memory (RAM, FLASH).
 *
 * @param e Expected string
 * @param a Actual string
 */
#define UT_ASSERT_EQUAL_STR(    e, a)  EMUNIT_CALL_ASSERT(EMUNIT_NUMTYPE_STR, ut_assert_str, (e, a))

/**
 * @brief Expect two strings to be the same with message
 *
 * The message version of @ref UT_ASSERT_EQUAL_STR.
 *
 * @param e   Expected string
 * @param a   Actual string
 * @param ... Format string followed by the format values.
 *            Standard @c printf format is used.
 */
#define UT_ASSERT_EQUAL_STR_MSG(e, a, ...) EMUNIT_CALL_ASSERT_MSG(EMUNIT_NUMTYPE_STR, ut_assert_str, (e, a), __VA_ARGS__)
/** @} <!-- emunit_assertions_str_group --> */

/** @} <!-- emunit_assertions_group --> */
#endif /* EMUNIT_ASSERTIONS_H_INCLUDED */
