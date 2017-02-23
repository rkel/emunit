#ifndef EMUNIT_ASSERTIONS_H_INCLUDED
#define EMUNIT_ASSERTIONS_H_INCLUDED
#include "emunit_macros.h"
#include "emunit_types.h"
#include "emunit_port.h"
/**
 * @file
 * @brief EMUnit header
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
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
 * @name Boolean assertions
 *
 * @{
 */

#define UT_ASSERT(exp)            EMUNIT_CALL_ASSERT(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_STR(exp), (exp)))
#define UT_ASSERT_MSG(exp, ...)   EMUNIT_CALL_ASSERT_MSG(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR(EMUNIT_STR(exp)), (exp)), __VA_ARGS__)

#define UT_ASSERT_TRUE(exp)
#define UT_ASSERT_TRUE_MSG(exp, msg)
#define UT_ASSERT_FALSE(exp)
#define UT_ASSERT_FALSE_MSG(exp, msg)

#define UT_ASSERT_NULL(ptr)
#define UT_ASSERT_NULL_MSG(ptr)
#define UT_ASSERT_NOT_NULL(ptr)
#define UT_ASSERT_NOT_NULL_MSG(ptr)
/** @} */


/**
 * @name Numeric assertions
 *
 * @{
 */

/**
 * @brief Base equal assertion
 *
 * This assertion is used to build any other type of equal assertions.
 * @param su       Signed or unsigned type (place s or u here).
 * @param cast_op  Casting operator, that would be placed in the front of
 *                 actual and expected values.
 * @param nt       Numeric type to be set
 * @param expected Expected value
 * @param actual   Actual   value
 */
#define UT_ASSERT_EQUAL_x(su, cast_op, nt, expected, actual) \
	EMUNIT_CALL_ASSERT(nt, ut_assert_equal,                  \
		(                                                    \
			(emunit_num_t){.su = cast_op (expected)},        \
			(emunit_num_t){.su = cast_op (actual)}           \
		)                                                    \
	)

#define UT_ASSERT_EQUAL(expected, actual) UT_ASSERT_EQUAL_x(s, (emunit_snum_t), EMUNIT_NUMTYPE_INT, expected, actual)

#define UT_ASSERT_EQUAL_BOOL(expected, actual) UT_ASSERT_EQUAL_x(u, (bool), EMUNIT_NUMTYPE_BOOL, expected, actual)

#define UT_ASSERT_EQUAL_INT8(expected, actual)  UT_ASSERT_EQUAL_x(s, (int8_t),  EMUNIT_NUMTYPE_S8,  expected, actual)
#define UT_ASSERT_EQUAL_INT16(expected, actual) UT_ASSERT_EQUAL_x(s, (int16_t), EMUNIT_NUMTYPE_S16, expected, actual)
#define UT_ASSERT_EQUAL_INT32(expected, actual) UT_ASSERT_EQUAL_x(s, (int32_t), EMUNIT_NUMTYPE_S32, expected, actual)
#define UT_ASSERT_EQUAL_INT64(expected, actual) UT_ASSERT_EQUAL_x(s, (int64_t), EMUNIT_NUMTYPE_S64, expected, actual)

#define UT_ASSERT_EQUAL_UINT8(expected, actual)  UT_ASSERT_EQUAL_x(u, (uint8_t),  EMUNIT_NUMTYPE_U8,  expected, actual)
#define UT_ASSERT_EQUAL_UINT16(expected, actual) UT_ASSERT_EQUAL_x(u, (uint16_t), EMUNIT_NUMTYPE_U16, expected, actual)
#define UT_ASSERT_EQUAL_UINT32(expected, actual) UT_ASSERT_EQUAL_x(u, (uint32_t), EMUNIT_NUMTYPE_U32, expected, actual)
#define UT_ASSERT_EQUAL_UINT64(expected, actual) UT_ASSERT_EQUAL_x(u, (uint64_t), EMUNIT_NUMTYPE_U64, expected, actual)

#define UT_ASSERT_EQUAL_HEX8(expected, actual)  UT_ASSERT_EQUAL_x(u, (uint8_t),  EMUNIT_NUMTYPE_X8,  expected, actual)
#define UT_ASSERT_EQUAL_HEX16(expected, actual) UT_ASSERT_EQUAL_x(u, (uint16_t), EMUNIT_NUMTYPE_X16, expected, actual)
#define UT_ASSERT_EQUAL_HEX32(expected, actual) UT_ASSERT_EQUAL_x(u, (uint32_t), EMUNIT_NUMTYPE_X32, expected, actual)
#define UT_ASSERT_EQUAL_HEX64(expected, actual) UT_ASSERT_EQUAL_x(u, (uint64_t), EMUNIT_NUMTYPE_X64, expected, actual)
/** @} */


/** @} */
#endif /* EMUNIT_ASSERTIONS_H_INCLUDED */
