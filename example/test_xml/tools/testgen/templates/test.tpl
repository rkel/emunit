{# EMUnit embedded unit test engine - Copyright (C) 2017 Radoslaw Koppel

 This program is free software: you can redistribute it and/or modify
 in under the terms of the GNU General Public license (version 3)
 as published by the Free Software Foundation AND MODIFIED BY the
 EMUnit exception.

 NOTE: The exception was added to the GPL to ensure
 that you can test any kind of software without being
 obligated to release the whole source code under the terms of GPL.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 and the EMUnit license exception along with this program.
 If not, it can be viewed in the original EMunit repository:
 <https://github.com/rkel/emunit>.
 -#}
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
	'_size': size,
	'_mode': mode,
	'_postfix': '_' + mode + size|string,
	'_outfmt': val_ofmt_table[mode],
	'_signes': val_sign_table[mode]}
-%}
{% if tparams._signes -%}
	{% set _ = tparams.update({
		'_min': -(2 ** (tparams._size - 1)),
		'_max':  (2 ** (tparams._size - 1)) - 1
	}) -%}
	{% if tparams._size == 64 -%}
		{# Special fix for warning: integer constant is so large that it is unsigned -#}
		{% set _ = tparams.update({ '_infmt': tparams.get('_outfmt') + 'u' }) -%}
	{% else -%}
		{% set _ = tparams.update({ '_infmt': tparams.get('_outfmt') }) -%}
	{% endif -%}
{% else -%}
	{% set _ = tparams.update({
		'_min': 0,
		'_max': (2 ** (tparams._size)) - 1,
		'_infmt': tparams.get('_outfmt') + 'u'
	}) -%}
{% endif -%}
{{ caller(tparams) }}
{% endfor -%}{# modes -#}
{% endfor -%}{# sizes -#}
{% endmacro -%}
