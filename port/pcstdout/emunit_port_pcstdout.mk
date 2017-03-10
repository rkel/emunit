# EMUnit PC target makefile
#
# Author: Radoslaw Koppel

# Highly based on:
# WinAVR Makefile Template written by Eric B. Weddington, Jorg Wunsch, et al.
#
# On command line:
# make all
# make clean
# make run
#
# Values that requires to be set before including this makefile:

# TARGET     - the name of output file without extension
# SRC        - sources of the unit test files (not including emunit engine)
# EMUNIT_DIR - the root directory with EMUnit enging
#
# The variables that may be set but not required:
#
# EXTRAINCDIRS - additional include directories
#
# ---------------------------------------------------------------------------

# Object directory
OBJDIR = ./OBJ/$(TARGET)

# Directory for generated outputs
OUTDIR = ./OUT

# Additional paths to look soruces for
# Leave it empty if there is not other source directory than current
vpath %.c $(SEARCHPATH)
vpath %.cpp $(SEARCHPATH)
vpath %.S $(SEARCHPATH)

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS += $(SEARCHPATH)

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here for C sources
CDEFS += 

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS += $(CDEFS)
CFLAGS += $(ALLDEFS)
CFLAGS += -Wa,-adhlns=$(@:%.o=%.lst) # ! listing in the target folder, not in source folder
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -adhlns:   create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex 
#       dump that will be displayed for a given single line of source input.
ASFLAGS = $(ADEFS) $(ALLDEFS) -Wa,-adhlns=$(@:%.o=%.lst),-gstabs,--listing-cont-lines=100


MATH_LIB = -lm


# List any extra directories to look for libraries here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRALIBDIRS += 



#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS += -Wl,-Map=$(OUTDIR)/$(TARGET).map,--cref
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += $(MATH_LIB)
LDFLAGS += -Wl,-gc-sections 

# Define programs and commands.
SHELL = sh
CC = gcc
OBJCOPY = objcopy
OBJDUMP = objdump
SIZE = size
AR = ar rcs
NM = nm
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
WINSHELL = cmd

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:


# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o) 

# Define all listing files.
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF $(OBJDIR)/.dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS   = -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -I. -x c++ $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS  = -I. -x assembler-with-cpp $(ASFLAGS) $(GENDEPFLAGS)


# Default target
all: begin gccversion build end

# Build the output
build: exe


exe: $(OUTDIR)/$(TARGET).exe
lss: $(OUTDIR)/$(TARGET).lss
sym: $(OUTDIR)/$(TARGET).sym


begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)

gccversion : 
	@$(CC) --version

$(OUTDIR)/$(TARGET).exe: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
$(OUTDIR)/%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C++ source files.
$(OUTDIR)/%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Create preprocessed source for use in sending a bug report.
$(OUTDIR)/%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@ 


# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(OUTDIR)/$(TARGET).exe
	$(REMOVE) $(OUTDIR)/$(TARGET).map
	$(REMOVE) $(OUTDIR)/$(TARGET).sym
	$(REMOVE) $(OUTDIR)/$(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:%.c=$(OUTDIR)/%.s)
	$(REMOVE) $(SRC:%.c=$(OBJDIR)/.dep/%.d)
	$(REMOVE) $(SRC:%.c=$(OUTDIR)/%.i)
	$(REMOVEDIR) $(OBJDIR)/.dep

# Create object files directory
$(shell mkdir -p $(OBJDIR) > /dev/null 2>&1)

# Create output files directory
$(shell mkdir -p $(OUTDIR) > /dev/null 2>&1)

# Include the dependency files.
-include $(shell mkdir -p $(OBJDIR)/.dep > /dev/null 2>&1) $(wildcard $(OBJDIR)/.dep/*)

# Makefile dependency
$(OBJ) : $(filter-out %.d,$(MAKEFILE_LIST))


# Listing of phony targets.
.PHONY : run all begin finish end sizebefore sizeafter gccversion \
build exe lss sym \
clean clean_list
