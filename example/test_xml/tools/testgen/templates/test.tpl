{% set _list = [] -%}
{% set _test_globals = {} -%}

{% macro prefix_set( prefix) -%}
{% set _ = _test_globals.update({'prefix': prefix}) %}
{%- endmacro -%}

{% macro func( postfix ) -%}
{% set fn_name = _test_globals.get('prefix') + postfix -%}
{% if _list.append( fn_name ) -%}{% endif -%}
static void {{ fn_name }}(void)
{%- endmacro -%}

{% macro passed_list( assert_func, values ) -%}
	/* Should pass */
	{%- for v in values %}
	{{ assert_func }}({{ v|join(', ')}});
	{%- endfor -%}
{%- endmacro -%}

{% macro suite_desc() -%}
{%- for test_name in _list %}
	UT_DESC_TC({{ test_name }})
{%- endfor -%}
{% endmacro -%}
