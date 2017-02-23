#ifndef EMUNIT_TYPES_H_INCLUDED
#define EMUNIT_TYPES_H_INCLUDED
/**
 * @file
 * @brief EMUnit types
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * File with EMUnit variable types.
 * @sa emunit_types_group
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <config_emunit.h>
#include "emunit_macros.h"

/**
 * @defgroup emunit_types_group <emunit_types> EMUnit variable types
 * @{
 * @ingroup emunit_group
 *
 * EMUnit variable types in separate file to help out with cross file inclusion.
 */

/**
 * @brief Number of characters in the translation buffers
 *
 * Numeric representation:
 * - 2^32 = 4 294 967 296 (10 characters + 1 for sign + 1 for NULL)
 * - 2^64 = 18 446 744 073 709 551 616 (20 characters + 1 for sign + 1 for NULL)
 */
#if EMUNIT_CONF_NUMBER_SIZE <= 32
#define EUNIT_NUMBUFF_SIZE 12
#elif EMUNIT_CONF_NUMBER_SIZE <= 64
#define EUNIT_NUMBUFF_SIZE 22
#else
#error Unsupported number size
#endif

/**
 * @brief The biggest unsigned variable type used for comparison
 *
 * This is the type used by default in main comparison functions.
 */
typedef EMUNIT_CN3(uint, EMUNIT_CONF_NUMBER_SIZE, _t) emunit_unum_t;

/**
 * @brief The biggest signed variable type used for comparison
 *
 * This is the type used by default in main comparison functions.
 */
typedef EMUNIT_CN3(int, EMUNIT_CONF_NUMBER_SIZE, _t) emunit_snum_t;

/**
 * @brief Union with signed and unsigned numeric type
 *
 * Union uses with any comparison function.
 */
typedef union
{
	emunit_unum_t u; //!< Unsigned value
	emunit_snum_t s; //!< Signed value
}emunit_num_t;

/**
 * @brief Maximum value for biggest unsigned numeric
 */
#define EMUNIT_UNUM_MAX EMUNIT_CN3(UINT, EMUNIT_CONF_NUMBER_SIZE, _MAX)

/**
 * @brief Minimum value for biggest signed numeric
 */
#define EMUNIT_SNUM_MIN EMUNIT_CN3(INT, EMUNIT_CONF_NUMBER_SIZE, _MIN)

/**
 * @brief Maximum value for biggest signed numeric
 */
#define EMUNIT_SNUM_MAX EMUNIT_CN3(INT, EMUNIT_CONF_NUMBER_SIZE, _MAX)

/**
 * @brief Reset reason
 *
 * The enumerator of possible reset reasons that would be returned by the port.
 * The reset reasons are related into current test unit state, not the real CPU state.
 */
typedef enum
{
	EMUNIT_RR_INIT,    /**< This is first start and the whole test should be initialised. */
	EMUNIT_RR_FINISH,  /**< The summarise and finish whole test */
	EMUNIT_RR_PANIC,   /**< Reset because of some unrecoverable internal assert. Buffer should be filled with the message. */
	EMUNIT_RR_TIMEOUT, /**< Timeout during test - normally WDT would realize reset in the middle of the test. */
	EMUNIT_RR_RUN,     /**< Run currently selected test case. */
}emunit_rr_t;

/**
 * @brief Display types for numeric values
 *
 * Enumerator of the numeric values used during comparison and displaying
 * the values.
 */
typedef enum
{
	EMUNIT_NUMTYPE_BOOL, /**< Boolean true or false values    */

	EMUNIT_NUMTYPE_U8,   /**< Unsigned 8 bits numeric type    */
	EMUNIT_NUMTYPE_S8,   /**< Signed 8 bits numeric type      */
	EMUNIT_NUMTYPE_X8,   /**< Hexadecimal 8 bits numeric type */

	EMUNIT_NUMTYPE_U16,  /**< Unsigned 16 bits numeric type    */
	EMUNIT_NUMTYPE_S16,  /**< Signed 16 bits numeric type      */
	EMUNIT_NUMTYPE_X16,  /**< Hexadecimal 16 bits numeric type */

#if (EMUNIT_CONF_NUMBER_SIZE > 16) || defined(__DOXYGEN__)
	EMUNIT_NUMTYPE_U32,  /**< Unsigned 32 bits numeric type    */
	EMUNIT_NUMTYPE_S32,  /**< Signed 32 bits numeric type      */
	EMUNIT_NUMTYPE_X32,  /**< Hexadecimal 32 bits numeric type */
#endif
#if (EMUNIT_CONF_NUMBER_SIZE > 32) || defined(__DOXYGEN__)
	EMUNIT_NUMTYPE_U64,  /**< Unsigned 32 bits numeric type    */
	EMUNIT_NUMTYPE_S64,  /**< Signed 32 bits numeric type      */
	EMUNIT_NUMTYPE_X64,  /**< Hexadecimal 32 bits numeric type */
#endif
	EMUNIT_NUMTYPE_STR, /**< String pointers */
	EMUNIT_NUMTYPE_MEM, /**< Memory pointers */

	EMUNIT_NUMTYPE_LAST  /**< Last value marker - startpoint for internal types */
}emunit_numtypes_t;

/**
 * @brief numeric type used for pointers
 *
 *
 */
#if UINTPTR_MAX == UINT16_MAX
#define EMUNIT_NUMTYPE_PTR EMUNIT_NUMTYPE_X16
#elif UINTPTR_MAX == UINT32_MAX
#define EMUNIT_NUMTYPE_PTR EMUNIT_NUMTYPE_X32
#elif UINTPTR_MAX == UINT64_MAX
#define EMUNIT_NUMTYPE_PTR EMUNIT_NUMTYPE_X64
#else
#error Unsupported pointer type
#endif

/**
 * @brief Maximal supported signed type
 */
#define EMUNIT_NUMTYPE_INT EMUNIT_CN2(EMUNIT_NUMTYPE_S, EMUNIT_CONF_NUMBER_SIZE)

/**
 * @brief Assertion header
 *
 * This array would be placed inside program memory and would be used to
 * hold all constant values for the assertion function.
 */
typedef struct
{
	char const __flash *p_file;  //!< File name
	unsigned int        line;    //!< Line number
	emunit_numtypes_t   numtype; //!< Numeric type
}emunit_assert_head_t;

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

/**
 * @brief The array assertion header type
 *
 * Type used to describe an array.
 */
typedef struct emunit_assert_array_head_s emunit_assert_array_head_t;


/**
 * @brief Array object comparison function
 *
 * This function should compare two objects.
 * It would be used by the array comparison function and gives it maximum
 * functionality and flexibility.
 *
 * @param[in] p_a Pointer to the first argument for comparison
 * @param[in] p_b Pointer to the second argument for comparison
 *
 * @retval true  Arguments are equal
 * @retval false Arguments are not equal
 */
typedef bool (*emunit_assert_obj_cmp_func)(const __memx void * p_a, const __memx void * p_b);

/**
 * @brief Array object printing function
 *
 * This function should print given object value.
 *
 * @param[in] p_obj  Pointer to the object to be printed
 * @param[in] p_head The array header - some printing functions may use it
 *                   to alternate output printing format (Hex, Dec, Unsigned...).
 */
typedef void (*emunit_assert_obj_print_func)(
		const __memx void *p_obj,
		const __flash emunit_assert_array_head_t * p_head);

/**
 * @brief The structure that describes array prepared for comparison
 *
 * This type is used to describe the array type for checking and printing.
 */
struct emunit_assert_array_head_s
{
	emunit_assert_head_t         head;      //!< Standard assertion header
	emunit_assert_obj_cmp_func   cmp_fnc;   //!< Single item comparing function
	emunit_assert_obj_print_func print_fnc; //!< Single item printing function
	size_t                       item_size; //!< The size of single array item
};

/**
 * @brief The variable part of for comparison
 *
 * Part describing the part of an arrays that can be placed in RAM during comparison.
 * It means that the informations here can be variable.
 */

typedef struct
{
	const __memx void * p_expected; //!< Expected array
	const __memx void * p_actual;   //!< Actual array
	const size_t        item_cnt;   //!< Number of items to compare
}emunit_assert_array_cmp_t;

/**
 * @brief Test function
 *
 * The pointer to any test function.
 * Also used as init and cleanup functions in a suite.
 */
typedef void (*emunit_test_fnc)(void);

/**
 * @brief Test descriptor
 *
 * The descriptor that would be placed into test table.
 * This may describe test or test suite, depending on the @c @p_name.
 */
typedef struct
{
	char __flash const * p_name; //!< The pointer to the name of the test case
	emunit_test_fnc      p_fnc; //!< Test function pointer
}emunit_test_desc_t;

/**
 * @name Positions of header entries in  test suite descriptor
 *
 * Mnemonics for the positions of the elements in test suite header.
 * Test suite header uses a few array entries of the type @ref emunit_test_desc_t.
 *
 * @note
 * Any change here would require update in @ref UT_DESC_TS_BEGIN macro.
 * @{
 */
	#define EMUNIT_TS_IDX_INITFIRST 0 /**< The index of the first initialisation function */
	#define EMUNIT_TS_IDX_INIT      1 /**< The index of the test initialisation function  */
	#define EMUNIT_TS_IDX_CLEANUP   2 /**< The index of the test cleanup function         */

	#define EMUNIT_TS_IDX_NAME      2 /**< The index of the name of the test suite */
	#define EMUNIT_TS_IDX_NULL1     0 /**< The index of the first NULL mark        */
	#define EMUNIT_TS_IDX_NULL2     1 /**< The index of the second NULL mark       */

	#define EMUNIT_TS_IDX_FIRST     3 /**< First index of the real test case in the test suite */
/** @} */


/**
 * @name Macros for creating test suite descriptors
 *
 * Macros used to to create an array with test suite description table.
 * This table(s) would be used to create test suite array.
 *
 * The example how to use them to create single suite:
 * @code
 * UT_DESC_TS_BEGIN(my_suite, NULL, NULL, NULL) // No comma, nor semicolon here
 * 	UT_DESC_TC(test1)
 * 	UT_DESC_TC(test2) // No commas between elements
 * UT_DESC_TS_END();  // Semicolon on the end is mandatory.
 * @endcode
 * @{
 */
	/**
	 * @brief Create variable name for test suite descriptor
	 *
	 * Auxiliary macro that creates the name of the array describing test
	 * with the selected name.
	 *
	 * @param name The name of suite
	 *
	 * @return The name of the array with suite description.
	 */
	#define UT_DESC_TS_VARNAME(name) EMUNIT_CN2(ut_desc_, name)

	/**
	 * @brief Macro for initialising new test suite
	 *
	 * This macro starts new test suite description.
	 * Creates the array with tests and the test suite header.
	 *
	 * @param name           The name of created suite.
	 *                       This name can be used then in @ref UT_TS_ARRAY_ENTRY macro.
	 *                       Also this name would be presented in test results.
	 * @param init_first_fnc The function called only once in all suites when
	 *                       The suite is started (before init of the first test in the suite).
	 *                       Can be NULL.
	 * @param init_fnc       Function to be called before every test in the suite.
	 * @param cleanup_fnc    Function to be called just before finishing the test in the suite.
	 *                       This function would be called also when error in the test is detected.
	 */
	#define UT_DESC_TS_BEGIN(name, init_first_fnc, init_fnc, cleanup_fnc) \
	    emunit_test_desc_t const __flash UT_DESC_TS_VARNAME(name)[] = {   \
	        {NULL, init_first_fnc},                                       \
	        {NULL, init_fnc},                                             \
	        {(const __flash char[]){EMUNIT_STR(name)}, cleanup_fnc},      \


	/**
	 * @brief Macro for closing test suite
	 *
	 * The macro that marks the end of the test suite
	 * and closes the created array.
	 */
	#define UT_DESC_TS_END() \
	        {NULL, NULL}     \
	    }

	/**
	 * @brief Macro for test case description entry
	 *
	 * The macro that creates single test entry in test suite descriptor.
	 *
	 * @param fnc The test function of the @ref emunit_test_fnc type.
	 */
	#define UT_DESC_TC(fnc) \
	    {(const __flash char[]){EMUNIT_STR(fnc)}, fnc},

	/**
	 * @brief Declare the test suite descriptor externally
	 *
	 * @param name The name of suite
	 */
	#define UT_DESC_TS_EXTERN(name) extern emunit_test_desc_t const __flash UT_DESC_TS_VARNAME(name)[]

/** @} */

/**
 * @name Macros for creating main test suites array
 *
 * Only one array of this type can be placed in whole unit test program.
 * This would contain the pointers to all test suite descriptors created by
 * @ref UT_DESC_TS_BEGIN macros.
 *
 * Use like in the code below:
 * @code
 * UT_DESC_TS_EXTERN(my_suite1);
 * UT_DESC_TS_EXTERN(my_suite2);
 *
 * UT_MAIN_TS_BEGIN()
 * 	UT_MAIN_TS_ENTRY(my_suite1)
 * 	UT_MAIN_TS_ENTRY(my_suite2)
 * UT_MAIN_TS_END();
 * @endcode
 * @{
 */
	/**
	 * @brief Begin of the main test suite array
	 *
	 * Create an array with the pointers to all test suites.
	 */
	#define UT_MAIN_TS_BEGIN()                                                \
	    emunit_test_desc_t const __flash * const __flash emunit_main_ts[] = { \

	/**
	 * @brief Close the main test suite array
	 */
	#define UT_MAIN_TS_END() NULL }

	/**
	 * @brief Add test suite entry into main array
	 *
	 * @param ts The name of the test suite
	 */
	#define UT_MAIN_TS_ENTRY(ts_name) (UT_DESC_TS_VARNAME(ts_name)),
/** @} */


/**
 * @brief Valid configuration key
 *
 * Key used to mark the configuration in @ref emunit_status_t as a valid.
 */
#define EMUNIT_STATUS_KEY_VALID   0x55aa

/**
 * @brief Invalid configuration key
 *
 * The value that should be used in port implementation to invalidate
 * the configuration when restart or first initialisation conditions are meet.
 *
 */
#define EMUNIT_STATUS_KEY_INVALID 0

/**
 * @brief The variable type used as a status validity key
 *
 * This variable type is used for to mark status structure as valid or invalid.
 */
typedef uint16_t emunit_status_key_t;

/**
 * @brief Status structure
 *
 * The status information for the whole test.
 * This variable would be placed into section that is not cleared during restart.
 *
 * How tests are organised:
 *
 */
typedef struct
{
	/** @brief The token with the key marking proper initialisation
	 *
	 * The key value set to @ref EMUNIT_STATUS_KEY_VALID after proper configuration.
	 * It can be cleared to @ref EMUNIT_STATUS_KEY_INVALID during port initialisation,
	 * when restart or first-start conditions are meet.
	 */
	emunit_status_key_t key_valid;
	/** @brief Index of the current suite */
	size_t ts_n_current;
	/** @brief Number of test suites failed */
	size_t ts_n_failed;
	/** @brief Number of test suites passed */
	size_t ts_n_passed;
	/** @brief Index of the current test case in the current test suite */
	size_t tc_n_current;
	/** @brief Number of test cases failed */
	size_t tc_n_failed;
	/** @brief Number of test cases passed */
	size_t tc_n_passed;
	/** Flag that marks that any test failed in current suite */
	bool ts_current_failed;
	/** @brief Reset reason internal status */
	emunit_rr_t rr;
}emunit_status_t;

/** @} */
#endif /* EMUNIT_TYPES_H_INCLUDED */