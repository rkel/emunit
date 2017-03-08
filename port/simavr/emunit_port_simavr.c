/**
 * @file
 * @brief EMUnit simavr port implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File with EMUnit port for simavr simulator
 * @sa emunit_port_simavr_group
 */
#include "config_emunit_simavr.h"
#include "emunit_port_simavr.h"
#include "emunit_private.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include "emunit_port_simavr_mmcu.c"


/**
 * @brief Put single character to out register
 *
 * Auxiliary internal function.
 * It sends to console given value without any conversion.
 *
 * @param c Character to be printed
 */
static inline void simavr_putc_raw(char c)
{
	*((char*)SIMULAVR_REGSTDOUT_ADDR) = c;
}

/**
 * @brief Print single character to the console
 *
 * Function uses simavr simulator special register to print the data to the
 * standard output.
 * This function performs some basic conversion.
 * See @ref simavr_putc_raw for function without any conversion.
 *
 * @param c Character to be printed
 */
static inline void simavr_putc(char c)
{
	switch(c)
	{
	case '\n':
		simavr_putc_raw('\r');
		break;
	case '\t':
		simavr_putc_raw(' ');
		simavr_putc_raw(' ');
		break;
	default:
		simavr_putc_raw(c);
		break;
	}
}


void emunit_port_simavr_out_init(void)
{
}

void emunit_port_simavr_out_deinit(void)
{
}

void emunit_port_simavr_out_write(char const * p_str, size_t len)
{
	while(0 != len--)
	{
		simavr_putc(*(p_str++));
	}
}

void emunit_port_simavr_early_init(emunit_status_key_t * p_valid_key)
{
	uint8_t mcusr = MCUCSR;
	if((0U != (mcusr & (~(1U<<WDRF))))
	  ||
	  (0U == mcusr) /* Special case for simavr */)
	{
		/* Restart request (invalidate status structure) */
		*p_valid_key = EMUNIT_STATUS_KEY_INVALID;
	}
	MCUCSR = 0U;
	/** @todo WDT may be used before any test is started */
	wdt_disable();
}

void emunit_port_simavr_restart(void)
{
	/* Restart using WDT */
	wdt_enable(WDTO_15MS);
	while(1)
	{
		/* Waiting for reset */
	}
}

void emunit_port_simavr_tc_begin(void)
{

}

void emunit_port_simavr_tc_end(void)
{

}


int main(void)
{
	emunit_run();
}

void before_main(void) __attribute__ ((naked)) __attribute__((used)) __attribute__ ((section (".init3")));

void before_main(void)
{
	emunit_early_init();
}
