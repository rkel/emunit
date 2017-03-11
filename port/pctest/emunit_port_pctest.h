#ifndef EMUNIT_PORT_PCTEST_H_INCLUDED
#define EMUNIT_PORT_PCTEST_H_INCLUDED
/**
 * @file
 * @brief EMUnit PC Test port
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 * File with EMUnit port for PC Test.
 * @sa emunit_port_pctest_group
 */
#include <emunit_types.h>
#include "emunit_port_pctest_arch.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @defgroup emunit_port_pctest_group <emunit_port_pctest> EMUnit PC Test port
 * @{
 * @ingroup emunit_port_group
 *
 * Very specific port that would be used to test the selected port
 * using bach or regular expressions.
 *
 * The port expects specific output for the test and fails only if the
 * output does not match.
 * @{
 */

/**
 * @brief Set the expected pattern
 *
 * Function sets the regular expression to match the incoming data.
 * The expression set would be matched with the nearest incoming data.
 *
 * @param p_str String with regular expression
 */
void emunit_pctest_expected_set(char const * p_str);

/**
 * @defgroup emunit_port_pctest_out_group Output stream functions
 * @{
 *
 * @sa emunit_port_out_group
 */
	/** See the @ref emunit_port_out_init documentation */
	void emunit_port_pctest_out_init(void);

	/** See the @ref emunit_port_out_deinit documentation */
	void emunit_port_pctest_out_deinit(void);

	/** See the @ref emunit_port_out_write documentation */
	void emunit_port_pctest_out_write(char const * p_str, size_t len);
/** @} */

/** See the @ref emunit_port_first_init documentation */
void emunit_port_pctest_early_init(emunit_status_key_t * p_valid_key);

/** See the @ref emunit_port_restart documentation */
void emunit_port_pctest_restart(void) __attribute__ ((noreturn));

/** See the @ref emunit_port_tc_begin documentation */
void emunit_port_pctest_tc_begin(void);

/** See the @ref emunit_port_tc_end documentation */
void emunit_port_pctest_tc_end(void);

/** @} */
#endif /* EMUNIT_PORT_PCTEST_H_INCLUDED */
