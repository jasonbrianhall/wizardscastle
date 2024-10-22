QT += core gui widgets

CONFIG += c++11

DEFINES += QT5

msvc {
    QMAKE_CXXFLAGS += /WX /W4
    QMAKE_CFLAGS += /WX /W4
    
    # Disable specific warnings
    QMAKE_CXXFLAGS += /wd4819 /wd4577 /wd4467
    QMAKE_CFLAGS += /wd4819 /wd4577 /wd4467
    
    # Enable additional C++ features
    QMAKE_CXXFLAGS += /Zc:__cplusplus /Zc:rvalueCast /Zc:inline /Zc:strictStrings /Zc:throwingNew /Zc:referenceBinding
}

# Compiler-specific options for non-MSVC compilers
!msvc {
    QMAKE_CXXFLAGS += -Wall -Wextra
    QMAKE_CFLAGS += -Wall -Wextra
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
