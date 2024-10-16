#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdarg.h>
#include "wizards-castle.h"
#include "player.h"
#include "gamestate.h"
#include "utilities.h"
#include "combat.h"
#include "vendor.h"
#include "wizardio.h"

#define WRAP_COORDINATE(coord) (((coord) - 1 + 8) % 8 + 1)
#define CALCULATE_ROOM_INDEX(level, x, y) (64 * ((level) - 1) + 8 * ((x) - 1) + (y) - 1)
#define ENSURE_VALID_CONTENT(content) ((content) + 100 * ((content) > 99))

void print_introduction(void)
{
	printStars();
        print_message          ("\n                * * * THE WIZARD'S CASTLE * * *\n\n");
	printStars();
	print_message_formatted("\n");
	print_message_formatted("MANY CYCLES AGO, IN THE KINGDOM OF ");
	print_message_formatted(          "N'Dic, the gnomic\n"
	                        "wizard ");
	print_message_formatted("ZOT FORGED HIS GREAT ");
	print_message           (          "*ORB OF POWER*. ");
	print_message_formatted("HE SOON\n"
	                        "VANISHED, LEAVING BEHIND HIS VAST SUBTERRANEAN CASTLE\n"
	                        "FILLED WITH ESURIENT MONSTERS, FABULOUS TREASURES, AND\n"
                                "THE INCREDIBLE "); 
        print_message(          "*ORB OF ZOT*.");
        print_message_formatted(" FROM THAT TIME HENCE, MANY\n"
	                        "A BOLD YOUTH HAS VENTURED INTO THE WIZARD'S CASTLE. AS\n"
	                        "OF NOW, *NONE* HAS EVER EMERGED VICTORIOUSLY! BEWARE!!\n\n");

}

bool main_game_loop(Player *player, GameState *game)
{
    char user_command[100];
    int game_over = 0;
    const char *room_contents[] = {
        "AN EMPTY ROOM", "THE ENTRANCE", "STAIRS GOING UP", "STAIRS GOING DOWN",
        "A POOL", "A CHEST", "GOLD PIECES", "FLARES", "A WARP", "A SINKHOLE",
        "A CRYSTAL ORB", "A BOOK", "A KOBOLD", "AN ORC", "A WOLF", "A GOBLIN",
        "AN OGRE", "A TROLL", "A BEAR", "A MINOTAUR", "A GARGOYLE", "A CHIMERA",
        "A BALROG", "A DRAGON", "A VENDOR", "A RED RUBY", "THE NORN STONE", "THE PALE_PEARL",
        "THE OPAL EYE", "A GREEN GEM", "THE BLUE FLAME", "THE PALANTIR", "THE SILMARIL"
    };
    char message[100];


    while (!game_over) {
        game->turn_count++;

       // Handle curses
        if (player->runestaff_flag == 0 && player->orb_flag == 0) {
            // Lethargy curse (similar to line 1960 in BASIC)
            if (!game->treasure[0] && random_number(100) <= 5) {  // 5% chance if no Ruby Red
                game->turn_count++;
                print_message_formatted("\nYOU ARE AFFECTED BY LETHARGY. YOU LOSE A TURN.\n");
            }

            // Leech curse (similar to line 1965 in BASIC)
            if (!game->treasure[2] && random_number(100) <= 5) {  // 5% chance if no Pale Pearl
                int gold_lost = random_number(5);
                player->gold = (player->gold > gold_lost) ? player->gold - gold_lost : 0;
                print_message_formatted("\nA LEECH ATTACKS YOU! YOU LOSE %d GOLD PIECES.\n", gold_lost);
            }

            // Forgetfulness curse (similar to lines 1975-2015 in BASIC)
            if (!game->treasure[4] && random_number(100) <= 5) {  // 5% chance if no Green Gem
                int old_x = player->x, old_y = player->y, old_z = player->level;
                player->x = random_number(8);
                player->y = random_number(8);
                player->level = random_number(8);
                print_message_formatted("\nYOU SUDDENLY FORGET WHERE YOU ARE!\n");
                print_message_formatted("YOU FIND YOURSELF AT (%d,%d) ON LEVEL %d.\n", player->x, player->y, player->level);

                // If player was in an empty room, mark old room as unexplored
                if (get_room_content(game, old_x, old_y, old_z) == EMPTY_ROOM) {
                    int index = CALCULATE_ROOM_INDEX(old_z, old_x, old_y);
                    game->discovered_rooms[index] = 0;
                }
            }
        }

        // Display random events (similar to lines 2010-2060 in BASIC)
        if (random_number(5) == 1) {
            print_message_formatted("\nYOU ");
            int event_type = random_number(7) + player->blindness_flag;
            if (event_type > 7) event_type = 4;
            
            switch (event_type) {
                case 1: print_message_formatted("SEE A BAT FLY BY!\n"); break;
                case 2: print_message_formatted("HEAR FOOTSTEPS!\n"); break;
                case 3: print_message_formatted("SNEEZED!\n"); break;
                case 4: print_message_formatted("STEPPED ON A FROG!\n"); break;
                case 5: print_message_formatted("SMELL SOMETHING FRYING!\n"); break;
                case 6: print_message_formatted("FEEL LIKE YOU'RE BEING WATCHED!\n"); break;
                case 7: print_message_formatted("HEAR FAINT RUSTLING NOISES!\n"); break;
            }
        }

        // Handle blindness cure (similar to lines 2065-2075 in BASIC)
        if (player->blindness_flag == 1 && game->treasure[3] == 1) {
            print_message_formatted("\nTHE OPAL EYE CURES YOUR BLINDNESS!\n");
            player->blindness_flag = 0;
        }

        // Handle sticky book cure (similar to lines 2080-2090 in BASIC)
        if (player->stickybook_flag == 1 && game->treasure[5] == 1) {
            print_message_formatted("\nTHE BLUE FLAME DISSOLVES THE BOOK!\n");
            player->stickybook_flag = 0;
        }

        print_message_formatted("\n");
        //print_status(player, game);
        
        int room_content = get_room_content(game, player->x, player->y, player->level);
        if (room_content == ENTRANCE) {  // The Entrance
            char message[100];
            snprintf(message, sizeof(message), "OK, %s, YOU ARE NOW ENTERING THE CASTLE!\n", get_race_name(player->race));
            print_message_formatted(message);
        } else if (room_content >= EMPTY_ROOM && room_content <= TREASURE_END) {
            char message[100];
            snprintf(message, sizeof(message), "HERE YOU FIND %s.\n", room_contents[room_content - EMPTY_ROOM]);
            print_message_formatted(message);
            if (room_content==GOLD)
            {
                int gold_found = random_number(1000);  // Random amount between 1 and 1000
                player->gold += gold_found;
                char message[100];
                snprintf(message, sizeof(message), "%d GOLD PIECES HAVE BEEN ADDED TO YOUR INVENTORY!\n", gold_found);
                print_message_formatted(message);
                set_room_content(game, player->x, player->y, player->level, 101);  // Empty the room
            }
            else if (room_content==FLARES)
            {
                int flares_found = random_number(5);  // Random amount between 1 and 5

                player->flares += flares_found;
                snprintf(message, sizeof(message), "%d FLARES HAVE BEEN ADDED TO YOUR INVENTORY!\n", flares_found);
                print_message_formatted(message);
                set_room_content(game, player->x, player->y, player->level, 101);  // Empty the room
           }
           // Monsters
           else if (room_content>=MONSTER_START && room_content <=MONSTER_END)
           {
               fight_monster(player, game);
               if (player->temp_intelligence>0 && player->intelligence > player->temp_intelligence)
               {
                   player->intelligence=player->temp_intelligence;
                   player->temp_intelligence=0;
               }
               if (player->temp_dexterity>0  && player->dexterity>player->temp_dexterity)
               {
                   player->dexterity=player->temp_dexterity;
                   player->temp_dexterity=0;
               }
               if (player->strength>18)
               {
                   player->strength=18;
               }


           }
           // Vendors
           else if (room_content==VENDOR)
           {
               handle_vendor(player, game);
               if (player->temp_intelligence>0 && player->intelligence > player->temp_intelligence)
               {
                   player->intelligence=player->temp_intelligence;
                   player->temp_intelligence=0;
               }
               if (player->temp_dexterity>0  && player->dexterity>player->temp_dexterity)
               {
                   player->dexterity=player->temp_dexterity;
                   player->temp_dexterity=0;
               }
               if (player->strength>18)
               {
                   player->strength=18;
                   player->temp_strength=0;
               }

           }
           // Treasure
           else if (room_content>=TREASURE_START && room_content<=TREASURE_END)
           {
                handle_treasure(player, game, room_content); 
           }
           
        } else {
            print_message_formatted("HERE YOU FIND AN UNKNOWN ROOM.\n");
            snprintf(message, sizeof(message), "%i\n", room_content);
            print_message_formatted(message);
        }
        game_over = check_game_over(player, game);
        if(!game_over)
        {
            strncpy(user_command, get_user_input_main(), sizeof(user_command) - 1);
            user_command[sizeof(user_command) - 1] = '\0';  // Ensure null-termination            snprintf(message, sizeof(message), "User Command: %s\n", user_command);
            //print_message_formatted(message);
            switch (user_command[0]) {
                case '\0':
                    print_message_formatted("\n\nPlease enter a command.\n\n");
                    break;

                case 'N': case 'S': case 'E': case 'W':
                    move_player(player, game, user_command[0]);
                    break;
                case 'U':
                    if (room_content == STAIRS_UP) {  // Stairs going up
                        player->level++;
                        if (player->level>8)
                        {
                            player->level=1;
                        }
                        print_message_formatted("YOU CLIMB UP THE STAIRS.\n");
                    } else {
                        print_message_formatted("THERE ARE NO STAIRS GOING UP FROM HERE!\n");
                    }
                    break;
                case 'D':
                    if (strncmp(user_command, "DR", 2) == 0) {
                        if (room_content == POOL)
                        {
                            drink_from_pool(player, game);
                        }
                        else
                        {
                            print_message_formatted("** IF YOU WANT A DRINK, FIND A POOL!");
                        }
                    }
                    else if (room_content == STAIRS_DOWN) {  // Stairs going down
                        player->level--;
                        if (player->level<1)
                        {
                            player->level=8;
                        }
                        print_message_formatted("YOU DESCEND THE STAIRS.\n");
                    } else {
                        print_message_formatted("THERE ARE NO STAIRS GOING DOWN FROM HERE!\n");
                    }
                    break;
                case 'M':
                    display_map(game, player);
                    break;
                case 'F':
                    use_flare(player, game);    
                    break;    
                case 'L':
                    use_lamp(player, game);
                    break;
                case 'O':
                    if (room_content == CHEST) {  // Chest
                        open_chest(player, game);
                    } else if (room_content == BOOK) {  // Book
                        open_book(player, game);
                    } else {
                        print_message_formatted("THERE'S NOTHING HERE TO OPEN!\n");
                    }
                    break;
                case 'G':
                    if (room_content == CRYSTAL_ORB) {  // Crystal orb
                        gaze_into_orb(player, game);
                    } else {
                        print_message_formatted("THERE'S NO CRYSTAL ORB HERE TO GAZE INTO!\n");
                    }
                    break;
                case 'T':
                    if (player->runestaff_flag) {
                        teleport(player, game);
                    } else {
                        print_message_formatted("YOU CAN'T TELEPORT WITHOUT THE RUNESTAFF!\n");
                    }
                    break;
                case 'Q':
                    print_message("Do you really want to quit now? (Y/N) ");
                    if (get_user_input_yn() == 'Y') {
                        game_over = 1;
                    } else {
                        print_message_formatted("OK, CONTINUE ON BRAVE ADVENTURER!\n");
                    }
                    break;
                case 'Z':
                    print_status(player, game);
                    break;
                case 'H':
                    print_help();
                    break;
                default:
                    print_message("Invalid command. Type 'H' for help.\n");
            }
        }
        if (!game_over) {
            game_over = check_game_over(player, game);
        }
    }

    end_game(player, game);
    // Ask if the player wants to play again
    print_message_formatted("\nARE YOU FOOLISH ENOUGH TO WANT TO PLAY AGAIN? ");
    char play_again = get_user_input_yn();
    if (play_again == 'Y') {
        print_message_formatted("\nSOME ADVENTURERS NEVER LEARN!\n\n");
        print_message_formatted("PLEASE BE PATIENT WHILE THE CASTLE IS RESTOCKED.\n\n");
        return 1;
    } else {
        print_message_formatted("\nGOOD BYE, AND GOOD LUCK IN YOUR TRAVELS!\n");
        return 0;        
    }
}

void print_help()
{
    print_message("\n*** WIZARD'S CASTLE COMMAND AND INFORMATION SUMMARY ***\n\n");
    print_message("THE FOLLOWING COMMANDS ARE AVAILABLE:\n\n");
    print_message("H/ELP     - Display this help information\n");
    print_message("N/ORTH    - Move north\n");
    print_message("S/OUTH    - Move south\n");
    print_message("E/AST     - Move east\n");
    print_message("W/EST     - Move west\n");
    print_message("U/P       - Go up stairs\n");
    print_message("D/OWN     - Go down stairs\n");
    print_message("DR/INK    - Drink from a pool\n");
    print_message("M/AP      - Display map of current level\n");
    print_message("F/LARE    - Light a flare\n");
    print_message("L/AMP     - Use lamp to look into adjacent room\n");
    print_message("O/PEN     - Open a chest or book\n");
    print_message("G/AZE     - Gaze into a crystal orb\n");
    print_message("T/ELEPORT - Teleport to a new location (requires Runestaff)\n");
    print_message("Q/UIT     - End the game\n");
    print_message("Z/tatus   - Player Status (South was already used)\n\n");

    #ifdef MSDOS
    print_message_formatted("PRESS ENTER TO CONTINUE...");
    while (getchar() != '\n');  // Wait for Enter key
    #endif

    print_message_formatted("THE CONTENTS OF ROOMS ARE AS FOLLOWS:\n\n");
    print_message          ("EMPTY    = EMPTY ROOM      BOOK     = BOOK            Chest = CHEST\n");
    print_message          ("STAIRS D = STAIRS DOWN     ENTRANCE = ENTRANCE/EXIT   FLARES = FLARES\n");
    print_message          ("GOLD     = GOLD PIECES     MONSTER  = MONSTER NAME    ORB = CRYSTAL ORB\n");
    print_message          ("POOL     = MAGIC POOL      SINKHOLE = SINKHOLE        TREASURE = TREASURE\n");
    print_message          ("STAIRS U = STAIRS UP       VENDOR   = VENDOR          WARP = WARP/ORB\n\n");

    print_message_formatted("THE BENEFITS OF HAVING TREASURES ARE:\n\n");
    print_message          ("RUBY RED    - Avoid Lethargy     PALE PEARL  - Avoid Leech\n");
    print_message          ("GREEN GEM   - Avoid Forgetting   OPAL EYE    - Cures Blindess\n");
    print_message          ("BLUE FLAME  - Dissolves Books    NORN STONE  - Pretty\n");
    print_message          ("PALANTIR    - Pretty             SILMARIL    - Pretty\n\n");

    print_message_formatted("PRESS ENTER TO CONTINUE...");
    while (getchar() != '\n');  // Wait for Enter key
}


