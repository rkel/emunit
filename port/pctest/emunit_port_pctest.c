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
 * @brief Expected pattern structure
 *
 * The structure that contains all the data required for the pattern matching.
 */
typedef struct
{
	bool    ready;                           /**< Pattern is set and ready */
	regex_t re;                              /**< The pattern itself */
	char    str_re[EMUNIT_TEST_PATTERN_MAX]; /**< Printable copy of test pattern */
}pctest_pattern_t;

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
 * @brief Expected pattern of the footer
 *
 * If the test footer appears it would be checked agains this pattern.
 */
pctest_pattern_t pctest_expected_footer;

/**
 * @brief Expected pattern of the current test
 *
 * The expected pattern to be checked.
 */
pctest_pattern_t pctest_expected_test_output;


/** The test case name used if no test case has been started */
static const char pctest_tc_before_name[] = "BEFORE ANY CASE";
/** The test case name used if all test cases have been finished */
static const char pctest_tc_after_name[]  = "AFTER ALL CASES";
/** The test case name used if not case have been started */
static const char pctest_ts_before_name[] = "BEFORE ANY SUITE";
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
	if(EMUNIT_IDX_INVALID == idx)
		return pctest_ts_before_name;
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
	if((EMUNIT_IDX_INVALID == sidx) || (sidx >= emunit_ts_total_count()))
		return pctest_tc_unknown_name;
	size_t idx = emunit_tc_current_index_get();
	if(EMUNIT_IDX_INVALID == idx)
		return pctest_tc_before_name;
	if(idx >= emunit_tc_count(sidx))
		return pctest_tc_after_name;
	return emunit_tc_name_get(sidx, idx);
}

/**
 * @brief Set the pattern
 *
 * Sets pattern in the selected destination.
 * Prints error information if the pattern is currently set.
 *
 * @param[in,out] p_dst   Destination pattern structure.
 * @param[in]     pattern The string with the pattern to be set.
 */
static void emunit_pctest_pattern_set(pctest_pattern_t * p_dst, char const * pattern)
{
	int ret;
	EMUNIT_IASSERT_MSG(EMUNIT_TEST_PATTERN_MAX > strlen(pattern), "Pattern string too long");
	EMUNIT_IASSERT_MSG(!p_dst->ready, "Pattern already set");

	ret = regcomp(&p_dst->re, pattern, REG_EXTENDED | REG_NOSUB);
	if(0 != ret)
	{
		fprintf(stderr, "Cannot compile reqular expression: %d.\n", ret);
		EMUNIT_IASSERT_MSG(false, "Cannot compile regular expression");
	}
	strcpy(p_dst->str_re, pattern);
	p_dst->ready = true;
}

/**
 * @brief Check against pattern
 *
 * Checks if given string matches the pattern.
 *
 * @param[in]  p_pattern Pattern to be checked
 * @param[in]  p_val     The string to be checked agaist the pattern
 * @return
 */
static bool emunit_pctest_pattern_check(pctest_pattern_t const * p_pattern, char const * p_val)
{
	int match_ret;
	match_ret = regexec(&p_pattern->re, p_val, 0, NULL, REG_NOSUB);
	return (0 == match_ret);
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

char const * emunit_pctest_regex_esc(char * const p_dst, char const * p_src)
{
	char * p_esc = p_dst;
	char c;
	do
	{
		c = *p_src++;
		switch(c)
		{
		case '.':
		case '?':
		case '\\':
		case '+':
		case '*':
		case '[':
		case '(':
		case ')':
			*p_esc++ = '\\';
			break;
		default:
			break;
		}
		*p_esc++ = c;
	}while('\0' != c);

	return p_dst;
}

void emunit_pctest_expected_footer_set(char const * p_str)
{
	emunit_pctest_pattern_set(&pctest_expected_footer, p_str);
}

void emunit_pctest_expected_set(char const * p_str)
{
	emunit_pctest_pattern_set(&pctest_expected_test_output, p_str);
}

void emunit_port_pctest_out_init(void)
{
}

void emunit_port_pctest_out_deinit(void)
{
}

void emunit_port_pctest_out_write(char const * p_str, size_t len)
{
	/* Select the pattern */
	pctest_pattern_t * p_pattern;
	size_t sidx = emunit_ts_current_index_get();
	if((sidx == EMUNIT_IDX_INVALID) || (sidx < emunit_ts_total_count()))
	{
		p_pattern = &pctest_expected_test_output;
	}
	else
	{
		p_pattern = &pctest_expected_footer;
	}

	/* Check against pattern */
	if(p_pattern->ready)
	{
		p_pattern->ready = false;
		if(len != strlen(p_str))
			pctest_error("String length does not match written length");
		else
		{
			int match_ret;
			if(!(emunit_pctest_pattern_check(p_pattern, p_str)))
				pctest_error(
					"Pattern does not match\n"
					"    Pattern:\n"
					"    ------------------------\n"
					"%s"
					"    ------------------------\n"
					"    Output:\n"
					"    ------------------------\n"
					"%s"
					"    ------------------------\n",
					p_pattern->str_re,
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
	memset(&pctest_expected_test_output, 0, sizeof(pctest_expected_test_output));

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

	/* Set long jmp temporary here is anything fails during initialisation */
	if(0 == setjmp(pctest_test_loop_jmp))
	{
		emunit_early_init();
		/* Prepare for the test header */
		emunit_test_prepare();
	}

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
