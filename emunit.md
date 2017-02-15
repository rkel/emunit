EUMnit - Embedded Unit test framework    {#mainpage}
=====================================

# Flat directory structure

There is no subdirectories inside emunit directory.
All source and include files are placed in single folder.
This simplify EMUnit usage - just add one directory to search and include path.

The only subdirectory that may be found here is _config_.
This is some default configuration and and normally it should be copied
into the directory of your test.

# Generated files

Some of the source files are generated.
They always have _gen postfix.
All generators are placed in tools directory.
Most of the generators uses json files as for configuration.

# Output format strings

Output format strings simplify supporting any requested output format that
is text based (support for binnary format is quite limited).

To print specified value put it into curly braces:

	emunit_str_assert_equal = "    Expected: {e} Is: {a}\n";

Or another example:

	emunit_str_assert_equal =
		"<error>\n"
		"    <file>{file}</file>\n"
		"    <line>{line}</line>\n"
		"    <expected>{e}</expected>\n"
		"    <actual>{a}</actual>\n
		"</error>\n";
