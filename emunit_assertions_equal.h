#ifndef EMUNIT_ASSERTIONS_EQUAL_H_INCLUDED
#define EMUNIT_ASSERTIONS_EQUAL_H_INCLUDED
/**
 * @file
 * @brief EMUnit header with equal assertions
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * @sa emunit_assertions_equal_group
 */

/**
 * @addtogroup emunit_assertions_int_group
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
 * @param actual   Actual value
 *
 * @sa ut_assert_equal
 */
#define UT_ASSERT_EQUAL_x(su, var_t, nt, expected, actual)        \
	do{                                                           \
		var_t UT_ASSERT_EQUAL_x_expected = (expected);            \
		var_t UT_ASSERT_EQUAL_x_actual   = (actual);              \
		EMUNIT_CALL_ASSERT(nt, ut_assert_equal,                   \
			(                                                     \
				(emunit_num_t){.su = UT_ASSERT_EQUAL_x_expected}, \
				(emunit_num_t){.su = UT_ASSERT_EQUAL_x_actual}    \
			)                                                     \
		);                                                        \
	}while(0)
/** @} <!-- emunit_assertions_int_group --> */

/**
 * @defgroup emunit_assertions_equal_group <emunit_assertions_equal> Equality assertions
 * @ingroup emunit_assertions_group
 * @{
 *
 * Assertions that compares two values.
 */

/**
 * @defgroup emunit_assertions_equal_def_group <emunit_assertions_equal_def> Default assertions
 * @{
 *
 * Default equality assertions.
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

#define UT_ASSERT_EQUAL(e, a)      UT_ASSERT_EQUAL_INT(e, a)
#define UT_ASSERT_EQUAL_INT( e, a) EMUNIT_CN2(UT_ASSERT_EQUAL_INT,  EMUNIT_CONF_NUMBER_SIZE)(e, a)
#define UT_ASSERT_EQUAL_UINT(e, a) EMUNIT_CN2(UT_ASSERT_EQUAL_UINT, EMUNIT_CONF_NUMBER_SIZE)(e, a)
#define UT_ASSERT_EQUAL_HEX( e, a) EMUNIT_CN2(UT_ASSERT_EQUAL_HEX,  EMUNIT_CONF_NUMBER_SIZE)(e, a)
/** @} <!-- emunit_assertions_equal_def_group --> */

/**
 * @def UT_ASSERT_EQUAL_BOOL(e, a)
 * @brief Boolean assertion
 *
 * Checks if two selected values have the same value.
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

/** @} <!-- emunit_assertions_equal_group --> */
#endif /* EMUNIT_ASSERTIONS_EQUAL_H_INCLUDED */
