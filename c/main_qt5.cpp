#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "wizardioqt5.h"

bool parse_arguments(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
    return std::find(args.begin(), args.end(), "--debug") != args.end();
}

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

