#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "wizardioqt5.h"

// Include C header files
extern "C" {
#include "wizards-castle.h"
#include "player.h"
#include "gamestate.h"
#include "utilities.h"
#include "combat.h"
#include "vendor.h"
#include "wizardio.h"
}

int main(int argc, char *argv[])
{
    initialize_qt(argc, argv);
    return 0;
}

