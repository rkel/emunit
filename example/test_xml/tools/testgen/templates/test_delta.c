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

{% for size in _sizes -%}
{% for mode in _modes -%}
{% set postfix = '_' + mode + size|string -%}
{% set ut_assert = 'UT_ASSERT_DELTA' + postfix|upper -%}
{% set val_format_table = {
	'int' : '%d',
	'uint': '%u',
	'hex' : '0x%x'
	} -%}
{% set val_sign_table = {
	'int' : True,
	'uint': False,
	'hex' : False
	} -%}
{% set current_format = val_format_table[mode] -%}
{% set current_signes = val_sign_table[mode] -%}
{% if current_signes -%}
	{% set val_min = -(2 ** (size - 1)) -%}
	{% set val_max =  (2 ** (size - 1)) - 1 -%}
	{% if size == 64 -%}
		{# Special fix for warning: integer constant is so large that it is unsigned -#}
		{% set val_postfix = 'u' -%}
	{% else -%}
		{% set val_postfix = '' -%}
	{% endif -%}
{% else -%}
	{% set val_min = 0 -%}
	{% set val_max =  (2 ** (size)) - 1 -%}
	{% set val_postfix = 'u' -%}
{% endif -%}
{% set delta = _delta_default -%}
{% set delta_max = (2 ** (size)) - 1 -%}
{# Macro that prints the value with correct postfix #}
{% macro ut_val(val) -%}
{{ current_format|format(val) + val_postfix -}}
{% endmacro -%}
{{ test_helper.func(postfix + '_bottom')}}
{
	{%- set expected = val_min + delta -%}
	{%- set fail_actual = expected + delta + 1 -%}

	/* Should pass */
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected-delta) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected+delta) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ current_format|format(delta) }}, {{ current_format|format(expected) }}, {{ current_format|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(fail_actual) }});
}

/* Test what happens if expected with delta underruns minimum value */
{{ test_helper.func(postfix + '_underrun')}}
{
	{%- set expected = val_min + (delta/2)|int -%}
	{%- set fail_actual = val_max -%}

	/* Should pass */
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(val_min) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected+delta) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ current_format|format(delta) }}, {{ current_format|format(expected) }}, {{ current_format|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(fail_actual) }});
}

{{ test_helper.func(postfix + '_top')}}
{
	{%- set expected = val_max - delta -%}
	{%- set fail_actual = expected - delta - 1 -%}

	/* Should pass */
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected-delta) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected+delta) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ current_format|format(delta) }}, {{ current_format|format(expected) }}, {{ current_format|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(fail_actual) }});
}

/* Test what happens if expected with delta overruns maximum value */
{{ test_helper.func(postfix + '_overrun')}}
{
	{%- set expected = val_max - (delta/2)|int -%}
	{%- set fail_actual = val_min -%}
	/* Should pass */
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(val_max) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected-delta) }});
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(expected) }});

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ current_format|format(delta) }}, {{ current_format|format(expected) }}, {{ current_format|format(fail_actual) }})
		);
	{{ ut_assert }}({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(fail_actual) }});
}

/* Test message version */
{{ test_helper.func(postfix + '_msg')}}
{
	{%- set expected = val_max -%}
	{%- set fail_actual = val_min -%}

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		"Message {{ postfix }}",
		TEST_DELTA_EXP({{ current_format|format(delta) }}, {{ current_format|format(expected) }}, {{ current_format|format(fail_actual) }})
		);
	{{ ut_assert }}_MSG({{ ut_val(delta)}}, {{ ut_val(expected) }}, {{ ut_val(fail_actual) }}, "Message %s", "{{ postfix }}");
}

{{ test_helper.func(postfix + '_only_max_fail')}}
{
	/* Should pass */
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_min) }}, {{ ut_val(val_max-1) }});
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_min+1) }}, {{ ut_val(val_max) }});
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_max) }}, {{ ut_val(val_min+1) }});
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_max-1) }}, {{ ut_val(val_min) }});
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ current_format|format(delta_max-1) }}, {{ current_format|format(val_min) }}, {{ current_format|format(val_max) }})
		);
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_min) }}, {{ ut_val(val_max) }});
}

{{ test_helper.func(postfix + '_only_min_fail')}}
{
	/* Should pass */
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_min) }}, {{ ut_val(val_max-1) }});
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_min+1) }}, {{ ut_val(val_max) }});
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_max) }}, {{ ut_val(val_min+1) }});
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_max-1) }}, {{ ut_val(val_min) }});
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"DELTA",
		NULL,
		TEST_DELTA_EXP({{ current_format|format(delta_max-1) }}, {{ current_format|format(val_max) }}, {{ current_format|format(val_min) }})
		);
	{{ ut_assert }}({{ ut_val(delta_max-1) }}, {{ ut_val(val_max) }}, {{ ut_val(val_min) }});
}
{% endfor -%}{# _modes -#}
{% endfor -%}{# _sizes #}

UT_DESC_TS_BEGIN(test_delta_suite, suite_init, suite_cleanup, NULL, NULL)
{{- test_helper.suite_desc() }}
UT_DESC_TS_END();
