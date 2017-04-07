{% import "test.tpl" as test_helper -%}
{{ test_helper.prefix_set('test_delta') -}}
/**
 * @file
 * @brief Delta assertions xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains delta part of the xml output test.
 * @sa emunit_test_xml
 */
#include "test.h"
#include <emunit.h>

/**
 * @brief The macro that creates expected details string
 *
 * This macro creates expected details string for delta assertion.
 * To be used with @ref test_expect_fail_assert.
 *
 * @param[in] d Delta value in details pattern
 * @param[in] e Expected value in details pattern
 * @param[in] a Actual pattern in details pattern
 *
 * @return Delta details expected pattern that matches given values.
 *         Also contains format string ("%s") and comma before the pattern.
 */
#define TEST_DELTA_EXP(d, e, a)                        \
	"%s",                                              \
	"[[:space:]]*<delta>"EMUNIT_STR(d)"</delta>"       \
	"[[:space:]]*<expected>"EMUNIT_STR(e)"</expected>" \
	"[[:space:]]*<actual>"EMUNIT_STR(a)"</actual>"

static void suite_init(void)
{
	test_expect_sinit_default("test_delta_suite");
}

static void suite_cleanup(void)
{
	test_expect_scleanup_default();
}

{% for test in _base %}
{% set ut_assert = 'UT_ASSERT_DELTA' + test._postfix|upper -%}
{{ test_helper.func(test._postfix + '_bottom')}}
{
	{{ test_helper.passed_list(ut_assert, test._passed ) }}
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._delta }}, {{ test._expected }}, {{ test._fail_bottom }})
		);
	{{ ut_assert }}({{ test._delta }}, {{ test._expected }}, {{ test._fail_bottom }});
}

{{ test_helper.func(test._postfix + '_top')}}
{
	{{ test_helper.passed_list(ut_assert, test._passed ) }}
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._delta }}, {{ test._expected }}, {{ test._fail_top }})
		);
	{{ ut_assert }}({{ test._delta }}, {{ test._expected }}, {{ test._fail_top }});
}

{{ test_helper.func(test._postfix + '_msg')}}
{
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		"Message for {{ test._postfix }}",
		TEST_DELTA_EXP({{ test._delta }}, {{ test._expected }}, {{ test._fail_top }})
		);
	{{ ut_assert }}_MSG({{ test._delta }}, {{ test._expected }}, {{ test._fail_top }}, "Message for %s", "{{ test._postfix }}");
}

{{ test_helper.func(test._postfix + '_big_delta')}}
{
	test_expect_success_test();
	{{ ut_assert }}(EMUNIT_UNUM_MAX, EMUNIT_{{ test._val_type|upper }}NUM_MIN, EMUNIT_{{ test._val_type|upper }}NUM_MAX);
	{{ ut_assert }}(EMUNIT_UNUM_MAX, EMUNIT_{{ test._val_type|upper }}NUM_MAX, EMUNIT_{{ test._val_type|upper }}NUM_MIN);
}
{% endfor -%}{# _base -#}


{% call(test) test_helper.each_test(_sizes, _modes) %}
{% set ut_assert = 'UT_ASSERT_DELTA' + test._postfix|upper -%}
{% set delta = _delta_default -%}
{% set delta_max = (2 ** (test._size)) - 1 -%}
{# Macro that prints the value with correct postfix #}
{% macro ut_val(test, val) -%}
{{ test._infmt|format(val) -}}
{% endmacro -%}
{{ test_helper.func(test._postfix + '_bottom')}}
{
	{%- set expected = test._min + delta -%}
	{%- set fail_actual = expected + delta + 1 -%}

	/* Should pass */
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected-delta) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected+delta) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._outfmt|format(delta) }}, {{ test._outfmt|format(expected) }}, {{ test._outfmt|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, fail_actual) }});
}

/* Test what happens if expected with delta underruns minimum value */
{{ test_helper.func(test._postfix + '_underrun')}}
{
	{%- set expected = test._min + (delta/2)|int -%}
	{%- set fail_actual = test._max -%}

	/* Should pass */
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, test._min) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected+delta) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._outfmt|format(delta) }}, {{ test._outfmt|format(expected) }}, {{ test._outfmt|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, fail_actual) }});
}

{{ test_helper.func(test._postfix + '_top')}}
{
	{%- set expected = test._max - delta -%}
	{%- set fail_actual = expected - delta - 1 -%}

	/* Should pass */
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected-delta) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected+delta) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._outfmt|format(delta) }}, {{ test._outfmt|format(expected) }}, {{ test._outfmt|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, fail_actual) }});
}

/* Test what happens if expected with delta overruns maximum value */
{{ test_helper.func(test._postfix + '_overrun')}}
{
	{%- set expected = test._max - (delta/2)|int -%}
	{%- set fail_actual = test._min -%}
	/* Should pass */
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, test._max) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected-delta) }});
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._outfmt|format(delta) }}, {{ test._outfmt|format(expected) }}, {{ test._outfmt|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, fail_actual) }});
}

/* Test message version */
{{ test_helper.func(test._postfix + '_msg')}}
{
	{%- set expected = test._max -%}
	{%- set fail_actual = test._min -%}

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		"Message {{ test._postfix }}",
		TEST_DELTA_EXP({{ test._outfmt|format(delta) }}, {{ test._outfmt|format(expected) }}, {{ test._outfmt|format(fail_actual) }})
		);
	{{ ut_assert }}_MSG({{ ut_val(test, delta)}}, {{ ut_val(test, expected) }}, {{ ut_val(test, fail_actual) }}, "Message %s", "{{ test._postfix }}");
}

{{ test_helper.func(test._postfix + '_only_max_fail')}}
{
	/* Should pass */
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._min) }}, {{ ut_val(test, test._max-1) }});
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._min+1) }}, {{ ut_val(test, test._max) }});
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._max) }}, {{ ut_val(test, test._min+1) }});
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._max-1) }}, {{ ut_val(test, test._min) }});
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._outfmt|format(delta_max-1) }}, {{ test._outfmt|format(test._min) }}, {{ test._outfmt|format(test._max) }})
		);
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._min) }}, {{ ut_val(test, test._max) }});
}

{{ test_helper.func(test._postfix + '_only_min_fail')}}
{
	/* Should pass */
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._min) }}, {{ ut_val(test, test._max-1) }});
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._min+1) }}, {{ ut_val(test, test._max) }});
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._max) }}, {{ ut_val(test, test._min+1) }});
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._max-1) }}, {{ ut_val(test, test._min) }});
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ test._outfmt|format(delta_max-1) }}, {{ test._outfmt|format(test._max) }}, {{ test._outfmt|format(test._min) }})
		);
	{{ ut_assert }}({{ ut_val(test, delta_max-1) }}, {{ ut_val(test, test._max) }}, {{ ut_val(test, test._min) }});
}
{% endcall -%}

UT_DESC_TS_BEGIN(test_delta_suite, suite_init, suite_cleanup, NULL, NULL)
{{- test_helper.suite_desc() }}
UT_DESC_TS_END();
