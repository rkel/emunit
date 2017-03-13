/**
 * @file
 * @brief EMUnit PC with stdout port implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File with EMUnit port that prints all data to stdout
 * @sa emunit_port_pctest_group
 */
#include "emunit_port_pctest.h"
#include <emunit_private.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <regex.h>
#include <setjmp.h>
#include <stdlib.h>

/* Variables required by the emunit engine */
emunit_status_t emunit_status;
emunit_display_status_t emunit_display_status;

/**
 * @brief Jump buffer to the beginning of the test loop
 *
 * The whole test runs in single thread.
 * When test fails long jump is performed to the main loop.
 *
 * This variable holds data calculated for the long jump to perform.
 */
static jmp_buf pctest_test_loop_jmp;

/**
 * @brief Number of tests failed
 *
 * This is the number of tests that failed to match the pattern.
 */
static unsigned int pctest_failed;

/**
 * @brief Number of tests successful
 *
 * This is the number of tests that passed to match the pattern.
 */
static unsigned int pctest_passed;

/**
 * @brief Expected pattern
 *
 * The expected pattern to be checked.
 */
static struct
{
	bool    ready; /**< Pattern is set and ready */
	regex_t re;    /**< The pattern itself */
}pctest_expected_pattern;

/** The test case name used if no test case has been started */
static const char pctest_tc_before_name[] = "BEFORE ANY CASE";
/** The test case name used if all test cases have been finished */
static const char pctest_tc_after_name[]  = "AFTER ALL CASES";
/** The test suite name used if all test suites have been finished */
static const char pctest_ts_after_name[]  = "AFTER ALL SUITES";
/** The test case name if there is no active test suite */
static const char pctest_tc_unknown_name[] = "---";


/**
 * @brief Get the current test suite name
 *
 * Auxiliary function that returns current test name.
 *
 * @return The pointer to the name string.
 */
static char const * emunit_pctest_current_ts_name(void)
{
	size_t idx = emunit_ts_current_index_get();
	if(idx >= emunit_ts_total_count())
		return pctest_ts_after_name;
	return emunit_ts_name_get(idx);
}

/**
 * @brief Get the current test case name
 *
 * Auxiliary function that returns current case name.
 *
 * @return The pointer to the name string.
 */
static char const * emunit_pctest_current_tc_name(void)
{
	size_t sidx = emunit_ts_current_index_get();
	if(sidx >= emunit_ts_total_count())
		return pctest_tc_unknown_name;
	size_t idx = emunit_tc_current_index_get();
	if(EMUNIT_IDX_INVALID == idx)
		return pctest_tc_before_name;
	if(idx >= emunit_tc_count(sidx))
		return pctest_tc_after_name;
	return emunit_tc_name_get(sidx, idx);
}


/**
 * @brief Function that prints the information about an error
 *
 * Function prints the line information about the error.
 * Prints also the information in with test the error occurred.
 *
 * @param[in] fmt Format string
 * @param[in] ... Parameters for the formatted string
 *
 * @note
 * This function increments internal error counter
 */
static void pctest_error(const char * fmt, ...)
{
	va_list vl;
	fprintf(
		stderr,
		"[%s][%s] ERROR: ",
		emunit_pctest_current_ts_name(),
		emunit_pctest_current_tc_name());
	va_start(vl, fmt);
	vfprintf(stderr, fmt, vl);
	va_end(vl);
	fputs("\n", stderr);

	++pctest_failed;
}

/**
 * @brief Pattern matched
 *
 * Function called when pattern matches - just writes information that
 * current test success.
 */
static void pctest_success(void)
{
	printf(
		"[%s][%s] SUCCESS\n",
		emunit_pctest_current_ts_name(),
		emunit_pctest_current_tc_name());
	++pctest_passed;
}

/**
 * @brief Summarise the whole test
 *
 * Function called at the end of the test - writes the statistic about the whole test.
 */
static void pctest_bill(void)
{
	puts("");
	puts("----------------------------------------");
	puts("Test finished");

	printf(
		"Success: %u\n"
		"Failed:  %u\n",
		pctest_passed,
		pctest_failed);

	if(0 == pctest_failed)
	{
		puts("All tests passed");
	}
}


void emunit_pctest_expected_set(char const * p_str)
{
	int ret;
	EMUNIT_IASSERT_MSG(!pctest_expected_pattern.ready, "Pattern already set");

	ret = regcomp(&pctest_expected_pattern.re, p_str, REG_EXTENDED | REG_NOSUB);
	if(0 != ret)
	{
		fprintf(stderr, "Cannot compile reqular expression: %d.\n", ret);
		EMUNIT_IASSERT_MSG(false, "Cannot compile regular expression");
	}
	pctest_expected_pattern.ready = true;
}

void emunit_port_pctest_out_init(void)
{
}

void emunit_port_pctest_out_deinit(void)
{
}

void emunit_port_pctest_out_write(char const * p_str, size_t len)
{
	/* Check against pattern */
	if(pctest_expected_pattern.ready)
	{
		pctest_expected_pattern.ready = false;
		if(len != strlen(p_str))
			pctest_error("String length does not match written length");
		else
		{
			int match_ret;
			match_ret = regexec(&pctest_expected_pattern.re, p_str, 0, NULL, REG_NOSUB);
			if(0 != match_ret)
				pctest_error(
					"Pattern does not match\n"
					"    Output:\n"
					"    ------------------------\n"
					"%s"
					"    ------------------------\n",
					p_str);
			else
				pctest_success();
		}
	}
	else
	{
		pctest_error(
			"No pattern set\n"
			"    Output:\n"
			"    ------------------------\n"
			"%s"
			"    ------------------------\n",
			p_str);
	}
}

void emunit_port_pctest_early_init(emunit_status_key_t * p_valid_key)
{
	pctest_failed = 0;
	pctest_passed = 0;
	memset(&pctest_expected_pattern, 0, sizeof(pctest_expected_pattern));

	/* We are working in a loop.
	 * Early init is called always and only once when the program starts */
	*p_valid_key = EMUNIT_STATUS_KEY_INVALID;
}

void emunit_port_pctest_restart(void)
{
	/* Exit current thread */
	longjmp(pctest_test_loop_jmp, 1);

	/* This should never happen */
	fputs("Thread did not exit properly", stderr);
	exit(-1);
}

void emunit_port_pctest_tc_begin(void)
{

}

void emunit_port_pctest_tc_end(void)
{

}


int main(void)
{
	int ret;
	emunit_early_init();

	/* -----------------------------------
	 * Start of the test loop
	 */
	/* EMUnit engine would jump here as long as there is anything to process */
	setjmp(pctest_test_loop_jmp);

	emunit_flush();
	ret = emunit_run();
	/*
	 * End of the test loop
	 * --------------------------------- */
	pctest_bill();
	return ret;
}
