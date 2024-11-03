#include "wizardioqt5.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Include C header files
extern "C" {
#include "combat.h"
#include "gamestate.h"
#include "player.h"
#include "utilities.h"
#include "vendor.h"
#include "wizardio.h"
#include "wizards-castle.h"
}

int main(int argc, char *argv[]) {
  initialize_qt(argc, argv);
  return 0;
}
