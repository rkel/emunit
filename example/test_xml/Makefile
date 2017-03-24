# Simple test makefile
EMUNIT_DIR = ../..
GEN_DIR = $(OBJDIR)/_gen
EMUNIT_PORT = pctest

TARGET = $(EMUNIT_PORT)_simple
SRC = \
	suites.c \
	main.c \
	test_boolean.c \
	test_str.c \
	test_equal.c

# The name (without extension) of the generated test files
GEN_TESTS = \
	test_delta

#################################################
# Test generator script
TESTGEN = python ./tools/testgen/testgen.py
# The directory with test descriptions (JSON files)
TESTGEN_DESC_DIR = ./tools/testgen/desc
# The directory with test templates
TESTGEN_TEMPLATE_DIR = ./tools/testgen/templates

# Automatically generated test files full patch
GEN_TESTS_FILES = $(GEN_TESTS:%=$(GEN_DIR)/%.c)
# Automatically generates test files file names
GEN_TESTS_FILE_NAMES = $(GEN_TESTS:%=%.c)

# Add file names into sources
SRC += $(GEN_TESTS_FILE_NAMES)
# And the directory with generated test files into search paths
SEARCHPATH += $(GEN_DIR)



# Include building system
include $(EMUNIT_DIR)/emunit.mk

# Generated files have to be processed before build.
# In other case, there would be problem to find them by the make
# before they are generated first time.
# It would be possible if in the SRC full path version of the
# files would be provided, but it would generate other problems.
pre_build: $(GEN_TESTS_FILES)

# Clean generated files glued into cleaning hook
clean_list: clean_gen

# The rule to clean the files
clean_gen:
	$(REMOVE) $(GEN_TESTS_FILES)

# Mark the relation between generated test file and template file
$(GEN_TESTS_FILES) : $(GEN_DIR)/%.c : $(TESTGEN_TEMPLATE_DIR)/%.c

# The rule to build the generated test file
$(GEN_TESTS_FILES) : $(GEN_DIR)/%.c : $(TESTGEN_DESC_DIR)/%.json
	@echo "Generating:" $@
	$(TESTGEN) -c $< -o $@

# Create the directory for generated C files
$(shell mkdir -p $(GEN_DIR) > /dev/null 2>&1)

.PHONY : clean_gen
