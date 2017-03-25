#ifndef EMUNIT_ASSERTIONS_DELTA_H_INCLUDED
#define EMUNIT_ASSERTIONS_DELTA_H_INCLUDED
/**
 * @file
 * @brief EMUnit header with delta assertions
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * @sa emunit_assertions_delta_group
 */

/**
 * @addtogroup emunit_assertions_int_group
 * @{
 */
/**
 * @brief Base delta assertion
 *
 * This assertion is used to build any other type of delta assertions.
 * @param su       Signed or unsigned type (place s or u here).
 * @param var_t    Variable type - this type of temporary variable would be
 *                 created and would be initialised by input value.
 *                 No casting is used to get compiler warning if data loss
 *                 is possible.
 * @param nt       Numeric type to be set
 * @param delta    Allowed delta from the expected value
 * @param expected Expected value
 * @param actual   Actual value
 *
 * @sa ut_assert_delta
 */
#define UT_ASSERT_DELTA_x(su, var_t, nt, delta, expected, actual) \
	do{                                                           \
		var_t  UT_ASSERT_DELTA_x_expected = (expected);           \
		var_t  UT_ASSERT_DELTA_x_actual   = (actual);             \
		EMUNIT_CALL_ASSERT(nt, ut_assert_delta,                   \
			(                                                     \
				(delta),                                          \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_expected}, \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_actual}    \
			)                                                     \
		);                                                        \
	}while(0)

/**
 * @brief Base delta assertionassertion with message
 *
 * This is message version of @ref UT_ASSERT_DELTA_x.
 * The formated message is processed and displayed if assertion fails.
 *
 * @copydetails UT_ASSERT_DELTA_x
 * @param ...   @param ... Format string followed by the format values.
 *              Standard @c printf format is used.
 */
#define UT_ASSERT_DELTA_MSG_x(su, var_t, nt, delta, expected, actual, ...) \
	do{                                                                    \
		var_t  UT_ASSERT_DELTA_x_expected = (expected);                    \
		var_t  UT_ASSERT_DELTA_x_actual   = (actual);                      \
		EMUNIT_CALL_ASSERT_MSG(nt, ut_assert_delta,                        \
			(                                                              \
				(delta),                                                   \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_expected},          \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_actual}             \
			),                                                             \
			__VA_ARGS__                                                    \
		);                                                                 \
	}while(0)
/** @} <!-- emunit_assertions_int_group --> */

/**
 * @defgroup emunit_assertions_delta_group <emunit_assertions_delta> Assertions with delta
 * @ingroup emunit_assertions_group
 * @{
 *
 * Assertions that compares two values with allowed difference.
 */

/**
 * @defgroup emunit_assertions_delta_def_group <emunit_assertions_delta_def> Default assertions
 * @{
 *
 * Default delta assertions.
 */

/**
 * @def UT_ASSERT_DELTA(d, e, a)
 * @brief Default integer with delta assertion
 *
 * This macro maps directly to @ref UT_ASSERT_DELTA_INT
 *
 * @param[in] d Delta
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_DELTA_INT(d, e, a)
 * @brief Default signed integer with delta assertion
 *
 * Macro that checks signed integer value with maximum allowed size.
 *
 * @sa EMUNIT_CONF_NUMBER_SIZE
 *
 * @param[in] d Delta
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_DELTA_UINT(d, e, a)
 * @brief Default unsigned integer with delta assertion
 *
 * Macro that checks unsigned integer value with maximum allowed size.
 *
 * @sa EMUNIT_CONF_NUMBER_SIZE
 *
 * @param[in] delta
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_DELTA_UINT(e, a)
 * @brief Default unsigned integer assertion with delta - hexadecimal display
 *
 * @copydetails UT_ASSERT_DELTA_UINT
 */

#define UT_ASSERT_DELTA(d, e, a)      UT_ASSERT_DELTA_INT(d, e, a)
#define UT_ASSERT_DELTA_INT( d, e, a) EMUNIT_CN2(UT_ASSERT_DELTA_INT,  EMUNIT_CONF_NUMBER_SIZE)(d, e, a)
#define UT_ASSERT_DELTA_UINT(d, e, a) EMUNIT_CN2(UT_ASSERT_DELTA_UINT, EMUNIT_CONF_NUMBER_SIZE)(d, e, a)
#define UT_ASSERT_DELTA_HEX( d, e, a) EMUNIT_CN2(UT_ASSERT_DELTA_HEX,  EMUNIT_CONF_NUMBER_SIZE)(d, e, a)


/** @todo Documentation */
#define UT_ASSERT_DELTA_MSG(d, e, a, ...)      UT_ASSERT_DELTA_INT(d, e, a)
#define UT_ASSERT_DELTA_INT_MSG( d, e, a, ...) EMUNIT_CN3(UT_ASSERT_DELTA_INT,  EMUNIT_CONF_NUMBER_SIZE, _MSG)(d, e, a, __VA_ARGS__)
#define UT_ASSERT_DELTA_UINT_MSG(d, e, a, ...) EMUNIT_CN3(UT_ASSERT_DELTA_UINT, EMUNIT_CONF_NUMBER_SIZE, _MSG)(d, e, a, __VA_ARGS__)
#define UT_ASSERT_DELTA_HEX_MSG( d, e, a, ...) EMUNIT_CN3(UT_ASSERT_DELTA_HEX,  EMUNIT_CONF_NUMBER_SIZE, _MSG)(d, e, a, __VA_ARGS__)
/** @} <!-- emunit_assertions_delta_def_group --> */

/**
 * @def UT_ASSERT_DELTA_INT8(d, e, a)
 * @brief Signed 8-bit assertion with delta
 *
 * Checks if actual value is between expected +/- delta.
 *
 * @param[in] d Delta
 * @param[in] e Expected value
 * @param[in] a Actual value
 */
/**
 * @def UT_ASSERT_DELTA_INT16(d, e, a)
 * @brief Signed 16-bit assertion with delta
 *
 * @copydetails UT_ASSERT_DELTA_INT8
 */
/**
 * @def UT_ASSERT_DELTA_INT32(d, e, a)
 * @brief Signed 32-bit assertion with delta
 *
 * @copydetails UT_ASSERT_DELTA_BOOL
 *
 *  @note This assertion may be disabled.
 *        See @ref EMUNIT_CONF_NUMBER_SIZE for details.
 */
/**
 * @def UT_ASSERT_DELTA_INT64(d, e, a)
 * @brief Signed 64-bit assertion with delta
 *
 * @copydetails UT_ASSERT_DELTA_INT32
 */

/**
 * @def UT_ASSERT_DELTA_UINT8(d, e, a)
 * @brief Unsigned 8-bit assertion with delta
 *
 * @copydetails UT_ASSERT_DELTA_INT8
 */
/**
 * @def UT_ASSERT_DELTA_UINT16(d, e, a)
 * @brief Unsigned 16-bit assertion with delta
 *
 * @copydetails UT_ASSERT_DELTA_INT16
 */
/**
 * @def UT_ASSERT_DELTA_UINT32(d, e, a)
 * @brief Unsigned 32-bit assertion with delta
 *
 * @copydetails UT_ASSERT_DELTA_INT32
 */
/**
 * @def UT_ASSERT_DELTA_UINT64(d, e, a)
 * @brief Unsigned 64-bit assertion with delta
 *
 * @copydetails UT_ASSERT_DELTA_INT64
 */

/**
 * @def UT_ASSERT_DELTA_HEX8(d, e, a)
 * @brief Unsigned 8-bit assertion with delta - hexadecimal display
 *
 * @copydetails UT_ASSERT_DELTA_INT8
 */
/**
 * @def UT_ASSERT_DELTA_HEX16(d, e, a)
 * @brief Unsigned 16-bit assertion with delta - hexadecimal display
 *
 * @copydetails UT_ASSERT_DELTA_INT16
 */
/**
 * @def UT_ASSERT_DELTA_HEX32(d, e, a)
 * @brief Unsigned 32-bit assertion with delta - hexadecimal display
 *
 * @copydetails UT_ASSERT_DELTA_INT32
 */
/**
 * @def UT_ASSERT_DELTA_HEX64(d, e, a)
 * @brief Unsigned 64-bit assertion with delta - hexadecimal display
 *
 * @copydetails UT_ASSERT_DELTA_INT64
 */

#define UT_ASSERT_DELTA_INT8(  d, e, a)  UT_ASSERT_DELTA_x(s, int8_t,   EMUNIT_NUMTYPE_S8,  (d), (e), (a))
#define UT_ASSERT_DELTA_INT16( d, e, a)  UT_ASSERT_DELTA_x(s, int16_t,  EMUNIT_NUMTYPE_S16, (d), (e), (a))
#define UT_ASSERT_DELTA_INT32( d, e, a)  UT_ASSERT_DELTA_x(s, int32_t,  EMUNIT_NUMTYPE_S32, (d), (e), (a))
#define UT_ASSERT_DELTA_INT64( d, e, a)  UT_ASSERT_DELTA_x(s, int64_t,  EMUNIT_NUMTYPE_S64, (d), (e), (a))
#define UT_ASSERT_DELTA_UINT8( d, e, a)  UT_ASSERT_DELTA_x(u, uint8_t,  EMUNIT_NUMTYPE_U8,  (d), (e), (a))
#define UT_ASSERT_DELTA_UINT16(d, e, a)  UT_ASSERT_DELTA_x(u, uint16_t, EMUNIT_NUMTYPE_U16, (d), (e), (a))
#define UT_ASSERT_DELTA_UINT32(d, e, a)  UT_ASSERT_DELTA_x(u, uint32_t, EMUNIT_NUMTYPE_U32, (d), (e), (a))
#define UT_ASSERT_DELTA_UINT64(d, e, a)  UT_ASSERT_DELTA_x(u, uint64_t, EMUNIT_NUMTYPE_U64, (d), (e), (a))
#define UT_ASSERT_DELTA_HEX8(  d, e, a)  UT_ASSERT_DELTA_x(u, uint8_t,  EMUNIT_NUMTYPE_X8,  (d), (e), (a))
#define UT_ASSERT_DELTA_HEX16( d, e, a)  UT_ASSERT_DELTA_x(u, uint16_t, EMUNIT_NUMTYPE_X16, (d), (e), (a))
#define UT_ASSERT_DELTA_HEX32( d, e, a)  UT_ASSERT_DELTA_x(u, uint32_t, EMUNIT_NUMTYPE_X32, (d), (e), (a))
#define UT_ASSERT_DELTA_HEX64( d, e, a)  UT_ASSERT_DELTA_x(u, uint64_t, EMUNIT_NUMTYPE_X64, (d), (e), (a))

/** @todo Documentation */
#define UT_ASSERT_DELTA_INT8_MSG(  d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(s, int8_t,   EMUNIT_NUMTYPE_S8,  (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_INT16_MSG( d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(s, int16_t,  EMUNIT_NUMTYPE_S16, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_INT32_MSG( d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(s, int32_t,  EMUNIT_NUMTYPE_S32, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_INT64_MSG( d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(s, int64_t,  EMUNIT_NUMTYPE_S64, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_UINT8_MSG( d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint8_t,  EMUNIT_NUMTYPE_U8,  (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_UINT16_MSG(d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint16_t, EMUNIT_NUMTYPE_U16, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_UINT32_MSG(d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint32_t, EMUNIT_NUMTYPE_U32, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_UINT64_MSG(d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint64_t, EMUNIT_NUMTYPE_U64, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_HEX8_MSG(  d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint8_t,  EMUNIT_NUMTYPE_X8,  (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_HEX16_MSG( d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint16_t, EMUNIT_NUMTYPE_X16, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_HEX32_MSG( d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint32_t, EMUNIT_NUMTYPE_X32, (d), (e), (a), __VA_ARGS__)
#define UT_ASSERT_DELTA_HEX64_MSG( d, e, a, ...)  UT_ASSERT_DELTA_MSG_x(u, uint64_t, EMUNIT_NUMTYPE_X64, (d), (e), (a), __VA_ARGS__)

/** @} <!-- emunit_assertions_delta_group --> */
#endif /* EMUNIT_ASSERTIONS_DELTA_H_INCLUDED */
