#ifndef EMUNIT_PORT_ARCH_PC_H_INCLUDED
#define EMUNIT_PORT_ARCH_PC_H_INCLUDED
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
 * @brief EMUnit PC architecture port implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File with EMUnit architecture for PC
 * @sa emunit_arch_pc_group
 */

/**
 * @defgroup emunit_arch_pc_group <emunit_arch_avr> EMUnit PC architecture
 * @{
 * @ingroup emunit_arch_group
 *
 * PC architecture should be easy enough that it should support any
 * kind of compiler.
 */

/**
 * @name Definitions of special keywords that have to be dummied
 *
 * EMUnit currently uses some of the keywords that are typical to bare metal
 * compilations for the MCU that places some variables in special space.
 *
 * This keywords have to be dummied for the PC compilation
 *
 * @todo This should be solved better - in more universal form.
 * @{
 */
/** Variable placed in FLASH memory */
#define __flash
/** Pointer to any type of the variables (FLASH or RAM) */
#define __memx
/** @} */

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define EMUNIT_FLASHSTR(s) s

#define PRIsPGM "s"

static inline void * emunit_memcpy(
	void * p_dst,
	void const __memx * p_src,
	size_t size)
{
	return memcpy(p_dst, p_src, size);
}

static inline size_t emunit_strlen(char const __memx * s)
{
	return strlen(s);
}

static inline char* emunit_strstr(char const * str1, char const __memx * str2)
{
	return strstr(str1, str2);
}

static inline int emunit_vsnprintf(
	char * s,
	size_t n,
	char const __memx * fmt,
	va_list va)
{
	return vsnprintf(s, n, fmt, va);
}

/** @} <!-- emunit_arch_pc_group --> */
#endif /* EMUNIT_PORT_ARCH_PC_H_INCLUDED */
