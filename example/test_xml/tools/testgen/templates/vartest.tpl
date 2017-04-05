{# Macro used to iterate through all possible variations of assertion size and mode -#}

{%- macro for_each_smtest(sizes, modes) %}
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
