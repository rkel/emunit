#ifndef CONFIG_EMUNIT_SIMAVR_H_INCLUDED
#define CONFIG_EMUNIT_SIMAVR_H_INCLUDED
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
