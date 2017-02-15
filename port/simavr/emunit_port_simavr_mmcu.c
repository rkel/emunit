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

