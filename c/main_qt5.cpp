#include <iostream>

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

// Declare the initialize_qt function from wizardioqt5.cpp
extern "C" void initialize_qt(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    initialize_qt(argc, argv);

    Player player;
    GameState game;
    bool playagain = true;
    bool debug_mode = false;

    while (playagain) 
    {
        initialize_player(&player);
        print_introduction();

        if (!debug_mode) {
            choose_race(&player);
            print_message("\n");
            choose_sex(&player);
            allocate_attributes(&player);
            buy_equipment(&player);
            buy_lamp_and_flares(&player);
        } else {
            // Debug mode setup (as in your original main.c)
            // ...
        }

        generate_castle(&game);

        if (debug_mode) {
            // Debug mode castle setup (as in your original main.c)
            // ...
        }

        playagain = main_game_loop(&player, &game);

        if (playagain) {
            print_message("\nSOME ADVENTURERS NEVER LEARN!\n\n");
            print_message("PLEASE BE PATIENT WHILE THE CASTLE IS RESTOCKED.\n\n");
        } else {
            print_message("\nGOOD BYE, AND GOOD LUCK IN YOUR TRAVELS!\n");
        }
    }

    return 0;
}

