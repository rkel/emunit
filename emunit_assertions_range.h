#ifndef EMUNIT_ASSERTIONS_RANGE_H_INCLUDED
#define EMUNIT_ASSERTIONS_RANGE_H_INCLUDED
/**
 * @file
 * @brief EMUnit header with range assertions
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * @sa emunit_assertions_range_group
 */

/**
 * @addtogroup emunit_assertions_int_group
 * @{
 */
/**
 * @brief Base range assertion
 *
 * This assertion is used to build any other type of range assertions.
 * @param su       Signed or unsigned type (place s or u here).
 * @param var_t    Variable type - this type of temporary variable would be
 *                 created and would be initialised by input value.
 *                 No casting is used to get compiler warning if data loss
 *                 is possible.
 * @param nt       Numeric type to be set
 * @param min      Minimal expected value
 * @param max      Maximal expected value
 * @param actual   Actual value
 *
 * @sa ut_assert_range
 */
#define UT_ASSERT_RANGE_x(su, var_t, nt, min, max, actual)     \
	do{                                                        \
		var_t  UT_ASSERT_DELTA_x_min = (min);                  \
		var_t  UT_ASSERT_DELTA_x_max = (max);                  \
		var_t  UT_ASSERT_DELTA_x_actual   = (actual);          \
		EMUNIT_CALL_ASSERT(nt, ut_assert_range,                \
			(                                                  \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_min},   \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_max},   \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_actual} \
			)                                                  \
		);                                                     \
	}while(0)

/**
 * @brief Base range assertion with message
 *
 * This is message version of @ref UT_ASSERT_RANGE_x.
 * The formated message is processed and displayed if assertion fails.
 *
 * @copydetails UT_ASSERT_RANGE_x
 * @param ...   @param ... Format string followed by the format values.
 *              Standard @c printf format is used.
 */
#define UT_ASSERT_RANGE_MSG_x(su, var_t, nt, min, max, actual, ...) \
	do{                                                             \
		var_t  UT_ASSERT_DELTA_x_min    = (min);                    \
		var_t  UT_ASSERT_DELTA_x_max    = (max);                    \
		var_t  UT_ASSERT_DELTA_x_actual = (actual);                 \
		EMUNIT_CALL_ASSERT_MSG(nt, ut_assert_range,                 \
			(                                                       \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_min},        \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_max},        \
				(emunit_num_t){.su = UT_ASSERT_DELTA_x_actual}      \
			),                                                      \
			__VA_ARGS__                                             \
		);                                                          \
	}while(0)
/** @} <!-- emunit_assertions_int_group --> */

/**
 * @defgroup emunit_assertions_range_group <emunit_assertions_range> Range assertions
 * @ingroup emunit_assertions_group
 * @{
 *
 * Assertions that checks if assertion is in selected range.
 * There are two options:
 * 1. If min <= max test is performed if actual value is between min and max (including min/max values).
 * 2. If min > max test is performed if actual value is outside min and max (but including min and max into valid values).
 *
 * This way following code:
 *
 * @code
 * UT_ASSERT_RANGE_INT8(100, 50, val);
 * @endcode
 *
 * Is valid if @c val is between -128 and 50 or between 100 and 127.
 */

/**
 * @defgroup emunit_assertions_range_def_group <emunit_assertions_range_def> Default assertions
 * @{
 *
 * Default range assertions.
 */

/**
 * @def UT_ASSERT_RANGE(min, max, a)
 * @brief Default integer in range assertion
 *
 * This macro maps directly to @ref UT_ASSERT_RANGE_INT
 *
 * @param[in] min Minimal expected value
 * @param[in] max Maximal expected value
 * @param[in] a   Actual value
 */
/**
 * @def UT_ASSERT_RANGE_INT(min, max, a)
 * @brief Default signed integer in range assertion
 *
 * Macro that checks signed integer value with maximum allowed size.
 *
 * @sa EMUNIT_CONF_NUMBER_SIZE
 *
 * @param[in] min Minimal expected value
 * @param[in] max Maximal expected value
 * @param[in] a   Actual value
 */
/**
 * @def UT_ASSERT_RANGE_UINT(min, max, a)
 * @brief Default unsigned integer in range assertion
 *
 * Macro that checks unsigned integer value with maximum allowed size.
 *
 * @sa EMUNIT_CONF_NUMBER_SIZE
 *
 * @param[in] min Minimal expected value
 * @param[in] max Maximal expected value
 * @param[in] a   Actual value
 */
/**
 * @def UT_ASSERT_RANGE_UINT(min, max a)
 * @brief Default unsigned integer assertion in range - hexadecimal display
 *
 * @copydetails UT_ASSERT_RANGE_UINT
 */

#define UT_ASSERT_RANGE(min, max, a)      UT_ASSERT_RANGE_INT(min, max, a)
#define UT_ASSERT_RANGE_INT( min, max, a) EMUNIT_CN2(UT_ASSERT_RANGE_INT,  EMUNIT_CONF_NUMBER_SIZE)(min, max, a)
#define UT_ASSERT_RANGE_UINT(min, max, a) EMUNIT_CN2(UT_ASSERT_RANGE_UINT, EMUNIT_CONF_NUMBER_SIZE)(min, max, a)
#define UT_ASSERT_RANGE_HEX( min, max, a) EMUNIT_CN2(UT_ASSERT_RANGE_HEX,  EMUNIT_CONF_NUMBER_SIZE)(min, max, a)

/** @todo Documentation */

#define UT_ASSERT_RANGE_MSG(min, max, a, ...)      UT_ASSERT_RANGE_INT_MSG(min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_INT_MSG( min, max, a, ...) EMUNIT_CN3(UT_ASSERT_RANGE_INT,  EMUNIT_CONF_NUMBER_SIZE, _MSG)(min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_UINT_MSG(min, max, a, ...) EMUNIT_CN3(UT_ASSERT_RANGE_UINT, EMUNIT_CONF_NUMBER_SIZE, _MSG)(min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_HEX_MSG( min, max, a, ...) EMUNIT_CN3(UT_ASSERT_RANGE_HEX,  EMUNIT_CONF_NUMBER_SIZE, _MSG)(min, max, a, __VA_ARGS__)

/** @} <!-- emunit_assertions_range_def_group --> */

/**
 * @def UT_ASSERT_RANGE_INT8(min, max, a)
 * @brief Signed 8-bit assertion
 *
 * Checks if actual value is between min and max.
 *
 * @param[in] min Minimal expected value
 * @param[in] max Maximal expected value
 * @param[in] a   Actual value
 */
/**
 * @def UT_ASSERT_RANGE_INT16(min, max, a)
 * @brief Signed 16-bit assertion
 *
 * @copydetails UT_ASSERT_RANGE_INT8
 */
/**
 * @def UT_ASSERT_RANGE_INT32(min, max, a)
 * @brief Signed 32-bit assertion
 *
 * @copydetails UT_ASSERT_RANGE_BOOL
 *
 *  @note This assertion may be disabled.
 *        See @ref EMUNIT_CONF_NUMBER_SIZE for details.
 */
/**
 * @def UT_ASSERT_RANGE_INT64(min, max, a)
 * @brief Signed 64-bit assertion
 *
 * @copydetails UT_ASSERT_RANGE_INT32
 */

/**
 * @def UT_ASSERT_RANGE_UINT8(min, max, a)
 * @brief Unsigned 8-bit assertion
 *
 * @copydetails UT_ASSERT_RANGE_INT8
 */
/**
 * @def UT_ASSERT_RANGE_UINT16(min, max, a)
 * @brief Unsigned 16-bit assertion
 *
 * @copydetails UT_ASSERT_RANGE_INT16
 */
/**
 * @def UT_ASSERT_RANGE_UINT32(min, max, a)
 * @brief Unsigned 32-bit assertion
 *
 * @copydetails UT_ASSERT_RANGE_INT32
 */
/**
 * @def UT_ASSERT_RANGE_UINT64(min, max, a)
 * @brief Unsigned 64-bit assertion
 *
 * @copydetails UT_ASSERT_RANGE_INT64
 */

/**
 * @def UT_ASSERT_RANGE_HEX8(min, max, a)
 * @brief Unsigned 8-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_RANGE_INT8
 */
/**
 * @def UT_ASSERT_RANGE_HEX16(min, max, a)
 * @brief Unsigned 16-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_RANGE_INT16
 */
/**
 * @def UT_ASSERT_RANGE_HEX32(min, max, a)
 * @brief Unsigned 32-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_RANGE_INT32
 */
/**
 * @def UT_ASSERT_RANGE_HEX64(min, max, a)
 * @brief Unsigned 64-bit assertion with hexadecimal display
 *
 * @copydetails UT_ASSERT_RANGE_INT64
 */

#define UT_ASSERT_RANGE_INT8(  min, max, a)  UT_ASSERT_RANGE_x(s, int8_t,   EMUNIT_NUMTYPE_S8,  min, max, a)
#define UT_ASSERT_RANGE_INT16( min, max, a)  UT_ASSERT_RANGE_x(s, int16_t,  EMUNIT_NUMTYPE_S16, min, max, a)
#define UT_ASSERT_RANGE_INT32( min, max, a)  UT_ASSERT_RANGE_x(s, int32_t,  EMUNIT_NUMTYPE_S32, min, max, a)
#define UT_ASSERT_RANGE_INT64( min, max, a)  UT_ASSERT_RANGE_x(s, int64_t,  EMUNIT_NUMTYPE_S64, min, max, a)
#define UT_ASSERT_RANGE_UINT8( min, max, a)  UT_ASSERT_RANGE_x(u, uint8_t,  EMUNIT_NUMTYPE_U8,  min, max, a)
#define UT_ASSERT_RANGE_UINT16(min, max, a)  UT_ASSERT_RANGE_x(u, uint16_t, EMUNIT_NUMTYPE_U16, min, max, a)
#define UT_ASSERT_RANGE_UINT32(min, max, a)  UT_ASSERT_RANGE_x(u, uint32_t, EMUNIT_NUMTYPE_U32, min, max, a)
#define UT_ASSERT_RANGE_UINT64(min, max, a)  UT_ASSERT_RANGE_x(u, uint64_t, EMUNIT_NUMTYPE_U64, min, max, a)
#define UT_ASSERT_RANGE_HEX8(  min, max, a)  UT_ASSERT_RANGE_x(u, uint8_t,  EMUNIT_NUMTYPE_X8,  min, max, a)
#define UT_ASSERT_RANGE_HEX16( min, max, a)  UT_ASSERT_RANGE_x(u, uint16_t, EMUNIT_NUMTYPE_X16, min, max, a)
#define UT_ASSERT_RANGE_HEX32( min, max, a)  UT_ASSERT_RANGE_x(u, uint32_t, EMUNIT_NUMTYPE_X32, min, max, a)
#define UT_ASSERT_RANGE_HEX64( min, max, a)  UT_ASSERT_RANGE_x(u, uint64_t, EMUNIT_NUMTYPE_X64, min, max, a)

/** @todo Documentation */

#define UT_ASSERT_RANGE_INT8_MSG(  min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(s, int8_t,   EMUNIT_NUMTYPE_S8,  min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_INT16_MSG( min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(s, int16_t,  EMUNIT_NUMTYPE_S16, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_INT32_MSG( min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(s, int32_t,  EMUNIT_NUMTYPE_S32, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_INT64_MSG( min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(s, int64_t,  EMUNIT_NUMTYPE_S64, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_UINT8_MSG( min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint8_t,  EMUNIT_NUMTYPE_U8,  min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_UINT16_MSG(min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint16_t, EMUNIT_NUMTYPE_U16, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_UINT32_MSG(min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint32_t, EMUNIT_NUMTYPE_U32, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_UINT64_MSG(min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint64_t, EMUNIT_NUMTYPE_U64, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_HEX8_MSG(  min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint8_t,  EMUNIT_NUMTYPE_X8,  min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_HEX16_MSG( min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint16_t, EMUNIT_NUMTYPE_X16, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_HEX32_MSG( min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint32_t, EMUNIT_NUMTYPE_X32, min, max, a, __VA_ARGS__)
#define UT_ASSERT_RANGE_HEX64_MSG( min, max, a, ...)  UT_ASSERT_RANGE_MSG_x(u, uint64_t, EMUNIT_NUMTYPE_X64, min, max, a, __VA_ARGS__)

/** @} <!-- emunit_assertions_range_group --> */
#endif /* EMUNIT_ASSERTIONS_RANGE_H_INCLUDED */
