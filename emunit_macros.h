#ifndef EMUNIT_MACROS_H_INCLUDED
#define EMUNIT_MACROS_H_INCLUDED
/**
 * @file
 * @brief EMUnit auxiliary macros
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * Auxiliary macros used internally.
 * @sa emunit_macros_group
 */
#include "config_emunit.h"

/**
 * @defgroup emunit_macros_group <emunit_macros> EMUnit auxiliary macros
 * @{
 * @ingroup emunit_group
 *
 * Auxiliary internal macros used mainly for port and display functions mapping.
 */

/**
 * @brief Stringify given parameter - second expansion level
 */
#define _EMUNIT_STR(name) #name

/**
 * @brief Internal macro to stringify given parameter
 *
 * This function would convert given parameter into string.
 * Used for include computation.
 *
 * @param name Parameter for stringification
 *
 * @return The given argument inside quotes.
 */
#define EMUNIT_STR(name) _EMUNIT_STR(name)

/**
 * @brief Just present the arguments
 *
 * Macro does nothing more than just returning the arguments.
 * It is used mainly for removing bracket functionality.
 *
 * @param ... Parameters to be presented
 *
 * @return Just given arguments
 *
 * @sa EMUNIT_DEBRACKET
 */
#define EMUNIT_ARGS(...) __VA_ARGS__

/**
 * @brief Remove arguments from the brackets
 *
 * Given argument has to be placed inside bracket.
 *
 * Example:
 * @code
 * EMUNIT_DEBRACKET((10, 11))  // -> 10, 11
 *
 * #define MY_ARGS   (5, 19)
 * EMUNIT_DEBRACKET(MY_ARGS)   // => 5, 19
 * @endcode
 *
 * @param a Arguments in a bracket that should be extracted
 *
 * @return Given parameter with the bracket removed
 */
#define EMUNIT_DEBRACKET(a) EMUNIT_ARGS a

/**
 * @brief Connect 2 names
 *
 * Macro that connects two given names to single mnemonic.
 * The macro makes sure that all arguments are totally expanded before
 * merging them.
 *
 * @param a1 First part of the mnemonic to create
 * @param a2 Second part of the mnemonic to create
 *
 * @return Mnemonic created by connecting a1 to a2
 *
 * @sa EMUNIT_CN3
 */
#define EMUNIT_CN2(a1, a2) _EMUNIT_CN2(a1, a2)

/**
 * @brief Connect 2 names - second expansion level
 *
 * Second expansion for @ref EMUNIT_CN2.
 *
 * @param a1 See documentation for @ref EMUNIT_CN2
 * @param a2 See documentation for @ref EMUNIT_CN2
 *
 * @return See documentation for @ref EMUNIT_CN2
 */
#define _EMUNIT_CN2(a1, a2) a1 ## a2

/**
 * @brief Connect 3 names
 *
 * Macro that connects three given names to single mnemonic.
 * The macro makes sure that all arguments are totally expanded before
 * merging them.
 *
 * @param a1 First part of the mnemonic to create
 * @param a2 Second part of the mnemonic to create
 * @param a3 Third part of the mnemonic to create
 *
 * @return Mnemonic created by connecting together a1, a2 and a3
 *
 * @sa EMUNIT_CN3
 */
#define EMUNIT_CN3(a1, a2, a3) _EMUNIT_CN3(a1, a2, a3)

/**
 * @brief Connect 3 names - second expansion level
 *
 * Second expansion for @ref EMUNIT_CN3.
 *
 * @param a1 See documentation for @ref EMUNIT_CN3
 * @param a2 See documentation for @ref EMUNIT_CN3
 * @param a3 See documentation for @ref EMUNIT_CN3
 *
 * @return See documentation for @ref EMUNIT_CN3
 */
#define _EMUNIT_CN3(a1, a2, a3) a1 ## a2 ## a3

/**
 * @brief Get first argument
 *
 *  This macro returns its first argument unchanged.
 *  The idea is that this macro may be expanded to get any selected argument.
 *  This is why the name like get_1 was used instead of get_first.
 *  Currently only first argument can be selected.
 *  It seems to be enough in my code.
 *
 *  The construction of this macro makes it sure that argument number starting
 *  from 1 can be processed correctly
 *
 * @param ... Arguments from witch first would be selected
 */
#define EMUNIT_ARG1(...)  _EMUNIT_ARG1(__VA_ARGS__, )

/**
 * @brief Get first argument - second expansion
 *
 * @sa EMUNIT_ARG1
 */
#define _EMUNIT_ARG1(...) __EMUNIT_ARG1(__VA_ARGS__)

/**
 * @brief Get first argument - third expansion
 *
 * @sa EMUNIT_ARG1
 */
#define __EMUNIT_ARG1(a1, ...) a1

/**
 * @brief Get arguments after the first one
 *
 *  This macro returns all arguments after first one.
 *
 * @note Before calling this macro make sure that
 *      there really is more than argument.
 *      See @ref EMUNIT_IF_ARGCNT1
 *
 * @param ... Arguments from witch first would be thrown out
 */
#define EMUNIT_ARG_AFTER1(...) _EMUNIT_ARG_AFTER1(__VA_ARGS__)

/**
 * @brief Get arguments after the first - second expansion
 *
 * @sa EMUNIT_ARG_AFTER1
 */
#define _EMUNIT_ARG_AFTER1(a1, ...) __VA_ARGS__

/**
 * @name Zeros arguments
 *
 * List of 0 values.
 * Used internally.
 * @{
 */

#define EMUNIT_ZEROS1  0                              /**< @brief single zero */
#define EMUNIT_ZEROS2  EMUNIT_ZEROS1,  EMUNIT_ZEROS1  /**< @brief 2 zeros     */
#define EMUNIT_ZEROS4  EMUNIT_ZEROS2,  EMUNIT_ZEROS2  /**< @brief 4 zeros     */
#define EMUNIT_ZEROS8  EMUNIT_ZEROS4,  EMUNIT_ZEROS4  /**< @brief 8 zeros     */
#define EMUNIT_ZEROS16 EMUNIT_ZEROS8,  EMUNIT_ZEROS8  /**< @brief 16 zeros     */
#define EMUNIT_ZEROS32 EMUNIT_ZEROS16, EMUNIT_ZEROS16 /**< @brief 32 zeros     */
#define EMUNIT_ZEROS64 EMUNIT_ZEROS32, EMUNIT_ZEROS32 /**< @brief 64 zeros     */
/** @} */

/**
 * @brief Check if number of arguments is 1
 *
 * This macro checks if number of arguments is 1 or more.
 *
 * @param ... Arguments to count and check
 *
 * @retval 0 literally if number of arguments i 2 and more
 * @retval 1 literally if number of arguments i 0 or 1
 *
 * @note
 * For preprocessor 0 arguments also equals 1 argument
 * @note
 * Character ~ is used as a last parameter for internal checking - it would
 * always create invalid mnemonic when contracting with anything.
 */
#define EMUNIT_ARGCNT1_CHECK(...) _EMUNIT_ARGCNT1_CHECK(__VA_ARGS__, EMUNIT_ZEROS64, 1, ~, ~)

/**
 * @brief Check if number of arguments is 1 - second expansion
 *
 * Here all the single arguments like @ref EMUNIT_ZEROS64 are splitted to
 * the final list of arguments.
 *
 * @sa EMUNIT_ARGCNT1_CHECK
 */
#define _EMUNIT_ARGCNT1_CHECK(...) __EMUNIT_ARGCNT1_CHECK(__VA_ARGS__)

/**
 * @brief Check if number of arguments is 1 - third expansion
 *
 * Now every value is processed as single argument.
 *
 * @sa EMUNIT_ARGCNT1_CHECK
 */
#define __EMUNIT_ARGCNT1_CHECK(a0, \
	_64, _63, _62, _61, _60, _59, _58, _57, _56, _55, _54, _53, _52, _51, _50, _49, \
	_48, _47, _46, _45, _44, _43, _42, _41, _40, _39, _38, _37, _36, _35, _34, _33, \
	_32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, \
	_16, _15, _14, _13, _12, _11, _10,  _9,  _8,  _7,  _6,  _5,  _4,  _3,  _2,  _1, \
	N, ...) N

/**
 * @brief Conditional selection macro
 *
 * This macro selects one of the bodies to generate.
 *
 * @param cond Condition, have to be 0 or 1 literally.
 *        Cannot be the result of computation, as preprocessor understands
 *        text only.
 * @param if_body The body to generate if condition is 1
 * @param el_body The body to generate if condition is 0
 */
#define EMUNIT_IF_SELECT(cond, if_body, el_body) \
	_EMUNIT_IF_SELECT(cond, if_body, el_body)

/**
 * @brief Conditional selection macro - second expansion
 *
 * The place where @c cond is expanded.
 *
 * @sa EMUNIT_IF_SELECT
 */
#define _EMUNIT_IF_SELECT(cond, if_body, el_body) \
	_EMUNIT_IF_SELECT_ ## cond(if_body, el_body)

/**
 * @brief Conditional selection macro - if 0
 *
 * Macro selects @c el_body argument
 *
 * @param if_body The argument that is not processed
 * @param el_body This argument would be returned by the macro
 */
#define _EMUNIT_IF_SELECT_0(if_body, el_body) el_body

/**
 * @brief Conditional selection macro - if 1
 *
 * Macro selects @c if_body argument
 *
 * @param if_body This argument would be returned by the macro
 * @param el_body The argument that is not processed
 */
#define _EMUNIT_IF_SELECT_1(if_body, el_body) if_body

/**
 * @brief Macro that expands to the one of the selected bodies
 *
 * @param args     Arguments to count (in a brackets)
 * @param if_body  Body if number of arguments is 1
 * @param el_body  Body if number of arguments is more than 1
 *
 * @return Macro returns if_body or else_body, depending on number of arguments
 *         in @c args parameter.
 */
#define EMUNIT_IF_ARGCNT1(args, if_body, el_body) \
	_EMUNIT_IF_ARGCNT1(args, if_body, el_body)

/**
 * @brief Macro tha expands to the one of the selected bodies - second expansion
 *
 * @sa EMUNIT_IF_ARGCNT1
 */
#define _EMUNIT_IF_ARGCNT1(args, if_body, el_body) \
	EMUNIT_IF_SELECT(EMUNIT_ARGCNT1_CHECK args, if_body, el_body)

/**
 * @brief Minimal value
 *
 * Macro selecting mnimal value
 *
 * @param[in] a First value to select
 * @param[in] b Second value to select
 *
 * @return Lower of two input values
 * @sa EMUNIT_MIN
 *
 * @attention This macro is not secured against arguments that modifies its value.
 * For example following code:
 * @code
 * EMUNIT_MIN(i++, j++)
 * @endcode
 * may give unexpected result.
 */
#define EMUNIT_MIN(a, b) ( (a) < (b) ? (a) : (b) )

/**
 * @brief Maximal value
 *
 * Macro selecting maximal value
 *
 * @param[in] a First value to select
 * @param[in] b Second value to select
 *
 * @return Higher of two input values
 * @sa EMUNIT_MIN
 */
#define EMUNIT_MAX(a, b) ( (a) > (b) ? (a) : (b) )

/**
 * @brief Universal static assertion
 *
 * This idea comes from the following page:
 * http://www.pixelbeat.org/programming/gcc/static_assert.html
 */
/* These can't be used after statements in c89. */
#ifdef __COUNTER__
  #define EMUNIT_STATIC_ASSERT(e,m) \
    ;enum { EMUNIT_CN2(emunit_static_assert_, __COUNTER__) = 1/(!!(e)) }
#else
  /* This can't be used twice on the same line so ensure if using in headers
   * that the headers are not included twice (by wrapping in #ifndef...#endif)
   * Note it doesn't cause an issue when used on same line of separate modules
   * compiled with gcc -combine -fwhole-program.  */
  #define EMUNIT_STATIC_ASSERT(e,m) \
    ;enum { EMUNIT_CN2(emunit_assert_line_, __LINE__) = 1/(!!(e)) }
#endif

/**
 * @name  Macros used for EMUnit porting.
 *
 * Normally all the functions in port files could be named the same and just selected
 * by selecting include and source file path.
 * This simple solution would generate ugly documentation.
 * So all port macros and files are named with the port macro name inside
 * and this macros are used to select the right one.
 * @{
 */
/*
 * @brief Port source selector
 *
 * The macro used in preprocessor inclusions to select
 * port file from the right port folder.
 *
 * @param file_name The part of the file name just after "emunit_port_simulavr".
 *                  For example for emunit_port_simulavr.h leave it empty and set h in @c file_ext.
 * @param file_ext  The extension of the requested file
 *                  The name without quotation.
 *
 * @return The name of the port file.
 */
#define EMUNIT_PORT_FILE(file_name, file_ext) \
	EMUNIT_STR(port/EMUNIT_CONF_PORT/EMUNIT_CN3(emunit_port_, EMUNIT_CONF_PORT, file_name).file_ext)

/**
 * @brief Create port base name
 *
 * Macro that adds the constant prefix into the name given.
 * This way the base port object name is created.
 *
 * @param name The name of element to be created (function, variable, type).
 *
 * @return The name in a form \<base_port_prefix\>_\<name\>.
 *
 * @sa Used by @ref EMUNIT_PORT_MAP_VFUNC and @ref EMUNIT_PORT_MAP_TFUNC
 */
#define EMUNIT_PORT_BASE_NAME(name) EMUNIT_CN2(emunit_port_, name)

/**
 * @brief Create port object name
 *
 * Macro that adds the selected port prefix into the name given.
 * This way the port object name is created.
 *
 * @param name The name of element to be created (function, variable, type).
 *
 * @return The name in a form \<port_prefix\>_\<name\>
 *
 * @sa EMUNIT_PORT_BASE_NAME
 */
#define EMUNIT_PORT_NAME(name) \
	EMUNIT_PORT_BASE_NAME(EMUNIT_CN3(EMUNIT_CONF_PORT, _, name))

/**
 * @brief Create port void function map
 *
 * Macro that creates static inline function and calls the selected port
 * function from inside.
 * This is void version, for the functions that does not return any value.
 *
 * @param name      The name of the function
 * @param args      Arguments for the function prototype, in brackets.
 * @param call_args Calling arguments in brackets.
 *
 * @return The whole static function definition that calls selected port function.
 */
#define EMUNIT_PORT_MAP_VFUNC(name, args, call_args)      \
    static inline void EMUNIT_PORT_BASE_NAME(name) args { \
        EMUNIT_PORT_NAME(name) call_args;                 \
    }

/**
 * @brief Create port type function map
 *
 * Macro that creates static inline function and calls the selected port
 * function from inside.
 * This is typed version, for the functions that does return some value.
 *
 * @param type      The type of the function
 * @param name      The name of the function
 * @param args      See @ref EMUNIT_PORT_MAP_VFUNC documentation for details.
 * @param call_args See @ref EMUNIT_PORT_MAP_VFUNC documentation for details.
 *
 * @return The whole static function definition that calls selected port function.
 */
#define EMUNIT_PORT_MAP_TFUNC(type, name, args, call_args) \
    static inline type EMUNIT_PORT_BASE_NAME(name) args {  \
        return EMUNIT_PORT_NAME(name) call_args;           \
    }

/** @} */

/**
 * @name  Macros used for EMUnit display redirecting.
 *
 * Normally all the functions in display files could be named the same and just selected
 * by selecting include and source file path.
 * This simple solution would generate ugly documentation.
 * So all port macros and files are named with the display macro name inside
 * and this macros are used to select the right one.
 * @{
 */
/**
 * @brief Display source selector
 *
 * The macro used in preprocessor inclusions to select
 * port file from the right display folder.
 *
 * @param file_name The part of the file name just after "emunit_display_xml".
 *                  For example for emunit_display_xml.h leave it empty and set h in @c file_ext.
 * @param file_ext  The extension of the requested file
 *                  The name without quotation.
 *
 * @return The name of the display file.
 */
#define EMUNIT_DISPLAY_FILE(file_name, file_ext) \
	EMUNIT_STR(display/EMUNIT_CN3(emunit_display_, EMUNIT_CONF_DISPLAY, file_name).file_ext)

/**
 * @brief Create display base name
 *
 * Macro that adds the constant prefix into the name given.
 * This way the base display object name is created.
 *
 * @param name The name of element to be created (function, variable, type).
 *
 * @return The name in a form \<base_display_prefix\>_\<name\>.
 *
 * @sa Used by @ref EMUNIT_DISPLAY_MAP_VFUNC and @ref EMUNIT_DISPLAY_MAP_TFUNC
 */
#define EMUNIT_DISPLAY_BASE_NAME(name) EMUNIT_CN2(emunit_display_, name)

/**
 * @brief Create display object name
 *
 * Macro that adds the selected display prefix into the name given.
 * This way the display object name is created.
 *
 * @param name The name of element to be created (function, variable, type).
 *
 * @return The name in a form \<display_prefix\>_\<name\>
 *
 * @sa EMUNIT_DISPLAY_BASE_NAME
 */
#define EMUNIT_DISPLAY_NAME(name) \
	EMUNIT_DISPLAY_BASE_NAME(EMUNIT_CN3(EMUNIT_CONF_DISPLAY, _, name))

/**
 * @brief Create display void function map
 *
 * Macro that creates static inline function and calls the selected display
 * function from inside.
 * This is void version, for the functions that does not return any value.
 *
 * @param name      The name of the function
 * @param args      Arguments for the function prototype, in brackets.
 * @param call_args Calling arguments in brackets.
 *
 * @return The whole static function definition that calls selected display
 *         function.
 */
#define EMUNIT_DISPLAY_MAP_VFUNC(name, args, call_args)      \
    static inline void EMUNIT_DISPLAY_BASE_NAME(name) args { \
        EMUNIT_DISPLAY_NAME(name) call_args;                 \
    }

/**
 * @brief Create display type function map
 *
 * Macro that creates static inline function and calls the selected display
 * function from inside.
 * This is typed version, for the functions that does return some value.
 *
 * @param type      The type of the function
 * @param name      The name of the function
 * @param args      See @ref EMUNIT_PORT_MAP_VFUNC documentation for details.
 * @param call_args See @ref EMUNIT_PORT_MAP_VFUNC documentation for details.
 *
 * @return The whole static function definition that calls selected display
 *         function.
 */
#define EMUNIT_DISPLAY_MAP_TFUNC(type, name, args, call_args) \
    static inline type EMUNIT_DISPLAY_BASE_NAME(name) args {  \
        return EMUNIT_DISPLAY_NAME(name) call_args;           \
    }
/**
 * @brief Map assertions display functions
 *
 * This function creates 2 kind of mappings:
 *
 * 1. For the normal function, named @c name.
 * 2. For the _msg function, named @c name @c _msg with additional
 *    arguments related to formated message.
 *
 * @param name      The base name of assertion display function
 * @param args      Arguments for the function prototype, in brackets.
 * @param call_args Calling arguments in brackets.
 *
 * @note Both arguments should be given without the msg type argument.
 *       Message arguments are always added as a 2 final arguments.
 */
#define EMUNIT_DISPLAY_MAP_ASSERT(name, args, call_args)                                     \
	EMUNIT_DISPLAY_MAP_VFUNC(name, args, call_args)                                          \
	EMUNIT_DISPLAY_MAP_VFUNC(                                                                \
		name ## _msg,                                                                        \
		(EMUNIT_DEBRACKET(args), const __flash char * emunit_p_fmt, va_list emunit_va_args), \
		(EMUNIT_DEBRACKET(call_args), emunit_p_fmt, emunit_va_args))
/** @} */

/** @} */
#endif /* EMUNIT_MACROS_H_INCLUDED */
