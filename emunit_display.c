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
 * @var emunit_display_status
 * @brief Display status variable
 *
 * The variable that stores current display status.
 * It has to be placed in port file in a way that it does not lost the value
 * when test is restarted.
 *
 * @sa emunit_status
 */
extern emunit_display_status_t emunit_display_status;

/**
 * @brief Buffer end pointer
 *
 * Used for internal calculations
 */
static const char * emunit_display_buffer_end =
	&(emunit_display_status.buffer[EMUNIT_CONF_BUFFER_SIZE]);
/** @} */

/**
 * @defgroup emunit_display_port_group
 * Presentation module internals to be used in porting files
 * @ingroup emunit_display_group
 *
 * Group of special kind of internal functions and types that would be used in the
 * @em emunit_display_MODE.c files.
 * @{
 */

/**
 * @brief Function used to clean up the buffer after printing
 *
 * This function, used in one of following functions:
 *
 * - @ref emunit_display_puts
 * - @ref emunit_display_printf
 *
 * It would be called after the message has been successfully placed in output
 * display buffer.
 * Now operating directly on the display buffer some parts can be replaced.
 *
 * Use @ref emunit_display_replace to make the replacement safetly.
 *
 * @note Clean up functionality is not implemented in @ref emunit_display_putc.
 *       Any single character access can be solved simply without calling
 *       after clean up function.
 *
 * @param p_start Start of the string just pushed into output buffer
 * @param len     Length of the string just pushed into output buffer
 */
typedef void (*emunit_display_cleanup_fn_t)(char * p_start, size_t len);

/**
 * @brief Get the buffer size left
 *
 * Internal function used to count the number of bytes in the buffer left
 *
 * @return Number of bytes left in the buffer
 */
static inline size_t emunit_display_max_size(void)
{
	return (emunit_display_buffer_end - emunit_display_status.w_ptr);
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
	EMUNIT_IASSERT(emunit_display_max_size() > 1);
	*(emunit_display_status.w_ptr++)= c;
}

/**
 * @brief Copy exactl number of characters to the output buffer
 *
 * Function copies exact number of bytes to the output.
 *
 * @note
 * The given buffer cannot have null characters inside included.
 * Any null character inside given character range can break the rest of the conversion.
 *
 * @param[in] cleanup Cleanup function to be called after data is sent to the buffer
 * @param[in] s       Source string to be copied into buffer.
 * @param[in] size    Size of the string to be copied into buffer.
 */
static void emunit_display_write(emunit_display_cleanup_fn_t cleanup, char const __memx * s, size_t size)
{
	const size_t max_size = emunit_display_max_size();
	char * ptr_start;
	EMUNIT_IASSERT(size < max_size);

	emunit_memcpy(emunit_display_status.w_ptr, s, size);
	ptr_start = emunit_display_status.w_ptr;
	emunit_display_status.w_ptr += size;
	if(NULL != cleanup)
		cleanup(ptr_start, size);
}

/**
 * @brief Put string up to selected number of characters
 *
 * @param[in] cleanup Cleanup function to be called after data is sent to the buffer.
 * @param[in] s       Source string to be copied into buffer.
 * @param[in] n       Maximum number of characters to copy.
 */
static void emunit_display_nputs(emunit_display_cleanup_fn_t cleanup, char const __memx * s, size_t n)
{
	const size_t slen = emunit_strlen(s);
	const size_t size = EMUNIT_MIN(slen, n);
	emunit_display_write(cleanup, s, size);
}

/**
 * @brief Put string
 *
 * @param[in] cleanup Cleanup function to be called after data is sent to the buffer
 * @param[in] s       Source string to be copied into buffer.
 */
static void emunit_display_puts(emunit_display_cleanup_fn_t cleanup, char const __memx * s)
{
	const size_t size = emunit_strlen(s);
	emunit_display_write(cleanup, s, size);
}

/**
 * @brief Variant of @ref emunit_display_printf but takes variable argument list
 *
 *
 * @param[in]  cleanup  Cleanup function to be called after data is printed into the buffer.
 * @param[in]  fmt      Format string.
 * @param[in]  args     Variable argument list.
 *
 * @sa emunit_display_printf
 */
static void emunit_display_vprintf(emunit_display_cleanup_fn_t cleanup, char const __memx * fmt, va_list args)
{
	const size_t max_size = emunit_display_max_size();
	size_t printed_size;
	char * ptr_start;

	printed_size = emunit_vsnprintf(
		emunit_display_status.w_ptr,
		max_size,
		fmt,
		args);

	EMUNIT_IASSERT(printed_size < max_size);
	ptr_start = emunit_display_status.w_ptr;
	emunit_display_status.w_ptr += printed_size;
	if(NULL != cleanup)
		cleanup(ptr_start, printed_size);
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
 * @param[in]  cleanup Cleanup function to be called after data is printed into the buffer
 * @param[in]  fmt     Format string.
 * @param[in]  ...     Parameters.
 *
 * @sa emunit_display_vprintf
 */
static void emunit_display_printf(emunit_display_cleanup_fn_t cleanup, char const __memx * fmt,	...)
{
	va_list args;
	va_start(args, fmt);
	emunit_display_vprintf(cleanup, fmt, args);
	va_end(args);
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
	if(size >= max_size)
	{
		size = max_size - 1;
	}
	emunit_memcpy(emunit_display_status.w_ptr, s, size);
	emunit_display_status.w_ptr += size;
}

/**
 * @brief Replace string in the output buffer
 *
 * This function may be used to safely replace part of the output buffer.
 * It is meant to be used inside @ref emunit_display_cleanup_fn_t.
 *
 * If the len of the new string is different than @c len,
 * buffer contents would be safely moved.
 *
 * If the buffer after movement would overflow this function would fail
 * calling internal assert.
 *
 * @param p_start Start of the part to be replaced.
 *                This address has to be located inside display output buffer.
 * @param len     Length of the string to replace
 * @param s
 *
 * @return Pointer to the buffer string just after replacement.
 */
static char* emunit_display_replace(char * p_start, size_t len, const char __memx * s)
{
	EMUNIT_IASSERT((emunit_display_status.buffer <= p_start) &&
		(p_start < emunit_display_buffer_end));
	EMUNIT_IASSERT(p_start + len <= emunit_display_status.w_ptr);
	size_t s_len = emunit_strlen(s);
	if(s_len != len)
	{
		EMUNIT_IASSERT((s_len <= len) ||
			(emunit_display_status.w_ptr + s_len - len < emunit_display_buffer_end));
		/* Move with trailing zero */
		char * p_to = p_start + s_len;
		char * p_from = p_start + len;
		size_t moved_size = emunit_display_status.w_ptr - p_start - len + 1;
		/** @todo Clean this up */
//		memmove(p_start + s_len, p_start + len, emunit_display_status.w_ptr - p_start - len + 1);
		memmove(p_to, p_from, moved_size);
		emunit_display_status.w_ptr = emunit_display_status.w_ptr + s_len - len;
	}
	emunit_memcpy(p_start, s, s_len);

	return p_start + s_len;
}

/** @} */

void emunit_display_clear(void)
{
	memset(&emunit_display_status, 0, sizeof(emunit_display_status));
	emunit_display_status.w_ptr = emunit_display_status.buffer;
}


void emunit_display_present(void)
{
	EMUNIT_IASSERT(emunit_display_status.w_ptr >= emunit_display_status.buffer);
	EMUNIT_IASSERT((emunit_display_status.w_ptr - emunit_display_status.buffer) <= EMUNIT_CONF_BUFFER_SIZE);
	/* Present only if there is anything to present */
	if(emunit_display_status.w_ptr > emunit_display_status.buffer)
	{
		emunit_port_out_init();
		emunit_port_out_write(
			emunit_display_status.buffer,
			emunit_display_status.w_ptr - emunit_display_status.buffer);
		emunit_port_out_deinit();
		emunit_display_clear();
	}
}


void emunit_display_panic(
	const __flash char * str_file,
	unsigned int line)
{
	emunit_display_clear();
	EMUNIT_DISPLAY_NAME(show_panic)(str_file, line);
}

/* Include the source file for selected display */
#include EMUNIT_DISPLAY_FILE(, c)
