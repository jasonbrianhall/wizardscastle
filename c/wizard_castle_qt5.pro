QT += core gui widgets

CONFIG += c++11

DEFINES += QT5

SOURCES += \
    main_qt5.cpp \
    wizardioqt5.cpp \
    wizards-castle.c \
    player.c \
    gamestate.c \
    utilities.c \
    combat.c \
    vendor.c

HEADERS += \
    wizardio.h \
    wizards-castle.h \
    player.h \
    gamestate.h \
    utilities.h \
    combat.h \
    vendor.h
    combat.c

TARGET = wizards_castle_qt5
