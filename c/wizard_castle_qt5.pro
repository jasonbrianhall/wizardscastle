QT += core gui widgets

CONFIG += c++11

DEFINES += QT5

# MSVC-specific configurations
msvc {
    QMAKE_CXXFLAGS += /MP /Zc:preprocessor /wd4819
    QMAKE_CFLAGS += /MP /Zc:preprocessor /wd4819
    DEFINES += _CRT_SECURE_NO_WARNINGS
    CONFIG += force_debug_info
    CONFIG -= incremental
}

SOURCES += \
    main_qt5.cpp \
    wizardioqt5.cpp \
    wizards-castle.c \
    player.c \
    gamestate.c \
    utilities.c \
    combat.c \
    vendor.c \
    save_load.c

HEADERS += \
    wizardio.h \
    wizards-castle.h \
    player.h \
    gamestate.h \
    utilities.h \
    combat.h \
    vendor.h \
    save_load.h

TARGET = wizards_castle_qt5
