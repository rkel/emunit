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

{# Macro used to iterate through all possible variations of assertion size and mode -#}
{%- macro each_test(sizes, modes) %}
{% set val_ofmt_table = {
	'int' : '%d',
	'uint': '%u',
	'hex' : '0x%x'
	} -%}
{% set val_sign_table = {
	'int' : True,
	'uint': False,
	'hex' : False
	} -%}

{% for size in sizes -%}
{% for mode in modes -%}
{% set tparams = {
	'size': size,
	'mode': mode,
	'postfix': '_' + mode + size|string,
	'outfmt': val_ofmt_table[mode],
	'signes': val_sign_table[mode]}
-%}
{% if tparams.signes -%}
	{% set _ = tparams.update({
		'val_min': -(2 ** (tparams.size - 1)),
		'val_max':  (2 ** (tparams.size - 1)) - 1
	})%}
	{% if tparams.size == 64 -%}
		{# Special fix for warning: integer constant is so large that it is unsigned -#}
		{% set _ = tparams.update({ 'infmt': tparams.get('outfmt') + 'u' }) %}
	{% else %}
		{% set _ = tparams.update({ 'infmt': tparams.get('outfmt') }) %}
	{% endif -%}
{% else -%}
	{% set _ = tparams.update({
		'val_min': 0,
		'val_max': (2 ** (tparams.size)) - 1,
		'infmt': tparams.get('outfmt') + 'u'
	})%}
{% endif -%}
{{ caller(tparams) }}
{% endfor -%}{# modes -#}
{% endfor -%}{# sizes #}
{% endmacro -%}
