#ifndef EMUNIT_PORT_H_INCLUDED
#define EMUNIT_PORT_H_INCLUDED
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
 * @defgroup emunit_arch_group <emunit_arch> EMUnit architecture port
 *
 * Functions and macros that are specyfic for the selected architecture
 * @{
 */

/**
 * @def EMUNIT_FLASHSTR(s)
 * @brief Place string in FLASH memory
 *
 * This macro is used for every internal string that may be placed in
 * FLASH.
 * In the architectures where FLASH and RAM are treated the same it may be
 * macro that just returns @c s.
 */

/**
 * @def PRIsPGM
 * String printf from program memory
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
 * @fn emunit_strlen
 * @brief Checks string length
 *
 * Function that returns the length of the string that may be placed in any kind
 * of memory.
 *
 * @param[in] s String
 * @return The length of the given string
 */

/**
 * @fn emunit_strstr
 * @brief Search the string
 *
 * Searches first occurrence of the @c str2 in @c str1,
 * where str2 can be placed in any kind of memory.
 *
 * @param[in] str1 String where to search in RAM
 * @param[in] str2 String witch to search in any kind of memory
 *
 * @return Pointer to the string or NULL.
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

/** @} <!-- emunit_arch_group -->*/

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
