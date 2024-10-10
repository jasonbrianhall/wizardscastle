#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "wizards-castle.h"

int main(int argc, char *argv[])
{
    Player player;
    GameState game;
    bool playagain = true;
    bool debug_mode = false;

    // Check for debug flag
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0) {
            debug_mode = true;
            
            break;
        }
    }

    while (playagain) 
    {
        initialize_player(&player);
        print_introduction();
        choose_race(&player);
        print_message("\n");
        choose_sex(&player);
        allocate_attributes(&player);
        buy_equipment(&player);
        buy_lamp_and_flares(&player);
        generate_castle(&game);

        if (debug_mode) {
            // Debug mode: give Runestaff, 10000 gold, 1000 flares, lamp, and reveal Orb location
            player.runestaff_flag = 1;
            player.gold = 10000;
            player.flares = 1000;
            player.lamp_flag = 1;
            printf("DEBUG MODE: You start with the Runestaff, 10000 gold, 1000 flares, and a lamp.\n");
            printf("DEBUG MODE: The Orb of Zot is located at (%d, %d) on level %d.\n", 
                   game.orb_location[0], game.orb_location[1], game.orb_location[2]);
        }

        playagain = main_game_loop(&player, &game);
    }
    return 0;
}
