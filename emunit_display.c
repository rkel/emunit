/**
 * @file
 * @brief EMUnit general presentation source file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * @sa emunit_display_group
 */

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "emunit_private.h"
#include "emunit_display.h"
#include "emunit_port.h"

/**
 * @defgroup emunit_display_internal_group Presentation module internals
 * @ingroup emunit_display_group
 *
 * @{
 */

/**
 * @brief Output data buffer
 *
 * Block of data that holds all printed data during single test.
 */
EMUNIT_NOINIT_VAR(char, emunit_display_buffer[EMUNIT_CONF_BUFFER_SIZE]);

/**
 * @brief Write pointer
 *
 * The write pointer to the buffer
 */
EMUNIT_NOINIT_VAR(char *, emunit_display_wptr);

/**
 * @brief Buffer end pointer
 *
 * Used for internal calculations
 */
static const char * emunit_display_buffer_end =
	&(emunit_display_buffer[EMUNIT_CONF_BUFFER_SIZE]);
/** @} */

/**
 * @defgroup emunit_display_port_group
 * Presentation module internals to be used in porting files
 * @ingroup emunit_display_group
 *
 * Group of special kind of internal functions that would be used in the
 * @em emunit_display_MODE.c files.
 * @{
 */

/**
 * @brief Get the buffer size left
 *
 * Internal function used to count the number of bytes in the buffer left
 *
 * @return Number of bytes left in the buffer
 */
static inline size_t emunit_display_max_size(void)
{
	return (emunit_display_buffer_end - emunit_display_wptr);
}

/**
 * @brief Put character into output buffer
 *
 * @note
 * This function would generate assertion failure if internal buffer overruns.
 *
 * @param c Character to be written
 */
static void emunit_display_putc(char c)
{
	EMUNIT_IASSERT(emunit_display_max_size() >= 1);
	*emunit_display_wptr++ = c;
}

/**
 * @brief Put string
 *
 * @param s Source string to be copied into buffer.
 */
static void emunit_display_puts(char const __memx * s)
{
	const size_t max_size = emunit_display_max_size();
	const size_t size = emunit_strlen(s);
	EMUNIT_IASSERT(size <= max_size);

	emunit_strncpy(emunit_display_wptr, s, size);
	emunit_display_wptr += size;
}

/**
 * @brief Internal formatted output function
 *
 * This function should be used internally by all the displaying functions
 * in selected display mode.
 *
 * @note
 * This function would generate assertion failure if internal buffer overruns.
 *
 * @param[in]  fmt Format string.
 * @param[in]  ... Parameters.
 */
static void emunit_display_printf(char const __memx * fmt,	...)
{
	va_list args_ptr;
	const size_t max_size = emunit_display_max_size();
	size_t printed_size;

	va_start(args_ptr, fmt);
	printed_size = emunit_snprintf(
		emunit_display_wptr,
		max_size,
		fmt,
		args_ptr);
	va_end(args_ptr);

	EMUNIT_IASSERT(printed_size < max_size);
	emunit_display_wptr += printed_size;
}

/**
 * @brief Put character in panic message printing
 *
 * Specialised function to be used in panic display.
 * It never asserts the fact if string fits the buffer.
 * It just limit the size of copied data.
 *
 * @note
 * Use @ref emunit_display_putc in any other displaying function
 * than @ref emunit_display_panic.
 *
 * @param c
 */
static void emunit_display_panic_putc(char c)
{
	if(emunit_display_max_size() >= 1)
	{
		*emunit_display_wptr++ = c;
	}
}

/**
 * @brief Put string in panic message printing
 *
 * Specialised function to be used in panic display.
 * It never asserts the fact if string fits the buffer.
 * It just limit the size of copied data.
 *
 * @note
 * Use @ref emunit_display_printf or @ref emunit_display_puts
 * in any other displaying function than @ref emunit_display_panic.
 *
 * @param s Source string to be copied into buffer.
 */
static void emunit_display_panic_puts(char const __memx * s)
{
	const size_t max_size = emunit_display_max_size();
	size_t size = emunit_strlen(s);
	if(size > max_size)
	{
		size = max_size;
	}
	emunit_strncpy(emunit_display_wptr, s, size);
	emunit_display_wptr += size;
}

/** @} */

void emunit_display_clear(void)
{
	emunit_display_wptr = emunit_display_buffer;
	memset(emunit_display_buffer, 0, sizeof(emunit_display_buffer));
}


void emunit_display_present(void)
{
	EMUNIT_IASSERT(emunit_display_wptr >= emunit_display_buffer);
	EMUNIT_IASSERT((emunit_display_wptr - emunit_display_buffer) <= EMUNIT_CONF_BUFFER_SIZE);
	emunit_port_out_init();
	emunit_port_out_write(
		emunit_display_buffer,
		emunit_display_wptr - emunit_display_buffer);
	emunit_port_out_deinit();
}


void emunit_display_panic(
	const __flash char * str_file,
	const __flash char * str_line,
	const __flash char * str_msg)
{
	emunit_display_clear();
	EMUNIT_DISPLAY_NAME(show_panic)(str_file, str_line, str_msg);
}

/* Include the source file for selected display */
#include EMUNIT_DISPLAY_FILE(, c)
