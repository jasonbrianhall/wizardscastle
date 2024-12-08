# Makefile for Wizard's Castle
VERSION=1.0

# Compiler
CC = gcc

# Source files
SRCS = main.c wizardio.c wizards-castle.c player.c gamestate.c combat.c utilities.c vendor.c save_load.c

# Output executable
TARGET = wizardscastle
DOS_TARGET = wizard.exe
WINDOWS_TARGET = wizard_windows.exe

# Compiler flags
CFLAGS = -Wall -Wextra
DEBUGFLAGS = -g -fsanitize=address

# Docker image for DJGPP
DJGPP_IMAGE = djfdyuruiry/djgpp

# Docker image for Windows
MINGW_IMAGE = mdashnet/mingw  # MinGW for Windows

# Compiler for Windows (MinGW)
MINGW_CC = x86_64-w64-mingw32-gcc

QT_WINDOWS_IMAGE = fedora:38

# Correct CSDPMI URL
CSDPMI_URL = http://na.mirror.garr.it/mirrors/djgpp/current/v2misc/csdpmi7b.zip

# Get current user and group IDs
USER_ID = $(shell id -u)
GROUP_ID = $(shell id -g)

# Docker volume for yum cache
YUM_CACHE_VOLUME = wizard-castle-yum-cache

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
	rm -f $(TARGET) $(DOS_TARGET) csdpmi7b.zip || true
	rm -rf csdpmi  || true
	rm *.exe || true
	rm *.EXE || true
	rm *.zip || true
	rm *.zip.*  || true
	rm *.tar.gz || true
	$(MAKE) -f $(QT_MAKEFILE) clean || true

# Target to pull the DJGPP Docker image
pull-djgpp:
	docker pull $(DJGPP_IMAGE)

pull-mingw:
	docker pull $(MINGW_IMAGE)

# Target to download CSDPMI
get-csdpmi:
	wget $(CSDPMI_URL)
	unzip -o csdpmi7b.zip -d csdpmi

# Target to build for MS-DOS using DJGPP in Docker
msdos: pull-djgpp get-csdpmi
	docker run --rm -v $(PWD):/src:z -u $(USER_ID):$(GROUP_ID) $(DJGPP_IMAGE) /bin/sh -c "cd /src && gcc $(SRCS) -o $(DOS_TARGET)"
	cp csdpmi/bin/CWSDPMI.EXE .

windows: pull-mingw
	docker run --rm -v $(PWD):/src:z -u $(USER_ID):$(GROUP_ID) $(MINGW_IMAGE) /bin/sh -c "cd /src && $(MINGW_CC) $(SRCS) -o $(WINDOWS_TARGET)"

# Target to run the MS-DOS executable using DOSBox
run-msdos: msdos
	dosbox $(DOS_TARGET)

rpm: clean
	# Create the source tarball
	mkdir -p wizardscastle-$(VERSION)
	cp *.c   wizardscastle-$(VERSION)/
	cp *.h   wizardscastle-$(VERSION)/
	cp *.cpp wizardscastle-$(VERSION)/
	cp *.pro wizardscastle-$(VERSION)/
	cp Makefile wizardscastle-$(VERSION)/
	tar -czf wizardscastle-$(VERSION).tar.gz wizardscastle-$(VERSION)
	rm -rf wizardscastle-$(VERSION)
	# Move the source tarball to the SOURCES directory for rpmbuild
	mkdir -p ~/rpmbuild/SOURCES
	cp wizardscastle-$(VERSION).tar.gz ~/rpmbuild/SOURCES/

	# Build the RPM using the existing spec file
	rpmbuild -ba -D 'debug_package %{nil}' wizardscastle.spec

# Qt-specific variables and targets
QMAKE = qmake
MINGW_QMAKE = mingw64-qmake-qt5 
QT_MAKEFILE = Makefile.qt
MINGW_QT_MAKEFILE = Makefile.qtwin64
QT_PRO = wizard_castle_qt5.pro

qt: $(QT_MAKEFILE)
	$(MAKE) -f $(QT_MAKEFILE) clean
	$(MAKE) -f $(QT_MAKEFILE)

$(QT_MAKEFILE): $(QT_PRO)
	$(QMAKE) -o $(QT_MAKEFILE) $(QT_PRO)

qt-windows: 
	$(MINGW_QMAKE) CONFIG+=static CONFIG+=release -o $(MINGW_QT_MAKEFILE)
	make -f $(MINGW_QT_MAKEFILE) clean
	make -f $(MINGW_QT_MAKEFILE)
	#cp /usr/x86_64-w64-mingw32/sys-root/mingw/bin/*.dll release




clean-qt:
	[ -f $(QT_MAKEFILE) ] && $(MAKE) -f $(QT_MAKEFILE) clean || true
	rm -f $(QT_MAKEFILE)

.PHONY: all debug clean pull-djgpp get-csdpmi msdos run-msdos rpm qt clean-qt windows pull-mingw
