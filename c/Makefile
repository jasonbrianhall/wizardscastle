# Makefile for Wizard's Castle

# Compiler
CC = gcc

# Source files
SRCS = main.c wizards-castle.c

# Output executable
TARGET = wizardscastle
DOS_TARGET = wizard.exe

# Compiler flags
CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g -fsanitize=address

# Docker image for DJGPP
DJGPP_IMAGE = djfdyuruiry/djgpp

# Correct CSDPMI URL
CSDPMI_URL = http://na.mirror.garr.it/mirrors/djgpp/current/v2misc/csdpmi7b.zip

# Default target
all: $(TARGET)

# Rule to build the executable without debug symbols
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Debug target to build with debug symbols
debug: CFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Clean target to remove generated files
clean:
	rm -f $(TARGET) $(DOS_TARGET) csdpmi7b.zip
	rm -rf csdpmi

# Target to pull the DJGPP Docker image
pull-djgpp:
	docker pull $(DJGPP_IMAGE)

# Target to download CSDPMI
get-csdpmi:
	wget $(CSDPMI_URL)
	unzip -o csdpmi7b.zip -d csdpmi

# Target to build for MS-DOS using DJGPP in Docker
msdos: pull-djgpp get-csdpmi
	docker run --rm -v $(PWD):/src $(DJGPP_IMAGE) bash -c "cd /src && gcc $(SRCS) -o $(DOS_TARGET)"
	cp csdpmi/bin/CWSDPMI.EXE .

# Target to run the MS-DOS executable using DOSBox
run-msdos: msdos
	dosbox $(DOS_TARGET)

.PHONY: all debug clean pull-djgpp get-csdpmi msdos run-msdos
