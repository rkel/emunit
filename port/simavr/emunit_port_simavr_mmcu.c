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
 * @brief MCU description for simavr implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File with EMUnit port for hardware AVR UART.
 * @sa emunit_port_simavr_group
 */
#include "config_emunit_simavr.h"
#include <emunit_macros.h>
/* Note simavr include directory needs to be in the include path */
#include <simavr/avr/avr_mcu_section.h>

AVR_MCU(F_CPU, EMUNIT_STR(MCU_NAME));
// tell simavr to listen to commands written in this (unused) register
AVR_MCU_SIMAVR_CONSOLE(SIMULAVR_REGSTDOUT_ADDR);

