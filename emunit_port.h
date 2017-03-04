#ifndef EMUNIT_PORT_H_INCLUDED
#define EMUNIT_PORT_H_INCLUDED
/**
 * @file
 * @brief EMUnit port declaration code
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * The file where all the port functions are redirected to
 * the selected port functions.
 * @sa emunit_port_group
 */
#include "emunit_macros.h"
#include <stdbool.h>
#include <stdarg.h>
#include EMUNIT_PORT_FILE( , h)

/**
 * @defgroup emunit_port_group <emunit_port> EMUnit port redirection
 * @{
 * @ingroup emunit_group
 *
 * The all the functions in the form emunit_port_<name> are redirected here
 * to the form of emunit_port_\<selected_port\>_name
 */

/**
 * @fn emunit_memcpy
 * @brief Copy data from any kind of supported memory
 *
 * Function that copies the data from any kind of supported memory.
 *
 * @param[out] p_dst Pointer to the target memory in RAM space
 * @param[in]  p_src Pointer to the source memory in any kind of memory space
 * @param[in]  size  The size of the block to copy
 *
 * @return Destination pointer is returned.
 */

/**
 * @fn emunit_strcpy
 * @brief Copy string from any kind of supported memory
 *
 * Function that copies string from any kind of supported memory.
 *
 * @param[out] p_dst Destination string pointer in RAM space.
 * @param[in]  p_src Source string pointer in any kind of memory space.
 * @param[in]  n     Maximum number of characters to copy.
 *
 * @return Destination pointer is returned.
 */

/**
 * @fn emunit_snprintf
 * @brief Standard C printf function
 *
 * Function that takes format string argument from any memory.
 *
 * @param[out] s   Pointer to a buffer where the resulting C-string is stored.
 *                 The buffer should have a size of at least n characters.
 * @param[in]  n   Maximum number of bytes to be used in the buffer.
 *                 The generated string has a length of at most n-1,
 *                 leaving space for the additional terminating null character.
 * @param[in]  fmt Format string.
 * @param[in]  va  Variable argument list.
 *
 * @return Negative on error.
 *         Number of characters written not counting NULL character.
 *         Notice that only when this returned value is non-negative
 *         and less than n, the string has been completely written.
 */

/**
 * @def PRIsPGM
 * String printf from program memory
 */

#if defined(__AVR__) /* Select architecture */
#if defined(__GNUC__) && (__GNUC__ >= 4) /* Select compiler */
#include <string.h>
#include <avr/pgmspace.h>
#include <stdio.h>

static inline void * emunit_memcpy(
	void * p_dst,
	void const __memx * p_src,
	size_t size)
{
	void * ret;
	if(0 > (signed char)__builtin_avr_flash_segment(p_src))
	{
		ret = memcpy(p_dst, p_src, size);
	}
	else
	{
		ret = memcpy_P(p_dst, p_src, size);
	}
	return ret;
}

static inline size_t emunit_strlen(char const __memx * s)
{
	if(0 > (signed char)__builtin_avr_flash_segment(s))
	{
		return strlen(s);
	}
	else
	{
		return strlen_P(s);
	}
}

static inline int emunit_vsnprintf(
	char * s,
	size_t n,
	char const __memx * fmt,
	va_list va)
{
	int ret;

	if(0 > (signed char)__builtin_avr_flash_segment(fmt))
	{
		ret = vsnprintf(s, n, fmt, va);
	}
	else
	{
		ret = vsnprintf_P(s, n, fmt, va);
	}
	return ret;
}

#define EMUNIT_FLASHSTR(s) \
	(__extension__({static const __flash char __c[] = (s); &__c[0];}))

#define PRIsPGM "S"

#else /* Compiler selection */
#error "Unsupported compiler"
#endif

#else /* Architecture selection */
#error "Unsupported architecture"
#endif


/**
 * @brief Auxiliary function that replaces standard C library function
 *
 * Syntax sweetener for @ref emunit_vsnprintf
 *
 * @param[out] s   See documentation for @ref emunit_vsnprintf
 * @param[in]  n   See documentation for @ref emunit_vsnprintf
 * @param[in]  fmt See documentation for @ref emunit_vsnprintf
 * @param[in]  ... Arguments for format string
 *
 * @return See documentation for @ref emunit_vsnprintf
 */
int emunit_snprintf(
	char * s,
	size_t n,
	char const __memx * fmt,
	...);

/**
 * @defgroup emunit_port_out_group Output stream functions
 * @{
 *
 * The output data streaming would be generated only just after reset.
 * During the whole test all the generated data is held in a RAM buffer.
 * This buffer is sent only just after the reset and before beginning of the test.
 * The output is initialised, then data is sent and output id de-initialised.
 * @{
 */
	/**
	 * @fn emunit_port_out_init
	 * @brief Initialise output port
	 */
	EMUNIT_PORT_MAP_VFUNC(out_init, (void), ())

	/**
	 * @fn emunit_port_out_deinit
	 * @brief De-initialise output port
	 */
	EMUNIT_PORT_MAP_VFUNC(out_deinit, (void), ())

	/**
	 * @fn emunit_port_out_write
	 * @brief Send data to the output port
	 *
	 * @param[in] p_str     The pointer to the data to be sent.
	 * @param[in] len       Number of bytes to be sent.
	 */
	EMUNIT_PORT_MAP_VFUNC(out_write, (char const * p_str, size_t len), (p_str, len))
/** @} */

/**
 * @fn emunit_port_early_init
 * @brief Early port initialisation
 *
 * Early init is called always at the very beginning of the program.
 * It is here for the out hardware to prepare some critical tasks
 * that has to be done before tests starts.
 * In most hardware ports it would be used to configure or disable WDT.
 *
 * @param[out] p_valid_key The pointer to validity key in status structure.
 *                         This function would never mark i as valid.
 *                         It could only mark it as an invalid if test should
 *                         be restarted or started from the beginning.
 */
EMUNIT_PORT_MAP_VFUNC(early_init, (emunit_status_key_t * p_valid_key), (p_valid_key))

/**
 * @fn emunit_port_restart
 * @brief Restart the whole program
 *
 * Function called when test condition fails and EMUnit library is ready
 * to reset.
 * The port function should make the MCU to reboot, it cannot return.
 */
static inline void EMUNIT_PORT_BASE_NAME(restart)(void) __attribute__ ((noreturn));
EMUNIT_PORT_MAP_VFUNC(restart, (void), ())


/**
 * @fn emunit_port_tc_begin
 * @brief Begin test case
 *
 * Function that would be called to mark that test case is just going to start.
 * If port supports any kind of timeouts it should start it right now.
 */
EMUNIT_PORT_MAP_VFUNC(tc_begin, (void), ())

/**
 * @fn emunit_port_tc_end
 * @brief Finalise test case
 *
 * Function that would be called to mark that test case has just been finished.
 * If port supports any kind of timeouts it should turn it off right now.
 */
EMUNIT_PORT_MAP_VFUNC(tc_end, (void), ())

/** @} */
#endif /* EMUNIT_PORT_H_INCLUDED */
