# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# WinAVR Makefile Template written by Eric B. Weddington, J�rg Wunsch, et al.
#
# Released to the Public Domain
#
# Additional material for this makefile was written by:
# Peter Fleury
# Tim Henigan
# Colin O'Flynn
# Reiner Patommel
# Markus Pfaff
# Sander Pool
# Frederik Rouleau
# Carlos Lamas
# Radoslaw Koppel
#
#----------------------------------------------------------------------------
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make coff = Convert ELF to AVR COFF.
#
# make extcoff = Convert ELF to AVR Extended COFF.
#
# make program = Download the hex file to the device, using avrdude.
#                Please customize the avrdude settings below first!
#
# make debug = Start either simulavr or avarice as specified for debugging, 
#              with avr-gdb or avr-insight as the front end for debugging.
#
# make filename.s = Just compile filename.c into the assembler code only.
#
# make filename.i = Create a preprocessed source file for use in submitting
#                   bug reports to the GCC project.
#
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------
 
# Configure the default target
.DEFAULT_GOAL := all

# We are using atmega128 for simulation
MCU = atmega32

# Output format. (can be srec, ihex, binary)
FORMAT = ihex


# Object files directory
#     To put object files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
OBJDIR = ./OBJ/$(TARGET)
# Directory for all generated ouptuts
#     To put output files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
OUTDIR = ./OUT

#----------




# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = 0


# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = dwarf-2

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

# Place -D or -U options here for All sources
# ALLDEFS += -D__SIMULAVR__=1

# Place -D or -U options here for C sources
CDEFS += -DF_CPU=$(F_CPU)UL -DMCU_NAME=$(MCU)

# Place -D or -U options here for ASM sources
ADEFS += -DF_CPU=$(F_CPU)

# Place -D or -U options here for C++ sources
CPPDEFS += -DF_CPU=$(F_CPU)UL
#CPPDEFS += -D__STDC_LIMIT_MACROS
#CPPDEFS += -D__STDC_CONSTANT_MACROS


#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS += -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += $(ALLDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += -ffunction-sections
CFLAGS += -freg-struct-return
CFLAGS += -Wa,-adhlns=$(@:%.o=%.lst) # ! listing in the target folder, not in source folder
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)


#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS += -g$(DEBUG)
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += $(ALLDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fpack-struct
CPPFLAGS += -fshort-enums
CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
CPPFLAGS += -Wundef
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += -Wa,-adhlns=$(@:%.o=%.lst) # ! listing in the target folder, not in source folder
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
#CPPFLAGS += $(CSTANDARD)

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

#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
#PRINTF_LIB = 
#PRINTF_LIB = $(PRINTF_LIB_MIN)
PRINTF_LIB = $(PRINTF_LIB_FLOAT)


# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
#SCANF_LIB = 
#SCANF_LIB = $(SCANF_LIB_MIN)
SCANF_LIB = $(SCANF_LIB_FLOAT)


MATH_LIB = -lm


# List any extra directories to look for libraries here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRALIBDIRS = 



#---------------- External Memory Options ----------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
# EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =



#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS += -Wl,-Map=$(OUTDIR)/$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
LDFLAGS += -Wl,-gc-sections 
# simavr _mmcu section anachor
LDFLAGS += -Wl,--undefined=_mmcu,--section-start=.mmcu=0x910000
# LDFLAGS += -T ./ld/avr5mod.x


#============================================================================


# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
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
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:
MSC_CREATING_DOC = Creating documentation:




# Define all object files.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o) 

# Define all listing files.
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst) 


# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF $(OBJDIR)/.dep/$(@F).d


# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. -x c++ $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS) $(GENDEPFLAGS)



# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT = $(OBJCOPY) --debugging
COFFCONVERT += --change-section-address .data-0x800000
COFFCONVERT += --change-section-address .bss-0x800000
COFFCONVERT += --change-section-address .noinit-0x800000
COFFCONVERT += --change-section-address .eeprom-0x810000

# Default target.
all: begin gccversion sizebefore build sizeafter end

# Change the build target to build a HEX file or a library.
# build: elf hex bin eep eep_bin lss sym
build: elf hex lss sym
#build: lib

# Run the test
run: simulavr_run


elf: $(OUTDIR)/$(TARGET).elf
hex: $(OUTDIR)/$(TARGET).hex
bin: $(OUTDIR)/$(TARGET).bin
eep: $(OUTDIR)/$(TARGET).eep
eep_bin: $(OUTDIR)/$(TARGET)_eep.bin
lss: $(OUTDIR)/$(TARGET).lss
sym: $(OUTDIR)/$(TARGET).sym
LIBNAME=$(OUTDIR)/lib$(TARGET).a
lib: $(OUTDIR)/$(LIBNAME)


# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(OUTDIR)/$(TARGET).elf

sizebefore:
	@if test -f $(OUTDIR)/$(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	2>/dev/null; echo; fi

sizeafter:
	@if test -f $(OUTDIR)/$(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; echo; fi



# Display compiler version information.
gccversion : 
	@$(CC) --version

# Generate documentation
make_doc:
	echo $(MSC_CREATING_DOC)
	
	echo -e "# Ten plik należy wczytać z poziomu katalogu głównego, w którym znajduje się plik podstawowy doxyfile\r" >  _doc/doxyfile_$(TARGET)
	echo -e "@INCLUDE = doxyfile\r"                                    >>  _doc/doxyfile_$(TARGET)
	echo -e "PROJECT_NAME = \"Oprogramowanie dla termometru, wersja LCD: $(LCD_NAME), wersja czujnika: $(SENSOR_NAME)\"\r" >> _doc/doxyfile_$(TARGET)
	echo -e "INPUT = . \\ \r"                                           >> _doc/doxyfile_$(TARGET)
	echo -e "$(SEARCHPATH) \r" >> _doc/doxyfile_$(TARGET)
	echo -e "OUTPUT_DIRECTORY = _doc/$(TARGET)/ \r"                   >> _doc/doxyfile_$(TARGET)
	echo -e "CHM_FILE = ../doc_$(TARGET).chm \r"                          >> _doc/doxyfile_$(TARGET)
	doxygen _doc/doxyfile_$(TARGET)


coff: $(OUTDIR)/$(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(OUTDIR)/$(TARGET).cof


extcoff: $(OUTDIR)/$(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(OUTDIR)/$(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(OUTDIR)/$(TARGET).cof

simulavr_run: elf
	$(SIMULAVR_CMD) $(SIMULAVR_PARAMS)

# Create final output files (.hex, .eep) from ELF output file.
$(OUTDIR)/%.hex: $(OUTDIR)/%.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(OUTDIR)/%.bin: $(OUTDIR)/%.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O binary -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(OUTDIR)/%.eep: $(OUTDIR)/%.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

$(OUTDIR)/%_eep.bin: $(OUTDIR)/%.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O binary $< $@ || exit 0
	

# Create extended listing file from ELF output file.
$(OUTDIR)/%.lss: $(OUTDIR)/%.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
$(OUTDIR)/%.sym: $(OUTDIR)/%.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@



# Create library from object files.
.SECONDARY : $(OUTDIR)/$(TARGET).a
.PRECIOUS : $(OBJ)
$(OUTDIR)/%.a: $(OBJ)
	@echo
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(OBJ)


# Link: create ELF output file from object files.
.SECONDARY : $(OUTDIR)/$(TARGET).elf
.PRECIOUS : $(OBJ)
$(OUTDIR)/%.elf: $(OBJ)
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
	$(REMOVE) $(OUTDIR)/$(TARGET).hex
	$(REMOVE) $(OUTDIR)/$(TARGET).eep
	$(REMOVE) $(OUTDIR)/$(TARGET).cof
	$(REMOVE) $(OUTDIR)/$(TARGET).elf
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
build elf hex eep lss sym coff extcoff \
clean clean_list program debug gdb-config \
print_target_name
