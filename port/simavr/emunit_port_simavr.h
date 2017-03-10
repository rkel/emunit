#ifndef EMUNIT_PORT_SIMAVR_H_INCLUDED
#define EMUNIT_PORT_SIMAVR_H_INCLUDED
/**
 * @file
 * @brief EMUnit simavr simulator port
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * File with EMUnit port for simavr simulator.
 * @sa emunit_port_simavr_group
 */
#include <emunit_types.h>
#include "emunit_port_simavr_arch.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @defgroup emunit_port_simavr_group <emunit_port_simavr> EMUnit SimAVR port
 * @{
 * @ingroup emunit_port_group
 *
 * @{
 */

/**
 * @defgroup emunit_port_simavr_out_group Output stream functions
 * @{
 *
 * @sa emunit_port_out_group
 */
	/** See the @ref emunit_port_out_init documentation */
	void emunit_port_simavr_out_init(void);

	/** See the @ref emunit_port_out_deinit documentation */
	void emunit_port_simavr_out_deinit(void);

	/** See the @ref emunit_port_out_write documentation */
	void emunit_port_simavr_out_write(char const * p_str, size_t len);
/** @} */

/** See the @ref emunit_port_first_init documentation */
void emunit_port_simavr_early_init(emunit_status_key_t * p_valid_key);

/** See the @ref emunit_port_restart documentation */
void emunit_port_simavr_restart(void) __attribute__ ((noreturn));

/** See the @ref emunit_port_tc_begin documentation */
void emunit_port_simavr_tc_begin(void);

/** See the @ref emunit_port_tc_end documentation */
void emunit_port_simavr_tc_end(void);

/** @} */
#endif /* EMUNIT_PORT_SIMAVR_H_INCLUDED */
