# EMUnit main makefile

# EMUnit sources
EMUNIT_SRC = \
  emunit.c \
  emunit_display.c \
  emunit_port.c

# Add EMUNIT sources to search path
SEARCHPATH = $(EMUNIT_DIR)

# Very simple, temporary solution for EMUnit sources
SRC += $(EMUNIT_SRC)

# Selected port building
include $(EMUNIT_DIR)/port/$(EMUNIT_PORT)/emunit_port_$(EMUNIT_PORT).mk
