# EMUnit embedded unit test engine - Copyright (C) 2017 Radoslaw Koppel
#
# This program is free software: you can redistribute it and/or modify
# in under the terms of the GNU General Public license (version 3)
# as published by the Free Software Foundation AND MODIFIED BY the
# EMUnit exception.
#
# NOTE: The exception was added to the GPL to ensure
# that you can test any kind of software without being
# obligated to release the whole source code under the terms of GPL.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# and the EMUnit license exception along with this program.
# If not, it can be viewed in the original EMunit repository:
# <https://github.com/rkel/emunit>.

# EMUnit main makefile

# Define the used port
CFLAGS = -DEMUNIT_CONF_PORT=$(EMUNIT_PORT)

# EMUnit sources
EMUNIT_SRC = \
  emunit.c \
  emunit_display.c \
  emunit_port.c

# Add EMUNIT sources to search path
SEARCHPATH += $(EMUNIT_DIR)

# Very simple, temporary solution for EMUnit sources
SRC += $(EMUNIT_SRC)

# Selected port building
include $(EMUNIT_DIR)/port/$(EMUNIT_PORT)/emunit_port_$(EMUNIT_PORT).mk
