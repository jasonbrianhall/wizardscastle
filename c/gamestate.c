
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "gamestate.h"
#include "utilities.h"
#include "wizardio.h"

// Game ending functions
int check_game_over(Player *player, GameState *game) {
    // Check if player has died
    if (player->strength <= 0 || player->intelligence <= 0 || player->dexterity <= 0) {
        return 1;
    }
    
    // Check if player has won (e.g., found the Orb of Zot and exited)
    /*if (player->orb_flag && player->x == 1 && player->y == 4 && player->level == 1) {
        return 1;
    }*/
    
    if (game->game_over)
    {
        return 1;
    }
    // Game is not over
    return 0;
}

void end_game(Player *player, GameState *game)
{
    printStars();
    
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    const char *armor_types[] = {"NO ARMOR", "LEATHER", "CHAINMAIL", "PLATE", "STONE"};
    const char *weapon_types[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD", "EXCALIBUR"};

    if (player->strength <= 0 || player->intelligence <= 0 || player->dexterity <= 0) {
        print_message_formatted("A NOBLE EFFORT, OH FORMERLY LIVING %s!\n\n", race_names[player->race - 1]);
        
        if (player->strength <= 0)
            print_message_formatted("YOU DIED DUE TO LACK OF STRENGTH.\n\n");
        else if (player->intelligence <= 0)
            print_message_formatted("YOU DIED DUE TO LACK OF INTELLIGENCE.\n\n");
        else
            print_message_formatted("YOU DIED DUE TO LACK OF DEXTERITY.\n\n");

        print_message_formatted("AT THE TIME YOU DIED, YOU HAD :\n");
    }
    else if (game->victory) {
        print_message_formatted("CONGRATULATIONS!\n\n");
        print_message_formatted("YOU LEFT THE CASTLE WITH THE ORB OF ZOT.\n\n");
        print_message_formatted("AN INCREDIBLY GLORIOUS VICTORY!!\n\n");
        print_message_formatted("IN ADDITION, YOU GOT OUT WITH THE FOLLOWING :\n");
    }
    else {
        print_message_formatted("YOU LEFT THE CASTLE WITHOUT THE ORB OF ZOT.\n\n");
        print_message_formatted("A LESS THAN AWE-INSPIRING DEFEAT.\n\n");
        print_message_formatted("WHEN YOU LEFT THE CASTLE, YOU HAD :\n");
    }

    // List treasures
    for (int i = 0; i < TREASURE_COUNT; i++) {
        if (game->treasure[i]) {
            print_message_formatted("%s\n", get_treasure_name(i));
        }
    }

    // Print equipment
    print_message_formatted("%s AND %s", weapon_types[player->weapon_type], armor_types[player->armor_type]);
    if (player->lamp_flag)
        print_message_formatted(" AND A LAMP");
    print_message_formatted("\n");

    // Print flares and gold
    print_message_formatted("YOU ALSO HAD %d FLARES AND %d GOLD PIECES\n", player->flares, player->gold);

    // Print Runestaff status
    if (player->runestaff_flag)
        print_message_formatted("AND THE RUNESTAFF\n");

    // Print turn count
    print_message_formatted("\nAND IT TOOK YOU %d TURNS!\n", game->turn_count);

    printStars();
}

// Map and room functions
void generate_castle(GameState *game)
{
    int x, y, z, q, q1, level;
    
    // Seed the random number generator
    srand(time(NULL));
    for (int i=0; i<TREASURE_END-TREASURE_START; i++)
    {
        game->treasure[i]=0;
    }
    game->game_over=0;
    game->victory=0;
    game->vendor_attacked=0;
    game->turn_count=0;
    
    // Seed the random number generator
    srand(time(NULL));

    // Set the entrance
    x = 1; y = 4; level = 1;
    game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = 2;  // Entrance
    

    // Initialize all rooms to empty (101)
    for (q = 0; q < MAP_SIZE; q++) {
        game->location_map[q] = EMPTY_ROOM;
        game->discovered_rooms[q] = 0;  // 0 means undiscovered
    }

    int entrance_index = CALCULATE_ROOM_INDEX(level, x, y);
    game->location_map[entrance_index] = ENTRANCE;
    game->discovered_rooms[entrance_index] = 1;  // 1 means discovered

    // Place stairs
    for (z = 1; z <= 8; z++) {
        for (int i = 1; i <= 2; i++) {
            do {
                x = random_number(8);
                y = random_number(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
            
            // Place stairs down on current level
            game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = STAIRS_UP;  // Stairs down
            
            // Place corresponding stairs up on the level below (or on level 1 if we're on level 8)
            int next_level = (z % 8) + 1;
            game->location_map[CALCULATE_ROOM_INDEX(next_level, x, y)] = STAIRS_DOWN;  // Stairs up
        }
    }


    // Place other elements
    for (z = 1; z <= 8; z++) {
        // Place monsters, treasures, etc.
        for (q = MONSTER_START; q <= MONSTER_END; q++) {
            do {
                x = random_number(8);
                y = random_number(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
            game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
        }

        // Place pools, chests, and gold
        for (q1 = 1; q1 <= 3; q1++) {
            for (q = 105; q <= 112; q++) {
                do {
                    x = random_number(8);
                    y = random_number(8);
                } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
                game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
            }
            
            // Place vendor
            do {
                x = random_number(8);
                y = random_number(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
            game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = VENDOR;
        }
    }

    // Place special items (crystal orb, ruby red, etc.)
    for (q = TREASURE_START; q <= TREASURE_END; q++) {
        do {
            z = random_number(8);
            x = random_number(8);
            y = random_number(8);
        } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
        game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
    }

    // Place treasures (126-133)
    for (q = TREASURE_START; q <= TREASURE_END; q++) {
        do {
            z = random_number(8);
            x = random_number(8);
            y = random_number(8);
        } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
        game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
    }

    // Place the Runestaff
    do {
        z = random_number(8);
        x = random_number(8);
        y = random_number(8);
    } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = MONSTER_START + random_number(MONSTER_END-MONSTER_START)-1;
    game->runestaff_location[0] = x;
    game->runestaff_location[1] = y;
    game->runestaff_location[2] = z;

    // Place the Orb of Zot
    do {
        z = random_number(8);
        x = random_number(8);
        y = random_number(8);
    } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = WARP;
    game->orb_location[0] = x;
    game->orb_location[1] = y;
    game->orb_location[2] = z;

    // Set the entrance
    x = 1; y = 4; z = 1;
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = ENTRANCE;
}

int get_room_content(GameState *game, int x, int y, int level)
{
    int index = CALCULATE_ROOM_INDEX(level, x, y);
    if (index >= 0 && index < MAP_SIZE) {
        return game->location_map[index];
    }
    print_message_formatted("Invalid Room %i\n", index);
    return -1;  // Invalid room
}


void set_room_content(GameState *game, int x, int y, int level, int content)
{
    int index = CALCULATE_ROOM_INDEX(level, x, y);
    if (index >= 0 && index < MAP_SIZE) {
        game->location_map[index] = content;
    } else {
        // Handle error: invalid room coordinates
        char error_message[100];
        snprintf(error_message, sizeof(error_message), 
                 "Error: Attempted to set invalid room content at (%d,%d) level %d\n", 
                 x, y, level);
        print_message_formatted(error_message);
    }
}


void mark_room_discovered(GameState *game, int x, int y, int level)
{
    int index = CALCULATE_ROOM_INDEX(level, x, y);
    game->discovered_rooms[index] = 1;
}

int is_room_discovered(GameState *game, int x, int y, int level)
{
    //print_message("is room discovered %i %i %i %i", level, x, y CALCULATE_ROOM_INDEX(level, x, y));
    int index = CALCULATE_ROOM_INDEX(level, x, y);
    
    //print_message("Discovered %i, %i\n\n", CALCULATE_ROOM_INDEX(level, x, y), game->discovered_rooms[index]);
    return game->discovered_rooms[index];
}

void handle_treasure(Player *player, GameState *game, int room_content)
{
    int treasure_index = room_content - TREASURE_START;
    const char* treasure_name = get_treasure_name(treasure_index);

    print_message_formatted("\nYOU FOUND %s!\n\n", treasure_name);
    
    game->treasure[treasure_index] = 1;
    player->treasure_count++;
    print_message_formatted("YOU NOW HAVE %d TREASURES.\n\n", player->treasure_count);

    // Apply special effects of treasures
    switch (treasure_index) {
        case 0: // Ruby Red
        	print_message_formatted("THE RUBY RED WILL HELP YOU AVOID LETHARGY.\n\n");
	    break;
        case 1: // Norn Stone
	    print_message_formatted("THE NORN STONE GLOWS WITH AN OTHERWORLDLY LIGHT.\n\n");
    	break;
        case 2: // Pale Pearl
	    print_message_formatted("THE PALE PEARL WILL PROTECT YOU FROM LEECHES.\n\n");
        	break;
        case 3: // Opal Eye
	    print_message_formatted("THE OPAL EYE WILL CURE BLINDNESS.\n");
        	if (player->blindness_flag) {
	            player->blindness_flag = 0;
	            print_message_formatted("YOUR BLINDNESS IS CURED!\n");
        	}
        	print_message_formatted("\n");
	    break;
        case 4: // Green Gem
	    print_message_formatted("THE GREEN GEM WILL HELP YOU AVOID FORGETTING.\n\n");
        	break;
        case 5: // Blue Flame
	    print_message_formatted("THE BLUE FLAME WILL DISSOLVE BOOKS.\n\n");
        	if (player->stickybook_flag) {
	        player->stickybook_flag = 0;
	        print_message_formatted("THE STICKY BOOK DISSOLVES!\n\n");
        	}
	    break;
        case 6: // Palantir
        case 7: // Silmaril
	    print_message_formatted("ITS BEAUTY IS BEYOND COMPARE.\n\n");
        	break;
    }

    // Remove the treasure from the room
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);   
}

