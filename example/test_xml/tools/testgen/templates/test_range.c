{% import "test.tpl" as test_helper -%}
{{ test_helper.prefix_set('test_range') -}}
/**
 * @file
 * @brief Range assertions xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains range part of the xml output test.
 * @sa emunit_test_xml
 */
#include "test.h"
#include <emunit.h>

/**
 * @brief The macro that creates expected details string
 *
 * This macro creates expected details string for range assertion.
 * To be used with @ref test_expect_fail_assert.
 *
 * @param[in] min Minimum value in expected range
 * @param[in] max Maximum value in expected range
 * @param[in] a   Actual pattern in details pattern
 *
 * @return Range details expected pattern that matches given values.
 *         Also contains format string ("%s") and comma before the pattern.
 */
#define TEST_RANGE_EXP(min, max, a)                \
	"%s",                                          \
	"[[:space:]]*<min>"EMUNIT_STR(min)"</min>"     \
	"[[:space:]]*<max>"EMUNIT_STR(max)"</max>"     \
	"[[:space:]]*<actual>"EMUNIT_STR(a)"</actual>"


static void suite_init(void)
{
	test_expect_sinit_default("test_range_suite");
}

static void suite_cleanup(void)
{
	test_expect_scleanup_default();
}

{% macro infmt(test, val) -%}
{{ test._infmt|format(val) -}}
{% endmacro -%}

{% macro outfmt(test, val) -%}
{{ test._outfmt|format(val) -}}
{% endmacro -%}

{% for test in _base %}
{% set ut_assert = 'UT_ASSERT_RANGE' + test._postfix|upper -%}

{{ test_helper.func(test._postfix + '_bottom')}}
{
	{{ test_helper.passed_list(ut_assert, test._passed ) }}
	{{ ut_assert }}({{ test._min }}, {{ test._max }}, {{ test._min }});
	{{ ut_assert }}({{ test._min }}, {{ test._max }}, {{ test._min }} + {{ infmt(test, 1) }});
	{{ ut_assert }}({{ test._min }}, {{ test._max }}, {{ test._max }});
	{{ ut_assert }}({{ test._min }}, {{ test._max }}, {{ test._max }} - {{ infmt(test, 1) }});
	{{ ut_assert }}({{ test._min }}, {{ test._max }}, {{ infmt(test, ((test._max)|int(0, 0) + (test._min)|int(0, 0)) / 2) }});
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ test._min }}, {{ test._max }}, {{ outfmt(test, ((test._min)|int(0, 0))-1) }})
		);
	{{ ut_assert }}({{ test._min }}, {{ test._max }}, {{ test._min }} - {{ infmt(test, 1) }});
}

{{ test_helper.func(test._postfix + '_top')}}
{
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ test._min }}, {{ test._max }}, {{ outfmt(test, ((test._max)|int(0, 0))+1) }})
		);
	{{ ut_assert }}({{ test._min }}, {{ test._max }}, {{ test._max }} + {{ infmt(test, 1) }});
}

{{ test_helper.func(test._postfix + '_outrange_bottom')}}
{
	/* Should pass */
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, {{ test._min }});
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, {{ test._max }});
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, {{ test._min }} - {{ infmt(test, 1) }});
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, {{ test._max }} + {{ infmt(test, 1) }});

	{{ ut_assert }}({{ test._max }}, {{ test._min }}, EMUNIT_{{ test._val_type|upper }}NUM_MIN);
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, EMUNIT_{{ test._val_type|upper }}NUM_MAX);
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ test._max }}, {{ test._min }}, {{ outfmt(test, ((test._min)|int(0, 0))+1) }})
		);
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, {{ test._min }} + {{ infmt(test, 1) }});
}

{{ test_helper.func(test._postfix + '_outrange_top')}}
{
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ test._max }}, {{ test._min }}, {{ outfmt(test, ((test._max)|int(0, 0))-1) }})
		);
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, {{ test._max }} - {{ infmt(test, 1) }});
}

{{ test_helper.func(test._postfix + '_outrange_middle')}}
{
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ test._max }}, {{ test._min }}, {{ outfmt(test, ((test._max)|int(0, 0) + (test._min)|int(0, 0)) / 2) }})
		);
	{{ ut_assert }}({{ test._max }}, {{ test._min }}, {{ infmt(test, ((test._max)|int(0, 0) + (test._min)|int(0, 0)) / 2) }});
}

{{ test_helper.func(test._postfix + '_msg')}}
{
	{{ ut_assert }}_MSG({{ test._min }}, {{ test._max }}, {{ test._min }}, "Dummy message");
	{{ ut_assert }}_MSG({{ test._min }}, {{ test._max }}, {{ test._min }} + {{ infmt(test, 1) }}, "Dummy message");
	{{ ut_assert }}_MSG({{ test._min }}, {{ test._max }}, {{ test._max }}, "Dummy message");
	{{ ut_assert }}_MSG({{ test._min }}, {{ test._max }}, {{ test._max }} - {{ infmt(test, 1) }}, "Dummy message");
	{{ ut_assert }}_MSG({{ test._min }}, {{ test._max }}, {{ infmt(test, ((test._max)|int(0, 0) + (test._min)|int(0, 0)) / 2) }}, "Dummy message");
	/* Should fail*/
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		"Message for {{ test._postfix }}",
		TEST_RANGE_EXP({{ test._min }}, {{ test._max }}, {{ outfmt(test, ((test._min)|int(0, 0))-1) }})
		);
	{{ ut_assert }}_MSG({{ test._min }}, {{ test._max }}, {{ test._min }} - {{ infmt(test, 1) }}, "Message for %s", "{{ test._postfix }}");
}
{% endfor -%}{# _base -#}


{% call(test) test_helper.each_test(_sizes, _modes) -%}
{% set ut_assert = 'UT_ASSERT_RANGE' + test._postfix|upper -%}

{%- macro call_assert(min, max, actual) -%}
	{{ ut_assert }}{%- if varargs|length > 0 -%} _MSG {%- endif -%}
	( {{- infmt(test, min) }}, {{ infmt(test, max) }}, {{ infmt(test, actual) }}
	{%- if varargs|length > 0 %}, {{ varargs|join(', ') }} {% endif -%} );{# -#}
{%- endmacro -%}


{{ test_helper.func(test._postfix + '_borders_passed') }}
{
	test_expect_success_test();
	{{ call_assert(test._min, test._max, test._min) }}
	{{ call_assert(test._min, test._max, test._max) }}
	{{ call_assert(test._max, test._min, test._min) }}
	{{ call_assert(test._max, test._min, test._max) }}
	{{ call_assert(test._min, test._max, ((test._max + test._min) / 2 ) ) }}
}

{{ test_helper.func(test._postfix + '_borders_passed_msg') }}
{
	test_expect_success_test();
	{{ call_assert(test._min, test._max, test._min, '"Dummy message"') }}
	{{ call_assert(test._min, test._max, test._max, '"Dummy message"') }}
	{{ call_assert(test._max, test._min, test._min, '"Dummy message"') }}
	{{ call_assert(test._max, test._min, test._max, '"Dummy message"') }}
	{{ call_assert(test._min, test._max, ((test._max + test._min) / 2 ), '"Dummy message"') }}
}

{{ test_helper.func(test._postfix + '_bottom') }}
{
	/* Should pass */
	{{ call_assert(test._min, test._max, test._min) }}
	{{ call_assert(test._min, test._max, test._max) }}
	{{ call_assert(test._min, test._max, ((test._max + test._min) / 2 ) ) }}
	{{ call_assert(test._min + 1, test._max, test._min + 1) }}

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ outfmt(test, test._min + 1) }}, {{ outfmt(test, test._max) }}, {{ outfmt(test, test._min) }})
		);
	{{ call_assert(test._min + 1, test._max, test._min) }}
}

{{ test_helper.func(test._postfix + '_top') }}
{
	/* Should pass */
	{{ call_assert(test._min, test._max, test._min) }}
	{{ call_assert(test._min, test._max, test._max) }}
	{{ call_assert(test._min, test._max, ((test._max + test._min) / 2 ) ) }}
	{{ call_assert(test._min, test._max - 1, test._max) }}

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ outfmt(test, test._min) }}, {{ outfmt(test, test._max - 1) }}, {{ outfmt(test, test._max) }})
		);
	{{ call_assert(test._min, test._max - 1, test._max) }}
}

{{ test_helper.func(test._postfix + '_outrange_bottom') }}
{
	/* Should pass */
	{{ call_assert(test._max, test._min, test._min) }}
	{{ call_assert(test._max, test._min, test._max) }}
	{{ call_assert(test._max - 2, test._min + 2, test._min) }}
	{{ call_assert(test._max - 2, test._min + 2, test._max) }}
	{{ call_assert(test._max - 2, test._min + 2, test._min + 2) }}
	{{ call_assert(test._max - 2, test._min + 2, test._max - 2) }}

	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ outfmt(test, test._max - 2) }}, {{ outfmt(test, test._min + 2) }}, {{ outfmt(test, test._min + 3) }})
		);
	{{ call_assert(test._max - 2, test._min + 2, test._min + 3) }}
}

{{ test_helper.func(test._postfix + '_outrange_top') }}
{
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ outfmt(test, test._max - 2) }}, {{ outfmt(test, test._min + 2) }}, {{ outfmt(test, test._max - 3) }})
		);
	{{ call_assert(test._max - 2, test._min + 2, test._max - 3) }}
}

{{ test_helper.func(test._postfix + '_outrange_middle') }}
{
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		NULL,
		TEST_RANGE_EXP({{ outfmt(test, test._max) }}, {{ outfmt(test, test._min) }}, {{ outfmt(test, (test._max + test._min) / 2) }})
		);
	{{ call_assert(test._max, test._min, (test._max + test._min) / 2) }}
}

{{ test_helper.func(test._postfix + '_bottom_msg') }}
{
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		"Message for {{ test._postfix }}_bottom",
		TEST_RANGE_EXP({{ outfmt(test, test._min + 10) }}, {{ outfmt(test, test._max - 10) }}, {{ outfmt(test, test._min + 9) }})
		);
	{{ call_assert(test._min + 10, test._max - 10, test._min + 9, '"Message for %s"', '"' + test._postfix + '_bottom"') }}
}

{{ test_helper.func(test._postfix + '_top_msg') }}
{
	/* Should fail */
	test_expect_fail_assert_here(
		TEST_STR_ID_ANY,
		"RANGE",
		"Message for {{ test._postfix }}_top",
		TEST_RANGE_EXP({{ outfmt(test, test._min + 10) }}, {{ outfmt(test, test._max - 10) }}, {{ outfmt(test, test._max - 9) }})
		);
	{{ call_assert(test._min + 10, test._max - 10, test._max - 9, '"Message for %s"', '"' + test._postfix + '_top"') }}
}
{% endcall -%}

UT_DESC_TS_BEGIN(test_range_suite, suite_init, suite_cleanup, NULL, NULL)
{{- test_helper.suite_desc() }}
UT_DESC_TS_END();
