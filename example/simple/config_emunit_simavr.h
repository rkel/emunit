#ifndef CONFIG_EMUNIT_SIMAVR_H_INCLUDED
#define CONFIG_EMUNIT_SIMAVR_H_INCLUDED
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
 * @brief EMUnit simavr port configuration
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File with EMUnit configuration for simavr port.
 * @sa config_emunit_simavr_group
 */
#include <avr/io.h>

/**
 * @defgroup config_emunit_simavr_group <config_emunit_simavr.h> EMUnit simavr port configuration
 * @{
 * @ingroup config_emunit_group
 *
 * Default EMUnit configuration for simavr port.
 * All values here can be overwritten by global definitions.
 */

/**
 * @brief Virtual stdout register address
 *
 * This is the address of virtual peripheral register in simavr.
 * Every character written here is sent to simavr stdout.
 */
#ifndef SIMULAVR_REGSTDOUT_ADDR
#define SIMULAVR_REGSTDOUT_ADDR (&OCDR)
#endif

/** @} */
#endif /* CONFIG_EMUNIT_SIMAVR_H_INCLUDED */
