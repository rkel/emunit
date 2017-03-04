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

#define UT_ASSERT(exp)            EMUNIT_CALL_ASSERT(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR(#exp), (exp)))
#define UT_ASSERT_TRUE(val)       UT_ASSERT_EQUAL_BOOL(true,  (val))
#define UT_ASSERT_FALSE(val)      UT_ASSERT_EQUAL_BOOL(false, (val))
#define UT_ASSERT_NULL(ptr)       EMUNIT_CN2(UT_ASSERT_EQUAL_HEX, EMUNIT_PTR_SIZE)((emunit_uintptr_t)NULL, (emunit_uintptr_t)(ptr))
#define UT_ASSERT_NOT_NULL(ptr)   EMUNIT_CALL_ASSERT(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR("(" #ptr ") != NULL"), ((ptr) != NULL)))
/** @} */

/**
 * @name Boolean assertions with messages
 *
 * @{
 */
#define UT_ASSERT_MSG(exp, ...)   EMUNIT_CALL_ASSERT_MSG(EMUNIT_NUMTYPE_BOOL, ut_assert, (EMUNIT_FLASHSTR(#exp), (exp)), __VA_ARGS__)
#define UT_ASSERT_TRUE_MSG(val, ...)
#define UT_ASSERT_FALSE_MSG(val, ...)
#define UT_ASSERT_NULL_MSG(ptr, ...)
#define UT_ASSERT_NOT_NULL_MSG(ptr, ...)
/** @} */

/**
 * @brief Default numeric assertions mapping
 *
 *
 * @{
 */
/**
 * @def UT_ASSERT_EQUAL(e, a)
 * @brief Default integer assertion
 *
 * This macro maps directly to @ref UT_ASSERT_EQUAL_INT
 *
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_EQUAL_INT(e, a)
 * @brief Default signed integer assertion
 *
 * Macro that checks signed integer value with maximum allowed size.
 *
 * @sa EMUNIT_CONF_NUMBER_SIZE
 *
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_EQUAL_UINT(e, a)
 * @brief Default unsigned integer assertion
 *
 * Macro that checks unsigned integer value with maximum allowed size.
 *
 * @sa EMUNIT_CONF_NUMBER_SIZE
 *
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_EQUAL_UINT(e, a)
 * @brief Default unsigned integer assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_EQUAL_UINT
 */

#define UT_ASSERT_EQUAL(e, a) UT_ASSERT_EQUAL_INT(e, a)

#define UT_ASSERT_EQUAL_INT( e, a) EMUNIT_CN2(UT_ASSERT_EQUAL_INT,  EMUNIT_CONF_NUMBER_SIZE)(e, a)
#define UT_ASSERT_EQUAL_UINT(e, a) EMUNIT_CN2(UT_ASSERT_EQUAL_UINT, EMUNIT_CONF_NUMBER_SIZE)(e, a)
#define UT_ASSERT_EQUAL_HEX( e, a) EMUNIT_CN2(UT_ASSERT_EQUAL_HEX,  EMUNIT_CONF_NUMBER_SIZE)(e, a)

/** @} */

/**
 * @name Numeric assertions
 *
 * Basic numeric assertions in a form:
 * @code
 * UT_ASSERT_<type><size>(expected, actual);
 * @endcode
 * @{
 */

/**
 * @brief Base equal assertion
 *
 * This assertion is used to build any other type of equal assertions.
 * @param su       Signed or unsigned type (place s or u here).
 * @param var_t    Variable type - this type of temporary variable would be
 *                 created and would be initialised by input value.
 *                 No casting is used to get compiler warning if data loss
 *                 is possible.
 * @param nt       Numeric type to be set
 * @param expected Expected value
 * @param actual   Actual   value
 */
#define UT_ASSERT_EQUAL_x(su, var_t, nt, expected, actual)          \
	do{                                                             \
		var_t UT_ASSERT_EQUAL_x_expected = (expected);              \
		var_t UT_ASSERT_EQUAL_x_actual   = (actual);                \
		EMUNIT_CALL_ASSERT(nt, ut_assert_equal,                     \
			(                                                       \
				(emunit_num_t){.su = (UT_ASSERT_EQUAL_x_expected)}, \
				(emunit_num_t){.su = (UT_ASSERT_EQUAL_x_actual)}    \
			)                                                       \
		);                                                          \
	}while(0)

/**
 * @def UT_ASSERT_EQUAL_BOOL(e, a)
 * @brief Boolean assertion
 *
 * Macro checks if two selected values have the same value.
 *
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_EQUAL_INT8(e, a)
 * @brief Signed 8-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_BOOL
 */
/**
 * @def UT_ASSERT_EQUAL_INT16(e, a)
 * @brief Signed 16-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_BOOL
 */
/**
 * @def UT_ASSERT_EQUAL_INT32(e, a)
 * @brief Signed 32-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_BOOL
 *
 *  @note This assertion may be disabled.
 *        See @ref EMUNIT_CONF_NUMBER_SIZE for details.
 */
/**
 * @def UT_ASSERT_EQUAL_INT64(e, a)
 * @brief Signed 64-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_INT32
 */

/**
 * @def UT_ASSERT_EQUAL_UINT8(e, a)
 * @brief Unsigned 8-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_INT8
 */
/**
 * @def UT_ASSERT_EQUAL_UINT16(e, a)
 * @brief Unsigned 16-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_INT16
 */
/**
 * @def UT_ASSERT_EQUAL_UINT32(e, a)
 * @brief Unsigned 32-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_INT32
 */
/**
 * @def UT_ASSERT_EQUAL_UINT64(e, a)
 * @brief Unsigned 64-bit assertion
 *
 * @copydetails UT_ASSERT_EQUAL_INT64
 */

/**
 * @def UT_ASSERT_EQUAL_HEX8(e, a)
 * @brief Unsigned 8-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_EQUAL_INT8
 */
/**
 * @def UT_ASSERT_EQUAL_HEX16(e, a)
 * @brief Unsigned 16-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_EQUAL_INT16
 */
/**
 * @def UT_ASSERT_EQUAL_HEX32(e, a)
 * @brief Unsigned 32-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_EQUAL_INT32
 */
/**
 * @def UT_ASSERT_EQUAL_HEX64(e, a)
 * @brief Unsigned 64-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_EQUAL_INT64
 */

#define UT_ASSERT_EQUAL_BOOL(  e, a)  UT_ASSERT_EQUAL_x(u, bool,     EMUNIT_NUMTYPE_BOOL,(e), (a))
#define UT_ASSERT_EQUAL_INT8(  e, a)  UT_ASSERT_EQUAL_x(s, int8_t,   EMUNIT_NUMTYPE_S8,  (e), (a))
#define UT_ASSERT_EQUAL_INT16( e, a)  UT_ASSERT_EQUAL_x(s, int16_t,  EMUNIT_NUMTYPE_S16, (e), (a))
#define UT_ASSERT_EQUAL_INT32( e, a)  UT_ASSERT_EQUAL_x(s, int32_t,  EMUNIT_NUMTYPE_S32, (e), (a))
#define UT_ASSERT_EQUAL_INT64( e, a)  UT_ASSERT_EQUAL_x(s, int64_t,  EMUNIT_NUMTYPE_S64, (e), (a))
#define UT_ASSERT_EQUAL_UINT8( e, a)  UT_ASSERT_EQUAL_x(u, uint8_t,  EMUNIT_NUMTYPE_U8,  (e), (a))
#define UT_ASSERT_EQUAL_UINT16(e, a)  UT_ASSERT_EQUAL_x(u, uint16_t, EMUNIT_NUMTYPE_U16, (e), (a))
#define UT_ASSERT_EQUAL_UINT32(e, a)  UT_ASSERT_EQUAL_x(u, uint32_t, EMUNIT_NUMTYPE_U32, (e), (a))
#define UT_ASSERT_EQUAL_UINT64(e, a)  UT_ASSERT_EQUAL_x(u, uint64_t, EMUNIT_NUMTYPE_U64, (e), (a))
#define UT_ASSERT_EQUAL_HEX8(  e, a)  UT_ASSERT_EQUAL_x(u, uint8_t,  EMUNIT_NUMTYPE_X8,  (e), (a))
#define UT_ASSERT_EQUAL_HEX16( e, a)  UT_ASSERT_EQUAL_x(u, uint16_t, EMUNIT_NUMTYPE_X16, (e), (a))
#define UT_ASSERT_EQUAL_HEX32( e, a)  UT_ASSERT_EQUAL_x(u, uint32_t, EMUNIT_NUMTYPE_X32, (e), (a))
#define UT_ASSERT_EQUAL_HEX64( e, a)  UT_ASSERT_EQUAL_x(u, uint64_t, EMUNIT_NUMTYPE_X64, (e), (a))
/** @} */


/**
 * @name String assertions
 *
 * @{
 */

/**
 * @brief
 */
#define UT_ASSERT_EQUAL_STR(   e, a)  EMUNIT_CALL_ASSERT(EMUNIT_NUMTYPE_STR, ut_assert_str, (e, a))
/** @} */

/** @} */
#endif /* EMUNIT_ASSERTIONS_H_INCLUDED */
