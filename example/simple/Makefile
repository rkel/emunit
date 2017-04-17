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

# Simple test makefile
EMUNIT_DIR = ../..
EMUNIT_PORT = pcstdout
EXTRAINCDIRS = c:/code/simavr_dist/include

TARGET = $(EMUNIT_PORT)_simple
SRC = main.c
F_CPU = 4000000

# Include building system
include $(EMUNIT_DIR)/emunit.mk
