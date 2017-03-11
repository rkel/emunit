/**
 * @file
 * @brief EMUnit PC with stdout port implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File with EMUnit port that prints all data to stdout
 * @sa emunit_port_pcstdout_group
 */
#include "emunit_port_pcstdout.h"
#include <emunit_private.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

/* Variables required by the emunit engine */
emunit_status_t emunit_status;
emunit_display_status_t emunit_display_status;

/**
 * @brief Variable that holds current test state
 *
 * Tests runs using threads.
 * This variable is cleared when test finishes.
 */
static bool pcstdout_continue_test;

/**
 * @brief Put single character to out register
 *
 * Auxiliary internal function.
 * It sends to console given value without any conversion.
 *
 * @param c Character to be printed
 */
static inline void pcstdout_putc_raw(char c)
{
	putchar(c);
}

/**
 * @brief Print single character to the console
 *
 * Function uses pcstdout simulator special register to print the data to the
 * standard output.
 * This function performs some basic conversion.
 * See @ref pcstdout_putc_raw for function without any conversion.
 *
 * @param c Character to be printed
 */
static inline void pcstdout_putc(char c)
{
	switch(c)
	{
	case '\t':
		pcstdout_putc_raw(' ');
		pcstdout_putc_raw(' ');
		break;
	default:
		pcstdout_putc_raw(c);
		break;
	}
}


void emunit_port_pcstdout_out_init(void)
{
}

void emunit_port_pcstdout_out_deinit(void)
{
}

void emunit_port_pcstdout_out_write(char const * p_str, size_t len)
{
	while(0 != len--)
	{
		pcstdout_putc(*(p_str++));
	}
}

void emunit_port_pcstdout_early_init(emunit_status_key_t * p_valid_key)
{
	/* We are working in a loop.
	 * Early init is called always and only once when the program starts */
	*p_valid_key = EMUNIT_STATUS_KEY_INVALID;
}

void emunit_port_pcstdout_restart(void)
{
	/* Exit current thread */
	pthread_exit((void*)1);

	/* This should never happen */
	fputs("Thread did not exit properly", stderr);
	exit(-1);
}

void emunit_port_pcstdout_tc_begin(void)
{

}

void emunit_port_pcstdout_tc_end(void)
{

}

/**
 * Thread That runs the test
 *
 * This is a thread that runs the test.
 * It clears @ref pcstdout_continue_test global variable if test finishes.
 *
 * @param p_param Unused
 * @return The value returned by the emunit test runtime
 */
static void* test_thread(void * p_param)
{
	(void)p_param;
	int ret;

	ret = emunit_run();
	/* If the run function exits normally it means that the whole test has finished.
	 * If the test continues it exits calling @ref emunit_port_pcstdout_restart.
	 */
	pcstdout_continue_test = false;
	return (void*)ret;
}


int main(void)
{
	int ret;
	emunit_early_init();
	pcstdout_continue_test = true;

	while(pcstdout_continue_test)
	{
		pthread_t thread_id;
		void * test_ret;

		emunit_flush();

		pthread_create(&thread_id, NULL, test_thread, NULL);
		pthread_join(thread_id, &test_ret);
		ret = (int)test_ret;
	}
	return ret;
}
