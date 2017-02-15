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
#include <config_emunit.h>

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
 * @brief Declare variable not to be cleared at program start
 *
 * Variable created by this macro have to retain its value during system restart.
 * Variable of such type cannot have any initialisation value.
 *
 * @note
 * Currently only GCC implementation using ".noinit" section.
 */
#define EMUNIT_NOINIT_VAR(type, name) \
	type name __attribute__((section(".noinit")))

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
/**
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
/** @} */

/** @} */
#endif /* EMUNIT_MACROS_H_INCLUDED */
