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
 * @brief EMUnit port source code
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * This file just includes the right source file form the selected port.
 * Nothing more done here.
 * It is here only to simplify the compilation rules.
 * @sa emunit_port_group
 */
#include "emunit_port.h"

int emunit_snprintf(
	char * s,
	size_t n,
	char const __memx * fmt,
	...)
{
	int ret;
	va_list args;
	va_start(args, fmt);
	ret = emunit_vsnprintf(s, n, fmt, args);
	va_end(args);

	return ret;
}

/* Include the source code for selected port */
#include EMUNIT_PORT_FILE( , c)
