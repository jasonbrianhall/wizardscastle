QT += core gui widgets

CONFIG += c++17

DEFINES += QT5

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
