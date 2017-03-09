#ifndef EMUNIT_PORT_ARCH_AVR_H_INCLUDED
#define EMUNIT_PORT_ARCH_AVR_H_INCLUDED
/**
 * @file
 * @brief EMUnit AVR architecture port implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File with EMUnit architecture for AVR simulator
 * @sa emunit_port_simavr_group
 */

/**
 * @addtogroup emunit_arch_avr_group <emunit_arch_avr> EMUnit AVR architecture
 * @{
 * @ingroup emunit_arch_group
 *
 * @{
 */

/**
 * @def EMUNIT_NOINIT_VAR(type, name)
 * @brief Declare variable not to be cleared at program start
 *
 * Variable created by this macro have to retain its value during system restart.
 * Variable of such type cannot have any initialisation value.
 *
 * @param type Variable type
 * @param name Variable name
 *
 * @note
 * This macro is only used internally by the files that uses this port.
 */

/* Check the architecture */
#if !(defined(__AVR__))
#error "Unsupported architecture"
#endif

/* Check compiler */
#if ( defined(__GNUC__) && (__GNUC__ >= 4) )

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

#define EMUNIT_NOINIT_VAR(type, name) \
	type name __attribute__((section(".noinit")))

#else /* Compiler selection */
#error "Unsupported compiler"
#endif

/**
 * @brief The macro that declares all required variables
 *
 * This macro should be called at the very top of the port file.
 * It declares all the variables required by the EMUNIT engine.
 */
#define EMUNIT_PORT_REQUIRED_VARIABLES()                               \
	EMUNIT_NOINIT_VAR(emunit_status_t, emunit_status);                 \
	EMUNIT_NOINIT_VAR(emunit_display_status_t, emunit_display_status)

#endif /* EMUNIT_PORT_ARCH_AVR_H_INCLUDED */
