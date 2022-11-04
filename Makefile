#---------------------------------------------------------------------------------
# TARGET	- Name of the output
# PROGADDR	- Program load address (0x80010000 is the default)
# SOURCES	- Source directories (. for current directory)
# INCLUDES	- Include search directories
# ISOXML	- Name of mkpsxiso ISO project script
#---------------------------------------------------------------------------------
TARGET      = MAIN
PROGADDR	= 0x80010000
SOURCES		= KAIN2/Game/ KAIN2/Game/PSX
INCLUDES	= KAIN2/ KAIN2/Game
DEFS		= PSX_VERSION DISC_VERSION NTSC_VERSION
ISOXML		= KAIN2.XML
DISC_ROOTFD	= DISC/

#---------------------------------------------------------------------------------
# USE_SLINK	- Flag to use SLINK, otherwise PSYLINK is used
#---------------------------------------------------------------------------------
USE_SLINK = FALSE

#---------------------------------------------------------------------------------
# LIBDIRS	- Library search directories
# LIBS		- Libraries to link during linking stage
#---------------------------------------------------------------------------------
LIBDIRS		=
LIBS		= LIBETC.LIB LIBPAD.LIB LIBGTE.LIB LIBMCRD.LIB LIBCD.LIB LIBSN.LIB LIBSPU.LIB LIBAPI.LIB

#---------------------------------------------------------------------------------
# CFLAGS	- Base C compiler options
# AFLAGS	- Base assembler options
#---------------------------------------------------------------------------------
CFLAGS		= -Xm -comments-c++ -g -Wall -O2 -G256
AFLAGS		= /g /l /zd /oat+,w-,c+ /q

#---------------------------------------------------------------------------------
# Specific options to debug capable environments (debug options are only usable with
# SN Debugger and a DTL-H2000, 2500 or the Parallel Port based PsyQ/SN Blue Dongle)
# (you must set an H2000 environment variable with TRUE to compile with debug options)
#---------------------------------------------------------------------------------
ifeq "$(H2000)" "TRUE"
CFLAGS		+= -g -DH2000
AFLAGS		+= /zd
endif

#---------------------------------------------------------------------------------
## CC		- C compiler (usually ccpsx)
## AS		- Assembler (usually asmpsx)
#---------------------------------------------------------------------------------
CC			= ccpsx
AS			= asmpsx

#---------------------------------------------------------------------------------
# Parse source directories for source files
#---------------------------------------------------------------------------------
CFILES		= $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.C))
AFILES		= $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.MIP))

#---------------------------------------------------------------------------------
# Generate file names for object binaries
#---------------------------------------------------------------------------------
OFILES		= $(AFILES:.MIP=.obj) $(CFILES:.C=.obj)

#---------------------------------------------------------------------------------
# Default rule, compiles all source files
#---------------------------------------------------------------------------------
all: $(OFILES)
	$(CC) -Xo$(PROGADDR) $(CFLAGS) $(addprefix -L,$(LIBDIRS)) $(addprefix -l,$(LIBS)) $(OFILES)
ifeq "$(USE_SLINK)" "TRUE"
	PSX_SLINK.BAT
else
	PSX_LINK.BAT
endif
#---------------------------------------------------------------------------------
# Clean-up rule
#---------------------------------------------------------------------------------
clean: cleanall

#---------------------------------------------------------------------------------
# ISO build rule (requires MKPSXISO)
#---------------------------------------------------------------------------------
iso:
	cpe2x $(DISC_ROOTFD)$(TARGET).CPE
	cd DISC
	mkpsxisox.exe $(ISOXML)

#---------------------------------------------------------------------------------
# Rule for compiling C source
#---------------------------------------------------------------------------------
%.obj: %.C
	$(CC) $(addprefix -D,$(DEFS)) $(CFLAGS) $(addprefix -I,$(INCLUDES)) -c $< -o $@

#---------------------------------------------------------------------------------
# Rule for assembling assembly source
#---------------------------------------------------------------------------------
%.obj: %.MIP
	$(AS) $(AFLAGS) $<,$@