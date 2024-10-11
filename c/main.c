#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "wizards-castle.h"

int main(int argc, char *argv[])
{
    Player player;
    GameState game;
    int q;
    bool playagain = true;
    bool debug_mode = false;
    int i;
    // Check for debug flag
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0) {
            debug_mode = true;
            break;
        }
    }

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
            // Debug mode: Set player as male elf with max attributes and equipment
            player.race = 2;  // Elf
            player.sex = 1;   // Male
            player.strength = 18;
            player.intelligence = 18;
            player.dexterity = 18;
            player.gold = 10000;
            player.flares = 1000;
            player.lamp_flag = 1;
            player.runestaff_flag = 1;
            player.weapon_type = 3;  // Sword
            player.armor_type = 3;   // Plate (assuming this is the best armor, equivalent to "Shield")
            player.armor_points = 21; // Max armor points for Plate armor


            printf("DEBUG MODE: You are a male elf with 18 Strength, 18 Intelligence, and 18 Dexterity.\n");
            printf("DEBUG MODE: You have a Sword and Plate armor.\n");
            printf("DEBUG MODE: You start with the Runestaff, 10000 gold, 1000 flares, and a lamp.\n");
            printf("DEBUG MODE: All rooms are discovered\n");

        }

        generate_castle(&game);

        if (debug_mode) {
            printf("DEBUG MODE: The Orb of Zot is located at (%d, %d) on level %d.\n", 
                   game.orb_location[1], game.orb_location[0], game.orb_location[2]);
            for (q = 0; q < MAP_SIZE; q++) {
                game.discovered_rooms[q] = 1;  // 0 means undiscovered
            }

        }

        playagain = main_game_loop(&player, &game);
    }
    return 0;
}
