/**
 * @file
 * @brief EMUnit XML presentation module implementation
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2016
 *
 *
 * @sa emunit_display_xml_group
 */
#include <inttypes.h>
#include <avr/pgmspace.h>
#include "../emunit.h"
#include "emunit_display_xml.h"

#define NEWLINE EMUNIT_CONF_DISPLAY_NL

/**
 * @brief Internal display types
 *
 * This enumerator expands the @ref emunit_numtypes_t adding some
 * displaying types used internally.
 */
typedef enum
{
	EMUNIT_DISPLAY_TYPE_PSTR = EMUNIT_NUMTYPE_LAST, /**< String in program memory */

}emunit_display_types_t;

/**
 * @brief Internal value display
 *
 * Union that connects numeric types and internal types to for displaying
 */
typedef union
{
	emunit_num_t num;

}emunit_display_val_t;

static void emunit_display_xml_putc_puts_P(void (*fn_putc)(char c), char const __flash * p_str)
{
	char c;
	while('\0' != (c = *p_str++))
	{
		fn_putc(c);
	}
}

static void emunit_display_xml_putc_ent(void (*fn_putc)(char c), char c)
{
	switch(c)
	{
	case '<':
		emunit_display_xml_putc_puts_P(fn_putc, EMUNIT_FLASHSTR("&lt;"));
		break;
	case '>':
		emunit_display_xml_putc_puts_P(fn_putc, EMUNIT_FLASHSTR("&gt;"));
		break;
	case '&':
		emunit_display_xml_putc_puts_P(fn_putc, EMUNIT_FLASHSTR("&amp;"));
		break;
	default:
		fn_putc(c);
		break;
	}
}

static void emunit_display_xml_puts_ent(void (*fn_putc)(char c), const __memx char * p_str)
{
	char c;
	while('\0' != (c = *p_str++))
	{
		emunit_display_xml_putc_ent(fn_putc, c);
	}
}

static void emunit_display_xml_failed_header(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * p_str_type)
{
	emunit_display_printf(
		EMUNIT_FLASHSTR(
			"\t\t\t<failure type=\%"PRIsPGM" id=\"%u\">" NEWLINE
			"\t\t\t\t<file>%"PRIsPGM"</file>"            NEWLINE
			"\t\t\t\t<line>%u</line>"                    NEWLINE
		),
		p_str_type,
		emunit_tc_failed_get() + 1,
		p_head->p_file,
		p_head->line
	);
	if(NULL != p_head->p_msg)
	{
		emunit_display_printf(
			EMUNIT_FLASHSTR("\t\t\t\t<msg>%"PRIsPGM"</msg>" NEWLINE),
			p_head->p_msg
		);
	}
	emunit_display_puts(EMUNIT_FLASHSTR("\t\t\t\t<details>" NEWLINE));
}

static void emunit_display_xml_failed_footer(const __flash emunit_assert_head_t * p_head)
{
	(void)p_head;
	emunit_display_puts(
		EMUNIT_FLASHSTR(
			"\t\t\t\t</details>" NEWLINE
			"\t\t\t</failure>" NEWLINE
		)
	);
}

/**
 * @brief Auxiliary function to print the value
 *
 * Function that prints the value in a format <name>val</name>.
 *
 * @param type   Type of the value to be printed.
 * @param p_name The name of the value.
 * @param val    Value itself.
 */
static void emunit_display_xml_value(
	emunit_numtypes_t type,
	const __flash char * p_name,
	emunit_num_t val)
{
	emunit_display_printf(
		EMUNIT_FLASHSTR("\t\t\t\t\t<%"PRIsPGM">"),
		p_name
	);

#define EMUNIT_DISPLAY_XML_CASES(size)                                                             \
	case EMUNIT_CN2(EMUNIT_NUMTYPE_U, size):                                                       \
		emunit_display_printf(EMUNIT_FLASHSTR("%"EMUNIT_CN2(PRIu, size)), (EMUNIT_CN3(uint, size, _t))val.u); \
		break;                                                                                     \
	case EMUNIT_CN2(EMUNIT_NUMTYPE_S, size):                                                       \
		emunit_display_printf(EMUNIT_FLASHSTR("%"EMUNIT_CN2(PRId, size)), (EMUNIT_CN3(int, size, _t))val.s);  \
		break;                                                                                     \
	case EMUNIT_CN2(EMUNIT_NUMTYPE_X, size):                                                       \
		emunit_display_printf(EMUNIT_FLASHSTR("%"EMUNIT_CN2(PRIx, size)), (EMUNIT_CN3(uint, size, _t))val.u); \
		break

	switch(type)
	{
	case EMUNIT_NUMTYPE_BOOL:
		emunit_display_puts(
			((bool)val.u) ? EMUNIT_FLASHSTR("true") : EMUNIT_FLASHSTR("false")
		);
		break;
	EMUNIT_DISPLAY_XML_CASES(8);
	EMUNIT_DISPLAY_XML_CASES(16);

#if (EMUNIT_CONF_NUMBER_SIZE > 16) || defined(__DOXYGEN__)
	EMUNIT_DISPLAY_XML_CASES(32);
#endif
#if (EMUNIT_CONF_NUMBER_SIZE > 32) || defined(__DOXYGEN__)
	EMUNIT_DISPLAY_XML_CASES(64);
#endif
	default:
		EMUNIT_IASSERT_MGS(false, "ERROR: Wrong value type");
		break;
	}

	emunit_display_printf(
		EMUNIT_FLASHSTR("</%"PRIsPGM">" NEWLINE),
		p_name
	);
}


void emunit_display_xml_show_panic(
	const __flash char * str_file,
	const __flash char * str_line,
	const __flash char * str_msg)
{
	emunit_display_panic_puts(NEWLINE"<panic>"NEWLINE);
	emunit_display_panic_puts("\t<file>");
	emunit_display_panic_puts(str_file);
	emunit_display_panic_puts("</file>"NEWLINE);

	emunit_display_panic_puts("\t<line>");
	emunit_display_panic_puts(str_line);
	emunit_display_panic_puts("</line>"NEWLINE);

	emunit_display_panic_puts("\t<msg>");
	emunit_display_xml_puts_ent(emunit_display_panic_putc, str_msg);
	emunit_display_panic_puts("</msg>"NEWLINE);
}

void emunit_display_xml_test_start(void)
{
	emunit_display_printf(
		EMUNIT_FLASHSTR(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" NEWLINE
			"<test name=\"%"PRIsPGM"\">"                 NEWLINE
		),
		emunit_test_name_get()
	);
}

void emunit_display_xml_test_end(void)
{
	emunit_display_printf(
		EMUNIT_FLASHSTR(
			"\t<testsumary>"            NEWLINE
			"\t\t<testsute-stat>"       NEWLINE
			"\t\t\t<total>%u</total>"   NEWLINE
			"\t\t\t<passed>%u</total>"  NEWLINE
			"\t\t\t<failed>%u</failed>" NEWLINE
			"\t\t</testsuite-stat>"     NEWLINE
			"\t\t<testcase-stat>"       NEWLINE
			"\t\t\t<total>%u</total>"   NEWLINE
			"\t\t\t<passed>%u</total>"  NEWLINE
			"\t\t\t<failed>%u</failed>" NEWLINE
			"\t\t</testcase-stat>"      NEWLINE
			"\t</testsummary>"          NEWLINE
			"</test>"                   NEWLINE
		),
		emunit_ts_total_count(),
		emunit_ts_passed_get(),
		emunit_ts_failed_get(),
		emunit_tc_total_count(),
		emunit_tc_passed_get(),
		emunit_tc_failed_get()
	);
}

void emunit_display_xml_ts_start(void)
{
	emunit_display_printf(
		EMUNIT_FLASHSTR(
			"\t<testsuite name=\"%"PRIsPGM"\">" NEWLINE
		),
		emunit_ts_name_get(emunit_ts_current_index_get())
	);
}

void emunit_display_xml_ts_end(void)
{
	emunit_display_puts(
		EMUNIT_FLASHSTR("\t</testsuite>" NEWLINE)
	);
}

void emunit_display_xml_tc_start(void)
{
	emunit_display_printf(
		EMUNIT_FLASHSTR("\t\t<testcase name=\"%"PRIsPGM"\">" NEWLINE),
		emunit_tc_name_get(emunit_ts_current_index_get(), emunit_tc_current_index_get())
	);
}

void emunit_display_xml_tc_end(void)
{
	emunit_display_puts(
		EMUNIT_FLASHSTR("\t\t</testcase>" NEWLINE)
	);
}

void emunit_display_xml_failed_assert(
	const __flash emunit_assert_head_t * p_head,
	const __flash char * str_expr)
{
	emunit_display_xml_failed_header(p_head, EMUNIT_FLASHSTR("ASSERT"));

	emunit_display_printf(
		EMUNIT_FLASHSTR("\t\t\t\t\t<expression>%"PRIsPGM"</expression>" NEWLINE),
		str_expr
	);
	emunit_display_xml_failed_footer(p_head);
}

void emunit_display_xml_failed_equal(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t expected,
	emunit_num_t actual)
{
	emunit_display_xml_failed_header(p_head, EMUNIT_FLASHSTR("EQUAL"));
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("expected"), expected);
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("actual"),   actual);
	emunit_display_xml_failed_footer(p_head);
}

void emunit_display_xml_failed_range(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t min,
	emunit_num_t max,
	emunit_num_t actual)
{
	emunit_display_xml_failed_header(p_head, EMUNIT_FLASHSTR("RANGE"));
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("min"),    min);
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("max"),    max);
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("actual"), actual);
	emunit_display_xml_failed_footer(p_head);
}

void emunit_display_xml_failed_delta(
	const __flash emunit_assert_head_t * p_head,
	emunit_num_t delta,
	emunit_num_t expected,
	emunit_num_t actual)
{
	emunit_display_xml_failed_header(p_head, EMUNIT_FLASHSTR("DELTA"));
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("expected"), expected);
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("delta"),    delta);
	emunit_display_xml_value(p_head->numtype, EMUNIT_FLASHSTR("actual"),   actual);
	emunit_display_xml_failed_footer(p_head);
}

void emunit_display_xml_failed_str(
	const __flash emunit_assert_head_t * p_head,
	const __memx char * str_expected,
	const __memx char * str_actual,
	size_t err_pos)
{
	emunit_display_xml_failed_header(p_head, EMUNIT_FLASHSTR("STRING"));
	/** @todo Display details of a string */
	emunit_display_xml_failed_footer(p_head);
}

void emunit_display_xml_failed_array(
	const __flash emunit_assert_array_head_t * p_head,
	const emunit_assert_array_cmp_t            p_cmp,
	size_t                                     err_pos)
{
	emunit_display_xml_failed_header(&p_head->head, EMUNIT_FLASHSTR("ARRAY"));
	/** @todo Display details of an array */
	emunit_display_xml_failed_footer(&p_head->head);
}
