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
 * @brief Maximum pattern length
 *
 * Constant that configures maximal size of the pattern string.
 */
#define EMUNIT_TEST_PATTERN_MAX 512

/**
 * @brief Function that would be called before test main loop
 *
 * This function have to be defined inside test.
 * It should prepare pattern for the incoming test header.
 */
void emunit_test_prepare(void);

/**
 * @brief Escape regular expression string
 *
 * Auxiliary function that replaces all occurrence of special characters
 * by its escaped version.
 *
 * @param[in]  p_dst Destination buffer.
 * @param[out] p_src Source string to be escaped.
 *
 * @return The pointer to the destination string.
 */
char const * emunit_pctest_regex_esc(char * p_dst, char const * p_src);

/**
 * @brief Set the expected footer pattern
 *
 * Function sets the regular expression to match the incoming footer data.
 * This pattern would be used when test footer would be detected.
 *
 * @param[in] p_str String with regular expression
 */
void emunit_pctest_expected_footer_set(char const * p_str);

/**
 * @brief Set the expected pattern
 *
 * Function sets the regular expression to match the incoming data.
 * The expression set would be matched with the nearest incoming data.
 *
 * @param[in] p_str String with regular expression
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
