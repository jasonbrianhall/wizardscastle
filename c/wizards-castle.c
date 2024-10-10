#include "wizards-castle.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define WRAP_COORDINATE(coord) (((coord) - 1 + 8) % 8 + 1)
#define CALCULATE_ROOM_INDEX(level, x, y) (64 * ((level) - 1) + 8 * ((x) - 1) + (y) - 1)
#define ENSURE_VALID_CONTENT(content) ((content) + 100 * ((content) > 99))

void print_introduction(void)
{
	printStars();
        print_message("\n                * * * THE WIZARD'S CASTLE * * *\n\n");
	printStars();
	print_message("\n");
	print_message("MANY CYCLES AGO, IN THE KINGDOM OF N'DIC, THE GNOMIC\n");
	print_message("WIZARD ZOT FORGED HIS GREAT *ORB OF POWER*. HE SOON\n");
	print_message("VANISHED, LEAVING BEHIND HIS VAST SUBTERRANEAN CASTLE\n");
	print_message("FILLED WITH ESURIENT MONSTERS, FABULOUS TREASURES, AND\n");
	print_message("THE INCREDIBLE *ORB OF ZOT*. FROM THAT TIME HENCE, MANY\n");
	print_message("A BOLD YOUTH HAS VENTURED INTO THE WIZARD'S CASTLE. AS\n");
	print_message("OF NOW, *NONE* HAS EVER EMERGED VICTORIOUSLY! BEWARE!!\n\n");

}

void printStars(void)
{
	print_message("****************************************************************\n");

}

void initialize_player(Player *player)
{
	// Set initial attributes
	player->race = 0;
	player->sex = 0;
	player->strength = 2;
	player->intelligence = 8;
	player->dexterity = 14;
	player->gold = 60;
	player->flares = 0;
	player->armor_type = 0;
	player->armor_points = 0;
	player->weapon_type = 0;
	player->lamp_flag = 0;

	// Set initial position
	player->x = 1;
	player->y = 4;
	player->level = 1;

	// Set initial flags
	player->runestaff_flag = 0;
	player->orb_flag = 0;

	// Additional variables from BASIC that might be needed
	player->book_flag = 0;
	player->blindness_flag = 0;
	player->stickybook_flag = 0;
	player->has_runestaff = 0;
	player->has_orb = 0;
	player->treasure_count = 0;
	player->web_count = 0;

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
        "A BALROG", "A DRAGON", "A VENDOR"
    };
    char message[100];


    while (!game_over) {
        game->turn_count++;

       // Handle curses
        if (player->runestaff_flag == 0 && player->orb_flag == 0) {
            // Lethargy curse (similar to line 1960 in BASIC)
            if (!game->treasure[0] && random_number(100) <= 5) {  // 5% chance if no Ruby Red
                game->turn_count++;
                printf("\nYOU ARE AFFECTED BY LETHARGY. YOU LOSE A TURN.\n");
            }

            // Leech curse (similar to line 1965 in BASIC)
            if (!game->treasure[2] && random_number(100) <= 5) {  // 5% chance if no Pale Pearl
                int gold_lost = random_number(5);
                player->gold = (player->gold > gold_lost) ? player->gold - gold_lost : 0;
                printf("\nA LEECH ATTACKS YOU! YOU LOSE %d GOLD PIECES.\n", gold_lost);
            }

            // Forgetfulness curse (similar to lines 1975-2015 in BASIC)
            if (!game->treasure[4] && random_number(100) <= 5) {  // 5% chance if no Green Gem
                int old_x = player->x, old_y = player->y, old_z = player->level;
                player->x = random_number(8);
                player->y = random_number(8);
                player->level = random_number(8);
                printf("\nYOU SUDDENLY FORGET WHERE YOU ARE!\n");
                printf("YOU FIND YOURSELF AT (%d,%d) ON LEVEL %d.\n", player->x, player->y, player->level);

                // If player was in an empty room, mark old room as unexplored
                if (get_room_content(game, old_x, old_y, old_z) == EMPTY_ROOM) {
                    int index = CALCULATE_ROOM_INDEX(old_z, old_x, old_y);
                    game->discovered_rooms[index] = 0;
                }
            }
        }

        // Display random events (similar to lines 2010-2060 in BASIC)
        if (random_number(5) == 1) {
            print_message("\nYOU ");
            int event_type = random_number(7) + player->blindness_flag;
            if (event_type > 7) event_type = 4;
            
            switch (event_type) {
                case 1: print_message("SEE A BAT FLY BY!\n"); break;
                case 2: print_message("HEAR FOOTSTEPS!\n"); break;
                case 3: print_message("SNEEZED!\n"); break;
                case 4: print_message("STEPPED ON A FROG!\n"); break;
                case 5: print_message("SMELL SOMETHING FRYING!\n"); break;
                case 6: print_message("FEEL LIKE YOU'RE BEING WATCHED!\n"); break;
                case 7: print_message("HEAR FAINT RUSTLING NOISES!\n"); break;
            }
        }

        // Handle blindness cure (similar to lines 2065-2075 in BASIC)
        if (player->blindness_flag == 1 && game->treasure[3] == 1) {
            print_message("\nTHE OPAL EYE CURES YOUR BLINDNESS!\n");
            player->blindness_flag = 0;
        }

        // Handle sticky book cure (similar to lines 2080-2090 in BASIC)
        if (player->stickybook_flag == 1 && game->treasure[5] == 1) {
            print_message("\nTHE BLUE FLAME DISSOLVES THE BOOK!\n");
            player->stickybook_flag = 0;
        }

        print_message("\n");
        //print_status(player, game);
        
        int room_content = get_room_content(game, player->x, player->y, player->level);
        if (room_content == 102) {  // The Entrance
            char message[100];
            snprintf(message, sizeof(message), "OK, %s, YOU ARE NOW ENTERING THE CASTLE!\n", get_race_name(player->race));
            print_message(message);
        } else if (room_content >= EMPTY_ROOM && room_content <= TREASURE_END) {
            char message[100];
            snprintf(message, sizeof(message), "HERE YOU FIND %s.\n", room_contents[room_content - 101]);
            print_message(message);
            if (room_content==GOLD)
            {
                int gold_found = random_number(1000);  // Random amount between 1 and 1000
                player->gold += gold_found;
                char message[100];
                snprintf(message, sizeof(message), "%d GOLD PIECES HAVE BEEN ADDED TO YOUR INVENTORY!\n", gold_found);
                print_message(message);
                set_room_content(game, player->x, player->y, player->level, 101);  // Empty the room
            }
            else if (room_content==108)
            {
                int flares_found = random_number(5);  // Random amount between 1 and 5

                player->flares += flares_found;
                snprintf(message, sizeof(message), "%d FLARES HAVE BEEN ADDED TO YOUR INVENTORY!\n", flares_found);
                print_message(message);
                set_room_content(game, player->x, player->y, player->level, 101);  // Empty the room
           }
           // Monsters
           else if (room_content>=113 && room_content <=124)
           {
               fight_monster(player, game);
           }
           // Vendors
           else if (room_content==125)
           {
               handle_vendor(player, game);
           }
           // Treasure
           else if (room_content>=126 && room_content<=133)
           {
                handle_treasure(player, game, room_content); 
           }
           
        } else {
            print_message("HERE YOU FIND AN UNKNOWN ROOM.\n");
            snprintf(message, sizeof(message), "%i\n", room_content);
            print_message(message);
        }
        game_over = check_game_over(player);
        if(!game_over)
        {
            strncpy(user_command, get_user_input_main(), sizeof(user_command) - 1);
            user_command[sizeof(user_command) - 1] = '\0';  // Ensure null-termination            snprintf(message, sizeof(message), "User Command: %s\n", user_command);
            //print_message(message);
            switch (user_command[0]) {
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
                        print_message("YOU CLIMB UP THE STAIRS.\n");
                    } else {
                        print_message("THERE ARE NO STAIRS GOING UP FROM HERE!\n");
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
                            print_message("** IF YOU WANT A DRINK, FIND A POOL!");
                        }
                    }
                    else if (room_content == STAIRS_DOWN) {  // Stairs going down
                        player->level--;
                        if (player->level<1)
                        {
                            player->level=8;
                        }
                        print_message("YOU DESCEND THE STAIRS.\n");
                    } else {
                        print_message("THERE ARE NO STAIRS GOING DOWN FROM HERE!\n");
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
                        print_message("THERE'S NOTHING HERE TO OPEN!\n");
                    }
                    break;
                case 'G':
                    if (room_content == CRYSTAL_ORB) {  // Crystal orb
                        gaze_into_orb(player, game);
                    } else {
                        print_message("THERE'S NO CRYSTAL ORB HERE TO GAZE INTO!\n");
                    }
                    break;
                case 'T':
                    if (player->runestaff_flag) {
                        teleport(player, game);
                    } else {
                        print_message("YOU CAN'T TELEPORT WITHOUT THE RUNESTAFF!\n");
                    }
                    break;
                case 'Q':
                    print_message("DO YOU REALLY WANT TO QUIT NOW? (Y/N) ");
                    if (get_user_input_yn() == 'Y') {
                        game_over = 1;
                    } else {
                        print_message("OK, CONTINUE ON BRAVE ADVENTURER!\n");
                    }
                    break;
                case 'Z':
                    print_status(player, game);
                    break;
                case 'H':
                    print_help();
                    break;
                default:
                    print_message("INVALID COMMAND. TYPE 'H' FOR HELP.\n");
            }
        }
        if (!game_over) {
            game_over = check_game_over(player);
        }
    }

    end_game(player, game);
    // Ask if the player wants to play again
    print_message("\nARE YOU FOOLISH ENOUGH TO WANT TO PLAY AGAIN? ");
    char play_again = get_user_input_yn();
    if (play_again == 'Y') {
        print_message("\nSOME ADVENTURERS NEVER LEARN!\n\n");
        print_message("PLEASE BE PATIENT WHILE THE CASTLE IS RESTOCKED.\n\n");
        return 1;
    } else {
        print_message("\nGOOD BYE, AND GOOD LUCK IN YOUR TRAVELS!\n");
        return 0;        
    }
}

// Player creation and attribute functions
void choose_race(Player *player)
{
    const char *races[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    char user_input[10];
    int valid_choice = 0;
    char message[100];  // Buffer for the message

    do {
        print_message("ALL RIGHT, BOLD ONE.\n");
        print_message("YOU MAY BE AN ELF, DWARF, MAN, OR HOBBIT.\n\n");
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);

        // Convert input to uppercase
        for (int i = 0; user_input[i]; i++) {
            user_input[i] = toupper(user_input[i]);
        }

        // Remove newline character if present
        user_input[strcspn(user_input, "\n")] = 0;

        player->race = 0;

        if (user_input[0] == 'H') player->race = 1;  // Hobbit
        else if (user_input[0] == 'E') player->race = 2;  // Elf
        else if (user_input[0] == 'M') player->race = 3;  // Man/Human
        else if (user_input[0] == 'D') player->race = 4;  // Dwarf

        if (player->race > 0) {
            player->strength += 2 * player->race;
            player->dexterity -= 2 * player->race;
            if (player->race == 2) {  // Elf gets extra points
                player->intelligence += 4;
            }
            snprintf(message, sizeof(message), "You have chosen to be a %s\n", races[player->race - 1]);
            print_message(message);
            valid_choice = 1;
        } else {
            print_message("\n** THAT WAS INCORRECT. PLEASE TYPE E, D, M, OR H.\n");
        }
    } while (!valid_choice);
}

void choose_sex(Player *player)
{
    char user_input[10];
    int valid_choice = 0;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    do {
        print_message("WHICH SEX DO YOU PREFER?\n\n");
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);

        // Convert input to uppercase
        for (int i = 0; user_input[i]; i++) {
            user_input[i] = toupper(user_input[i]);
        }

        // Remove newline character if present
        user_input[strcspn(user_input, "\n")] = 0;

        switch (user_input[0]) {
            case 'M':
                player->sex = 1;
                valid_choice = 1;
                break;
            case 'F':
                player->sex = 0;
                valid_choice = 1;
                break;
            default:
                {
                    char message[100];
                    snprintf(message, sizeof(message), "** CUTE %s, REAL CUTE. TRY M OR F.\n", race_names[player->race - 1]);
                    print_message(message);
                }
                break;
        }
    } while (!valid_choice);

    print_message("\n");

}

void allocate_attributes(Player *player)
{
    int other_points;
    char user_input[10];
    int points_to_add;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    // Set other_points based on race
    if (player->race == 1) { // Hobbit
        other_points = 4;
    } else { // Elf, Human, Dwarf
        other_points = 8;
    }

    printf("OK, %s, YOU HAVE THE FOLLOWING ATTRIBUTES:\n", race_names[player->race - 1]);
    printf("STRENGTH = %d    INTELLIGENCE = %d    DEXTERITY = %d\n", 
           player->strength, player->intelligence, player->dexterity);
    printf("AND %d OTHER POINTS TO ALLOCATE AS YOU WISH.\n\n", other_points);

    // Allocate points to Strength
    while (other_points > 0) {
        print_message("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR STRENGTH? ");
        fgets(user_input, sizeof(user_input), stdin);
        points_to_add = atoi(user_input);

        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->strength += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            printf("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    // Allocate points to Intelligence
    while (other_points > 0) {
        print_message("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR INTELLIGENCE? ");
        fgets(user_input, sizeof(user_input), stdin);
        points_to_add = atoi(user_input);

        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->intelligence += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            printf("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    // Allocate remaining points to Dexterity
    if (other_points > 0) {
        printf("ALLOCATING REMAINING %d POINTS TO DEXTERITY.\n", other_points);
        player->dexterity += other_points;
    }

    print_message("\nYOUR ATTRIBUTES ARE NOW:\n");
    printf("STRENGTH = %d    INTELLIGENCE = %d    DEXTERITY = %d\n", 
           player->strength, player->intelligence, player->dexterity);
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
        print_message(error_message);
    }
}

// Movement and action functions
void move_player(Player *player, GameState *game, char direction)
{
    int current_room = get_room_content(game, player->x, player->y, player->level);
    int printstatusmessage=1;
    
    // If I cast web in a previous turn, webcount should be zero again
    player->web_count=0;

    // Check if player is at the entrance and moving north
    if (current_room == 102 && direction == 'N') {
        if (player->orb_flag) {
            print_message("Congratulations! You've escaped the castle with the Orb of Zot!\n");
            // Set a flag or call a function to end the game with victory
            game->game_over = 1;
            game->victory = 1;
            return;
        } else {
            print_message("You're at the entrance. Are you sure you want to leave without the Orb of Zot? (Y/N) ");
            char choice = get_user_input_yn();
            if (choice == 'Y') {
                print_message("You leave the castle empty-handed. Game over!\n");
                // Set a flag or call a function to end the game
                game->game_over = 1;
                return;
            } else {
                print_message("You decide to stay in the castle.\n");
                return;
            }
        }
    }

    int new_x = player->x;
    int new_y = player->y;
    switch(direction) {
        case 'N':
            new_x--;
            break;
        case 'S':
            new_x++;
            break;
        case 'W':
            new_y--;
            break;
        case 'E':
            new_y++;
            break;
        case 'T':  //teleport
            printstatusmessage=0;
            break;
        default:
            print_message("Invalid direction!\n");
            return;
    }

    // Handle wrapping around the castle edges
    new_x = (new_x - 1 + 8) % 8 + 1;  // Wrap x between 1 and 8
    new_y = (new_y - 1 + 8) % 8 + 1;  // Wrap y between 1 and 8

    // Update player position
    player->x = new_x;
    player->y = new_y;

    // Print movement message
    if (printstatusmessage==1)
    {
        char message[100];
        snprintf(message, sizeof(message), "You move %s to (%d, %d) on level %d.\n", 
                 direction == 'N' ? "North" :
                 direction == 'S' ? "South" :
                 direction == 'W' ? "West" : "East",
                 player->y, player->x, player->level);
        print_message(message);
    }
    // Check for special room events (like warp or sinkhole)
    int room_content = get_room_content(game, player->x, player->y, player->level);
    mark_room_discovered(game, player->x, player->y, player->level);
    
    if (room_content == WARP) {  // Warp
        player->x = random_number(8);
        player->y = random_number(8);
        player->level = random_number(8);
        print_message("You've been warped to a random location!\n");
    } else if (room_content == SINKHOLE) {  // Sinkhole
        if (player->level < 8) {
            player->level++;
        } else {
            player->level=1;
        }
        print_message("You've fallen through a sinkhole to the level below!\n");

    }

    // Print the new room description
}

void fight_monster(Player *player, GameState *game)
{
    int room_content = get_room_content(game, player->x, player->y, player->level);
    int is_vendor = (room_content == VENDOR);
    int enemy_strength, enemy_dexterity;
    int can_bribe = 1;
    const char *enemy_name = is_vendor ? "VENDOR" : get_monster_name(room_content);
    player->web_count=0;
    // Set enemy stats based on room content
    if (is_vendor) {
        enemy_strength = 15;
        enemy_dexterity = 15;
    } else {
        enemy_strength = (room_content - MONSTER_START + 1) * 2;
        enemy_dexterity = room_content - MONSTER_START + 8;
    }

    while (1) {
        printf("\nYOU'RE FACING %s!\n\n", enemy_name);
        print_message("YOU MAY (A)TTACK OR (R)ETREAT.\n");
        if (can_bribe) {
            print_message("YOU CAN ALSO ATTEMPT A (B)RIBE.\n");
        }
        if (player->intelligence > 14) {
            print_message("YOU CAN ALSO (C)AST A SPELL.\n");
        }
        print_message("\n");
        printf("YOUR STRENGTH IS %d, YOUR DEXTERITY IS %d, AND YOUR INTELLIGENCE IS %d.\n", 
               player->strength, player->dexterity, player->intelligence);

        char choice = get_user_input();

        switch (choice) {
            case 'A':
                if (player->weapon_type == 0) {
                    printf("\n** POUNDING ON %s WON'T HURT IT!\n", enemy_name);
                } else if (player->stickybook_flag) {
                    print_message("\n** YOU CAN'T BEAT IT TO DEATH WITH A BOOK!\n");
                } else if (random_number(20) + player->dexterity <= random_number(20) + (3 * player->blindness_flag)) {
                    print_message("\nYOU MISSED, TOO BAD!\n");
                } else {
                    printf("\nYOU HIT THE EVIL %s!\n", enemy_name);
                    enemy_strength -= player->weapon_type;
                    if ((room_content == GARGOYLE || room_content == DRAGON) && random_number(8) == 1) {
                        printf("\nOH NO! YOUR %s BROKE!\n", get_weapon_name(player->weapon_type));
                        player->weapon_type = 0;
                    }
                    if (enemy_strength <= 0) {
                        handle_combat_victory(player, game, is_vendor, enemy_name);
                        return;
                    }
                }
                break;

            case 'R':
                if (random_number(20) + player->dexterity > random_number(20) + enemy_dexterity) {
                    print_message("\nYOU HAVE ESCAPED!\n");
                    move_player_randomly(player, game);
                    return;
                } else {
                    print_message("\nYOU FAILED TO RETREAT!\n");
                }
                break;

            case 'B':
                if (!can_bribe) {
                    print_message("\n** CHOOSE ONE OF THE OPTIONS LISTED.\n");
                    continue;
                }
                if (handle_bribe(player, game, enemy_name)) {
                    return;
                }
                can_bribe = 0;
                break;

            case 'C':
                if (player->intelligence <= 14) {
                    print_message("\n** YOU CAN'T CAST A SPELL NOW!\n");
                    continue;
                }
                if (handle_spell(player, game, &enemy_strength, enemy_name)) {
                    return;
                }
                break;

            default:
                print_message("\n** CHOOSE ONE OF THE OPTIONS LISTED.\n");
                continue;
        }

        // Enemy's turn
        if (choice != 'R' || (choice == 'R' && random_number(20) + player->dexterity <= random_number(20) + enemy_dexterity)) {
            if (player->web_count > 0) {
                player->web_count--;
                if (player->web_count == 0) {
                    print_message("\nTHE WEB JUST BROKE!\n");
                } else {
                    printf("\nTHE %s IS STUCK AND CAN'T ATTACK NOW!\n", enemy_name);
                    continue;  // Skip the enemy's attack
                }
            }

            printf("\nTHE %s ATTACKS!\n", enemy_name);
            if (random_number(7) + random_number(7) + random_number(7) + 3 * player->blindness_flag >= player->dexterity) {
                print_message("\nOUCH! HE HIT YOU!\n");
                int damage = (enemy_strength / 2) + 1;
                
                // Apply armor reduction
                if (player->armor_type != 0) {
                    damage -= player->armor_type;
                    player->armor_points -= player->armor_type;
                    if (damage < 0) {
                        player->armor_points -= damage;  // Absorb excess damage
                        damage = 0;
                    }
                    if (player->armor_points < 0) {
                        player->armor_points = 0;
                        player->armor_type = 0;
                        print_message("\nYOUR ARMOR HAS BEEN DESTROYED... GOOD LUCK!\n");
                    }
                }
                
                player->strength -= damage;
                printf("YOU TAKE %d DAMAGE!\n", damage);
                
                if (player->strength <= 0) {
                    print_message("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                    game->game_over = 1;
                    return;
                }
            } else {
                print_message("\nWHAT LUCK, HE MISSED YOU!\n");
            }
        }
    }
}
void handle_combat_victory(Player *player, GameState *game, int is_vendor, const char *enemy_name)
{
    printf("\n%s LIES DEAD AT YOUR FEET!\n", enemy_name);
    
    if (random_number(5) == 1) {  // 20% chance of eating
        printf("\nYOU SPEND AN HOUR EATING %s%s.\n", enemy_name, get_random_body_part());
    }

    if (is_vendor) {
        print_message("\nYOU GET ALL HIS WARES:\n");
        print_message("PLATE ARMOR\n");
        player->armor_type = 3;
        player->armor_points = 21;
        print_message("A SWORD\n");
        player->weapon_type = 3;
        print_message("A STRENGTH POTION\n");
        player->strength = min(player->strength + random_number(6), 18);
        print_message("AN INTELLIGENCE POTION\n");
        player->intelligence = min(player->intelligence + random_number(6), 18);
        print_message("A DEXTERITY POTION\n");
        player->dexterity = min(player->dexterity + random_number(6), 18);
        if (!player->lamp_flag) {
            print_message("A LAMP\n");
            player->lamp_flag = 1;
        }
    } else {
        // Check if this was the room with the Runestaff
        if (player->x == game->runestaff_location[0] &&
            player->y == game->runestaff_location[1] &&
            player->level == game->runestaff_location[2]) {
            print_message("\nGREAT ZOT! YOU'VE FOUND THE RUNESTAFF!\n");
            player->runestaff_flag = 1;
            game->runestaff_location[0] = 0;  // Mark as found
        }
    }

    int gold_found = random_number(1000);
    printf("\nYOU NOW GET HIS HOARD OF %d GP'S!\n", gold_found);
    player->gold += gold_found;

    // Clear the room
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
}

int handle_bribe(Player *player, GameState *game, const char *enemy_name)
{
    if (player->treasure_count == 0) {
        print_message("\nALL I WANT IS YOUR LIFE!\n");
        return 0;
    }

    int treasure_index = random_number(8) - 1;
    while (!game->treasure[treasure_index]) {
        treasure_index = random_number(8) - 1;
    }

    printf("\nI WANT %s. WILL YOU GIVE IT TO ME? ", get_treasure_name(treasure_index));
    char choice = get_user_input_yn();

    if (choice == 'Y') {
        game->treasure[treasure_index] = 0;
        player->treasure_count--;
        print_message("\nOK, JUST DON'T TELL ANYONE ELSE.\n");
        if (strcmp(enemy_name, "VENDOR") == 0) {
            game->vendor_attacked = 1;  // Vendor won't trade anymore
        }
        return 1;
    }
    return 0;
}

int handle_spell(Player *player, GameState *game, int *enemy_strength, const char *enemy_name)
{
    print_message("\nWHICH SPELL (WEB, FIREBALL, DEATHSPELL)? \n\n");
    char spell = get_user_input();

    switch (spell) {
        case 'W':
            player->strength--;
            if (player->strength <= 0) {
                game->game_over = 1;
                return 1;
            }
            player->web_count = random_number(8) + 1;  // Set web count to 1-9 turns
            printf("\nTHE %s IS STUCK AND CAN'T ATTACK FOR %d TURNS!\n", enemy_name, player->web_count);
            return 0;
        case 'F':
            player->strength--;
            player->intelligence--;
            if (player->strength <= 0 || player->intelligence <= 0) {
                game->game_over = 1;
                return 1;
            }
            int damage = random_number(7) + random_number(7);
            printf("\nIT DOES %d POINTS WORTH OF DAMAGE.\n", damage);
            *enemy_strength -= damage;
            if (*enemy_strength <= 0) {
                handle_combat_victory(player, game, 0, enemy_name);
                return 1;
            }
            return 0;

        case 'D':
            print_message("\nDEATH . . . ");
            if (player->intelligence < random_number(4) + 15) {
                print_message("YOURS!\n");
                player->intelligence = 0;
                game->game_over = 1;
                return 1;
            } else {
                print_message("HIS!\n");
                handle_combat_victory(player, game, 0, enemy_name);
                return 1;
            }

        default:
            print_message("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
            return 0;
    }
}

void move_player_randomly(Player *player, GameState *game) 
{
    char directions[] = {'N', 'S', 'E', 'W'};
    char direction = directions[random_number(4) - 1];
    move_player(player, game, direction);
}

const char* get_weapon_name(int weapon_type)
{
    const char* weapon_names[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD"};
    return weapon_names[weapon_type];
}

const char* get_random_body_part()
{
    const char* body_parts[] = {" SANDWICH", " STEW", " SOUP", " BURGER", " ROAST", " FILET", " TACO", " PIE"};
    return body_parts[random_number(8) - 1];
}

const char* get_random_species()
{
    const char* body_parts[] = {"ELF", "HUMAN", "DWARF", "HOBBIT"};
    return body_parts[random_number(4) - 1];
}


void handle_vendor(Player *player, GameState *game)
{
    print_message("\nYou've encountered a vendor!\n");
    
    // Check if the player has attacked the vendor before
    if (game->vendor_attacked) {
        fight_monster(player, game);
        return;
    }

    char choice;
    do {
        print_message("\nDo you want to (T)rade, (A)ttack, or (I)gnore the vendor?\n");
        choice = get_user_input();

        switch(choice) {
            case 'T':
                trade_with_vendor(player, game);
                return;
            case 'A':
                attack_vendor(player, game);
                return;
            case 'I':
                print_message("You ignore the vendor and move on.\n");
                return;
            default:
                print_message("Invalid choice. Please choose T, A, or I.\n");
        }
    } while (1);
}

void trade_with_vendor(Player *player, GameState *game)
{
    // Offer to buy treasures
    for (int i = 0; i < TREASURE_COUNT; i++) {
        if (game->treasure[i]) {
            int offer = random_number(1500) * (i + 1);
            char message[100];
            snprintf(message, sizeof(message), "Do you want to sell %s for %d GP? (Y/N) ", 
                     get_treasure_name(i), offer);
            print_message(message);
            
            if (get_user_input_yn() == 'Y') {
                player->gold += offer;
                game->treasure[i] = 0;
                player->treasure_count--;
                print_message("Sold!\n");
            }
        }
    }

    // Offer to sell items if player has enough gold
    while (player->gold >= 10) {
        print_message("\n\nWhat would you like to buy?\n");
        print_message("    1. Improve Strength (1000 GP)\n");
        print_message("    2. Improve Intelligence (1000 GP)\n");
        print_message("    3. Improve Dexterity (1000 GP)\n");
        print_message("    4. Armor (1250-2000 GP)\n");
        print_message("    5. Weapon (1250-2000 GP)\n");
        print_message("    6. Lamp (1000 GP)\n");
        print_message("    7. Flares (10 GP)\n");
        print_message("    8. Nothing more\n\n");

        char purchase_choice = get_user_input();
        switch(purchase_choice) {
            case '1':
                if(player->gold>=1000)
		{
                    if (player->strength < 18)
                    {
                        player->strength = min(player->strength + random_number(6), 18);
                        player->gold -= 1000;
                    }
                    else
                    {
                        print_message("\nYOU ARE ALREADY HERCULES\n");
                    }
                }
                else
                {
                    print_message("\nYOU ARE TOO POOR TO BUY A STRENGTH POTION\n");
                }
                break;
            case '2':
                if(player->gold>=1000)
		{
                    if (player->intelligence < 18)
                    {
                        player->intelligence = min(player->strength + random_number(6), 18);
                        player->gold -= 1000;
                    }
                    else
                    {
                        print_message("\nYOU ARE ALREADY A GENIUS ... GENIUS\n");
                    }
                }
                else
                {
                    print_message("\nYOU ARE TOO POOR TO BUY A STRENGTH POTION\n");
                }
                break;
            case '3':
                if(player->gold>=1000)
		{
                    if (player->dexterity < 18)
                    {
                        player->dexterity = min(player->strength + random_number(6), 18);
                        player->gold -= 1000;
                    }
                    else
                    {
                        print_message("\nYOU ARE ALREADY NIMBLE ... GENIUS\n");
                    }
                }
                else
                {
                    print_message("\nYOU ARE TOO POOR TO BUY A STRENGTH POTION\n");
                }
                break;
            case '4':
                if (player->gold<1250)
                {
                    print_message("\nYOU DO NOT HAVE ENOUGH GOLD TO BUY ARMOR!\n");
                }
                else
                {
                    buy_armor(player);
                }
                break;
            case '5':
                if (player->gold<1250)
                {
                    print_message("\nYOU DO NOT HAVE ENOUGH GOLD TO BUY WEAPONS!\n");
                }
                else
                {
                    buy_weapon(player);
                }
                break;
            case '6':
                if (!player->lamp_flag && player->gold >= 1000) {
                    player->lamp_flag = 1;
                    player->gold -= 1000;
                    print_message("You bought a lamp!\n");
                } else if (player->lamp_flag) {
                    print_message("\nYou already have a lamp!\n");
                    continue;
                } else {
                    print_message("\nYou don't have enough gold for a lamp.\n");
                    continue;
                }
                break;
            case '7':
                buy_flares(player);
                break;
            case '8':
                print_message("\nOK FINE, DON'T TRADE\n");
                return;
            default:
                print_message("Invalid choice.\n");
                continue;
        }

        //print_message("Purchase successful!\n");
    }

    print_message("You don't have enough gold for any more purchases.\n");
}

void attack_vendor(Player *player, GameState *game)
{
    print_message("You attack the vendor!\n");
    game->vendor_attacked = 1;

    // Call fight_monster to handle the combat
    fight_monster(player, game);
}

void buy_flares(Player *player)
{
    int max_flares = player->gold / 10;
    char message[100];
    snprintf(message, sizeof(message), "How many flares do you want to buy? (0-%d): ", max_flares);
    print_message(message);

    int flares_to_buy = get_user_input_number();

    if (flares_to_buy < 0 || flares_to_buy > max_flares) {
        print_message("Invalid number of flares.\n");
        return;
    }

    player->flares += flares_to_buy;
    player->gold -= flares_to_buy * 10;

    snprintf(message, sizeof(message), "You bought %d flares for %d GP.\n", flares_to_buy, flares_to_buy * 10);
    print_message(message);
}

void buy_armor(Player *player)
{
    print_message("\n\nArmor Options:\n");
    print_message("    1. Leather (1250 GP)\n");
    print_message("    2. Chainmail (1500 GP)\n");
    print_message("    3. Plate (2000 GP)\n");
    print_message("    4. Nothing\n\n");

    char armor_choice = get_user_input();
    switch(armor_choice) {
        case '1':
            if (player->gold >= 1250) {
                if (player->armor_type < 1)
                {
                    player->armor_type = 1;
                    player->armor_points = 7;
                    player->gold -= 1250;
                    print_message("\nArmor purchased successfully!\n");
                }
                else
                {

                    print_message("\nYOUR ARMOR IS ALREADY SUPERIOR BUT I WILL REPAIR YOUR ARMOR!!!   ON THE HOUSE\n\n");
                    player->armor_points = player->armor_type*7;
                }
            } else {
                print_message("Not enough gold for Leather Armor.\n");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                if(player->armor_type < 2)
                {
                    player->armor_type = 2;
                    player->armor_points = 14;
                    player->gold -= 1500;
                    print_message("\nArmor purchased successfully!\n");

                }
                else
                {
                    print_message("\nYOUR ARMOR IS ALREADY SUPERIOR BUT I WILL REPAIR YOUR ARMOR!!!   ON THE HOUSE\n\n");
                    player->armor_points = player->armor_type*7;
                }
            } else {
                print_message("Not enough gold for Chainmail.\n");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                if (player->armor_type<3)
                {
                    player->armor_type = 3;
                    player->armor_points = 21;
                    player->gold -= 2000;
                    print_message("Armor purchased successfully!\n");

                }
                else
                {
                    print_message("YOUR ARMOR IS ALREADY SUPERIOR BUT I WILL REPAIR YOUR ARMOR!!!   ON THE HOUSE\n\n");
                    player->armor_points = player->armor_type*7;
                }
                
            } else {
                print_message("Not enough gold for Plate Armor.\n");
                return;
            }
            break;
        case '4':
            return;
        default:
            print_message("Invalid choice. No armor purchased.\n");
            return;
    }
}

void buy_weapon(Player *player)
{
    print_message("\nWeapon Options:\n");
    print_message("1. Dagger (1250 GP)\n");
    print_message("2. Mace (1500 GP)\n");
    print_message("3. Sword (2000 GP)\n");
    print_message("4. Nothing\n");

    char weapon_choice = get_user_input();
    switch(weapon_choice) {
        case '1':
            if (player->gold >= 1250) {
                if (player->weapon_type < 1)
                {
                    player->weapon_type = 1;
                    player->gold -= 1250;
                    print_message("Weapon purchased successfully!\n");
                }
                else
                {
                    print_message("YOUR WEAPON IS ALREADY SUPERIOR!\n");
                }
            } else {
                print_message("Not enough gold for a Dagger.\n");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                if (player->weapon_type <2)
                {
                    player->weapon_type = 2;
                    player->gold -= 1500;
                    print_message("Weapon purchased successfully!\n");

                }
                else
                {
                    print_message("YOUR WEAPON IS ALREADY SUPERIOR!\n");
                }                
            } else {
                print_message("Not enough gold for a Mace.\n");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                if (player->weapon_type < 3)
                {
                    player->weapon_type = 3;
                    player->gold -= 2000;
                    print_message("Weapon purchased successfully!\n");
                }
                else
                {
                    print_message("YOUR WEAPON IS ALREADY SUPERIOR!\n");                
                }
                
            } else {
                print_message("Not enough gold for a Sword.\n");
                return;
            }
            break;
        case '4':
            return;
        default:
            print_message("Invalid choice. No weapon purchased.\n");
            return;
    }
    
}

// Helper function to get treasure names
const char* get_treasure_name(int index)
{
    static const char* treasure_names[] = {
        "Ruby Red", "Norn Stone", "Pale Pearl", "Opal Eye",
        "Green Gem", "Blue Flame", "Palantir", "Silmaril"
    };
    return treasure_names[index];
}

// Helper function for minimum of two integers
int min(int a, int b)
{
    return (a < b) ? a : b;
}

// Item and treasure functions
void buy_equipment(Player *player)
{
    char user_input[10];
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    const char *armor_types[] = {"NO ARMOR", "LEATHER", "CHAINMAIL", "PLATE"};
    const char *weapon_types[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD"};
    int cost;
    char message[100];

    // Buy Armor
    print_message("\nOK, ");
    print_message(race_names[player->race - 1]);

    printf(", YOU HAVE %d GOLD PIECES (GP'S).\n\n", player->gold);
    
    print_message("THESE ARE THE TYPES OF ARMOR YOU CAN BUY :\n");
    print_message("PLATE<30> CHAINMAIL<20> LEATHER<10> NOTHING<0>\n\n");
    
    do {
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);
        user_input[0] = toupper(user_input[0]);

        switch(user_input[0]) {
            case 'P': player->armor_type = 3; cost = 30; break;
            case 'C': player->armor_type = 2; cost = 20; break;
            case 'L': player->armor_type = 1; cost = 10; break;
            case 'N': player->armor_type = 0; cost = 0; break;
            default:
                printf("\n** ARE YOU A %s OR A FOOL? TRY AGAIN.\n\n", race_names[player->race - 1]);
                continue;
        }
        break;
    } while (1);

    player->gold -= cost;
    player->armor_points = player->armor_type * 7;

    // Buy Weapon
    snprintf(message, sizeof(message), "\nOK, %s, YOU HAVE %d GP'S LEFT.\n\n", race_names[player->race - 1], player->gold);
    print_message(message);
    print_message("THESE ARE THE TYPES OF WEAPONS YOU CAN BUY :\n");
    print_message("SWORD<30> MACE<20> DAGGER<10> NOTHING<0>\n\n");
    
    do {
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);
        user_input[0] = toupper(user_input[0]);

        switch(user_input[0]) {
            case 'S': 
                if (player->gold < 30) {
                    print_message("** YOUR DUNGEON EXPRESS CARD - YOU LEFT HOME WITHOUT IT!\n\n");
                    continue;
                }
                player->weapon_type = 3; cost = 30; break;
            case 'M': 
                if (player->gold < 20) {
                    print_message("** SORRY SIR, I'M AFRAID I DON'T GIVE CREDIT!\n\n");
                    continue;
                }
                player->weapon_type = 2; cost = 20; break;
            case 'D': player->weapon_type = 1; cost = 10; break;
            case 'N': player->weapon_type = 0; cost = 0; break;
            default:
                print_message("** TRY AGAIN, YOUR CHOICE MUST BE S, M, D, OR N.\n\n");
                continue;
        }
        break;
    } while (1);

    player->gold -= cost;

    printf("\nYOU NOW HAVE %s ARMOR AND A %s.\n", 
           armor_types[player->armor_type], weapon_types[player->weapon_type]);
    printf("YOU HAVE %d GOLD PIECES LEFT.\n", player->gold);

}

void buy_lamp_and_flares(Player *player)
{
    char user_input_yn, user_input[10];
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    // Try to buy a lamp
    if (player->gold >= 20 && !player->lamp_flag) {
        print_message("\nDO YOU WANT TO BUY A LAMP FOR 20 GP'S?\n");
        do {
            print_message("YOUR CHOICE (Y/N):  ");
            user_input_yn = get_user_input_yn();
            if (user_input_yn == 'Y') {
                player->lamp_flag = 1;
                player->gold -= 20;
                print_message("\nOK, LAMP PURCHASED. IT'S GUARANTEED TO OUTLIVE YOU!\n");
                break;
            } else if (user_input_yn == 'N') {
                break;
            } else {
                print_message("** PLEASE ANSWER YES OR NO.\n");
            }
        } while (1);
    }

   // Try to buy flares
    if (player->gold >= 1) {
        printf("\nOK, %s, YOU HAVE %d GOLD PIECES LEFT.\n", race_names[player->race - 1], player->gold);
        print_message("FLARES COST 1 GP EACH. HOW MANY DO YOU WANT? ");
        
        int flares_to_buy;
        char *endptr;
        do {
            fgets(user_input, sizeof(user_input), stdin);
            user_input[strcspn(user_input, "\n")] = 0; // Remove newline

            // Check if the input is "0" to explicitly buy no flares
            if (strcmp(user_input, "0") == 0) {
                print_message("YOU CHOSE NOT TO BUY ANY FLARES.\n");
                return;
            }

            // Convert input to integer and check for errors
            flares_to_buy = strtol(user_input, &endptr, 10);

            if (*endptr != '\0') {
                print_message("** INVALID INPUT. PLEASE ENTER A NUMBER OR 0 TO BUY NO FLARES.\n");
            } else if (flares_to_buy < 0) {
                print_message("** PLEASE ENTER A NON-NEGATIVE NUMBER.\n");
            } else if (flares_to_buy > player->gold) {
                printf("** YOU CAN ONLY AFFORD %d. PLEASE ENTER A LOWER NUMBER.\n", player->gold);
            } else {
                player->flares += flares_to_buy;
                player->gold -= flares_to_buy;
                printf("\nOK, YOU NOW HAVE %d FLARES AND %d GOLD PIECES LEFT.\n", player->flares, player->gold);
                return;
            }
            print_message("HOW MANY FLARES DO YOU WANT? (OR ENTER 0 TO BUY NONE) ");
        } while (1);
    }
}

void use_lamp(Player *player, GameState *game)
{
    if (!player->lamp_flag) {
        print_message("You don't have a lamp!\n");
        return;
    }

    print_message("Which direction do you want to shine the lamp? (N/S/E/W) ");
    char direction = get_user_input();
    int dx = 0, dy = 0;

    switch(direction) {
        case 'N': dx = -1; break;
        case 'S': dx = 1; break;
        case 'W': dy = -1; break;
        case 'E': dy = 1; break;
        default:
            print_message("Invalid direction!\n");
            return;
    }

    int x = WRAP_COORDINATE(player->x + dx);
    int y = WRAP_COORDINATE(player->y + dy);
    
    mark_room_discovered(game, x, y, player->level);
    int room_content = get_room_content(game, x, y, player->level);
    char symbol = get_room_symbol(room_content);

    print_message("\nThe lamp reveals: ");
    char room_str[4] = " ? ";
    room_str[1] = symbol;
    print_message(room_str);
    print_message("\n");
}

void use_flare(Player *player, GameState *game)
{
    if (player->flares <= 0) {
        print_message("You don't have any flares!\n");
        return;
    }

    player->flares--;
    print_message("\nYou light a flare. It illuminates the surrounding rooms:\n\n");

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int x = WRAP_COORDINATE(player->x + dx);
            int y = WRAP_COORDINATE(player->y + dy);
            
            mark_room_discovered(game, x, y, player->level);
            int room_content = get_room_content(game, x, y, player->level);
            char symbol = get_room_symbol(room_content);

            if (dx == 0 && dy == 0) {
                print_message("[@]");
            } else {
                char room_str[4] = " ? ";
                room_str[1] = symbol;
                print_message(room_str);
            }
        }
        print_message("\n");
    }
}

void open_chest(Player *player, GameState *game)
{
    print_message("\nYou open the chest and ");

    int event = random_number(4);
    switch(event) {
        case 1:
            print_message("KABOOM! IT EXPLODES!!\n");
            int damage = random_number(6);
            player->strength -= damage;
            printf("You take %d damage.\n", damage);
            if (player->strength <= 0) {
                print_message("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                game->game_over = 1;
            }
            break;

        case 2:
        case 4:
            {
                int gold = random_number(1000);
                printf("find %d gold pieces!\n", gold);
                player->gold += gold;
            }
            break;

        case 3:
            print_message("GAS!! YOU STAGGER FROM THE ROOM!\n");
            game->turn_count += 20;  // Equivalent to T = T + 20 in BASIC
            // Move player in a random direction
            char directions[] = {'N', 'S', 'E', 'W'};
            char direction = directions[random_number(4) - 1];
            move_player(player, game, direction);
            break;
    }

    // Remove the chest from the room
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
}

void drink_from_pool(Player *player, GameState *game)
{
    print_message("\nYou take a drink and ");

    int effect = random_number(8);
    switch(effect) {
        case 1:
            player->strength = min(18, player->strength + random_number(3));
            print_message("feel STRONGER.\n");
            break;
        case 2:
            player->strength -= random_number(3);
            print_message("feel WEAKER.\n");
            if (player->strength <= 0) {
                print_message("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                game->game_over = 1;
            }
            break;
        case 3:
            player->intelligence = min(18, player->intelligence + random_number(3));
            print_message("feel SMARTER.\n");
            break;
        case 4:
            player->intelligence -= random_number(3);
            print_message("feel DUMBER.\n");
            if (player->intelligence <= 0) {
                print_message("\nYOU DIED DUE TO LACK OF INTELLIGENCE.\n");
                game->game_over = 1;
            }
            break;
        case 5:
            player->dexterity = min(18, player->dexterity + random_number(3));
            print_message("feel NIMBLER.\n");
            break;
        case 6:
            player->dexterity -= random_number(3);
            print_message("feel CLUMSIER.\n");
            if (player->dexterity <= 0) {
                print_message("\nYOU DIED DUE TO LACK OF DEXTERITY.\n");
                game->game_over = 1;
            }
            break;
        case 7:
            {
                int new_race;
                do {
                    new_race = random_number(4);
                } while (new_race == player->race);
                player->race = new_race;
                printf("become a %s.\n", get_race_name(player->race));
            }
            break;
        case 8:
            player->sex = 1 - player->sex;  // Toggle between 0 and 1
            printf("turn into a %s %s!\n", 
                   player->sex ? "MALE" : "FEMALE", 
                   get_race_name(player->race));
            break;
    }

    // The pool remains in the room, so we don't remove it
}



void teleport(Player *player, GameState *game)
{
    if (!player->runestaff_flag) {
        print_message("\n** YOU CAN'T TELEPORT WITHOUT THE RUNESTAFF!\n");
        return;
    }

    int new_x, new_y, new_level;

    print_message("\nEnter X-coordinate (1-8): ");
    new_x=get_user_input_number();
    if (new_x < 1 || new_x > 8) {
        print_message("Invalid coordinate. Teleportation failed.\n");
        return;
    }

    print_message("Enter Y-coordinate (1-8): ");
    new_y=get_user_input_number();
        if (new_y < 1 || new_y > 8) {
        print_message("Invalid coordinate. Teleportation failed.\n");
        return;
    }

    print_message("Enter Z-coordinate (level 1-8): ");
    new_level=get_user_input_number();
        if (new_level < 1 || new_level > 8) {
        print_message("Invalid level. Teleportation failed.\n");
        return;
    }


    // X and Y are switched
    player->x = new_y;
    player->y = new_x;
    player->level = new_level;

    printf("\nYou have teleported to (%d, %d) on level %d.\n", player->y, player->x, player->level);

    // Check if the player teleported to the Orb of Zot
    if (player->x == game->orb_location[0] && 
        player->y == game->orb_location[1] && 
        player->level == game->orb_location[2]) {
        print_message("\nGREAT UNMITIGATED ZOT!\n");
        print_message("\nYOU JUST FOUND ***THE ORB OF ZOT***!\n");
        print_message("\nTHE RUNESTAFF HAS DISAPPEARED!\n");
        player->runestaff_flag = 0;
        player->orb_flag = 1;
        game->orb_location[0] = 0;  // Mark as found
    }
    move_player(player, game, 'T');
   
}

void gaze_into_orb(Player *player, GameState *game)
{
    if (get_room_content(game, player->x, player->y, player->level) != CRYSTAL_ORB) {
        print_message("\n** IT'S HARD TO GAZE WITHOUT AN ORB!\n");
        return;
    }

    print_message("\nYou gaze into the crystal orb and see ");

    int vision = random_number(6);
    switch(vision) {
        case 1:
            print_message("yourself in a bloody heap!\n");
            player->strength -= random_number(2);
            if (player->strength <= 0) {
                print_message("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                game->game_over = 1;
            }
            break;
        case 2:
            printf("yourself drinking from a pool and becoming %s!\n", 
                   get_monster_name(MONSTER_START + random_number(12) - 1));
            break;
        case 3:
            printf("%s gazing back at you!\n", 
                   get_monster_name(MONSTER_START + random_number(12) - 1));
            break;
        case 4:
            {
                int x = random_number(8);
                int y = random_number(8);
                int z = random_number(8);
                int content = get_room_content(game, x, y, z);
                char room_desc[100];  // Adjust size as needed
                get_room_description(content, room_desc);
                printf("%s at (%d,%d) Level %d.\n", room_desc, x, y, z);
            }
            break;
        case 5:
            {
                int x, y, z;
                if (random_number(8) < 4) {
                    x = game->orb_location[0];
                    y = game->orb_location[1];
                    z = game->orb_location[2];
                } else {
                    x = random_number(8);
                    y = random_number(8);
                    z = random_number(8);
                }
                printf("***THE ORB OF ZOT*** at (%d,%d) Level %d!\n", x, y, z);
            }
            break;
        case 6:
            print_message("a soap opera rerun!\n");
            break;
    }
}

// Utility functions
int random_number(int max_value)
{
    return 1 + rand() % max_value;
}

void print_status(Player *player, GameState *game)
{
    char message[256];  // Buffer for formatting messages

    print_message("\n=== PLAYER STATUS ===\n");

    // Print player race and attributes
    snprintf(message, sizeof(message), "Race: %s\n", get_race_name(player->race));
    print_message(message);
    snprintf(message, sizeof(message), "Strength: %d  Intelligence: %d  Dexterity: %d\n",
             player->strength, player->intelligence, player->dexterity);
    print_message(message);

    // Print player position
    snprintf(message, sizeof(message), "Location: Level %d, Room (%d, %d)\n",
             player->level, player->y, player->x);
    print_message(message);

    // Print player inventory
    snprintf(message, sizeof(message), "Gold Pieces: %d  Flares: %d\n",
             player->gold, player->flares);
    print_message(message);

    // Print armor and weapon
    const char* armor_types[] = {"No Armor", "Leather", "Chainmail", "Plate"};
    const char* weapon_types[] = {"No Weapon", "Dagger", "Mace", "Sword"};
    snprintf(message, sizeof(message), "Armor: %s  Weapon: %s\n",
             armor_types[player->armor_type], weapon_types[player->weapon_type]);
    print_message(message);

    // Print special items
    print_message("Special Items: ");
    if (player->lamp_flag) print_message("Lamp ");
    if (player->runestaff_flag) print_message("Runestaff ");
    if (player->orb_flag) print_message("Orb of Zot ");
    print_message("\n");

    // Print curses or blessings
    print_message("Status Effects: ");
    if (player->blindness_flag) print_message("Blind ");
    if (player->stickybook_flag) print_message("Sticky Book ");
    print_message("\n");

    // Print number of treasures
    snprintf(message, sizeof(message), "Treasures Found: %d\n", player->treasure_count);
    print_message(message);
    for (int i=0; i<TREASURE_END-TREASURE_START; i++)
    {
        if(game->treasure[i]==1)
        {
        	print_message("     ");
        	print_message(get_treasure_name(i));
        	print_message("\n");
        }
    }

    print_message("======================\n\n");
}

void display_map(GameState *game, Player *player)
{
    print_message("\n=== MAP OF LEVEL ");
    char level_str[3], number_str[3];
    snprintf(level_str, sizeof(level_str), "%d", player->level);
    print_message(level_str);
    print_message(" ===\n\n");

    // Print top border with column coordinates
    print_message("       1        2        3        4        5        6        7        8     \n");
    print_message("  +--------+--------+--------+--------+--------+--------+--------+--------+\n");

    for (int x = 1; x <= 8; x++) {
        // Print row coordinate
        snprintf(number_str, sizeof(number_str), "%d", x);

        print_message(number_str);

        for (int y = 1; y <= 8; y++) {
            print_message("|");
            if (x == player->x && y == player->y) {
                print_message("  [YOU] ");
            } else if (is_room_discovered(game, x, y, player->level)) {
                int room_content = get_room_content(game, x, y, player->level);
                char room_str[9] = "        \0";
                get_room_description(room_content, room_str);
                print_message(room_str);
            } else {
                print_message("????????");
            }
        }
        print_message("|\n");

        // Print horizontal border between rows
        if (x < 8) {
            print_message("  +--------+--------+--------+--------+--------+--------+--------+--------+\n");
        }
    }

    // Print bottom border
    print_message("  +--------+--------+--------+--------+--------+--------+--------+--------+\n");

    print_message("\nLEGEND:\n");
    print_message("[YOU]    = Your location   EMPTY    = Empty room     ENTRANCE = Entrance\n");
    print_message("POOL     = Magic Pool      CHEST    = Treasure Chest\n");
    print_message("GOLD     = Gold Pieces     FLARES   = Flares\n");
    print_message("WARP     = Warp/Orb        SINKHOLE = Sinkhole\n");
    print_message("CRYSTAL  = Crystal Orb     BOOK     = Magic Book\n");
    print_message("MONSTER  = Monster Name    VENDOR   = Vendor\n");
    print_message("TREASURE = Treasure Name   ???????? = Undiscovered\n");
    print_message("STAIRS UP= Stairs U        STAIRS D = Stairs Down\n");
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

    print_message("THE CONTENTS OF ROOMS ARE AS FOLLOWS:\n\n");
    print_message(". = EMPTY ROOM      B = BOOK            C = CHEST\n");
    print_message("D = STAIRS DOWN     E = ENTRANCE/EXIT   F = FLARES\n");
    print_message("G = GOLD PIECES     M = MONSTER         O = CRYSTAL ORB\n");
    print_message("P = MAGIC POOL      S = SINKHOLE        T = TREASURE\n");
    print_message("U = STAIRS UP       V = VENDOR          W = WARP/ORB\n\n");

    print_message("THE BENEFITS OF HAVING TREASURES ARE:\n\n");
    print_message("RUBY RED    - AVOID LETHARGY     PALE PEARL  - AVOID LEECH\n");
    print_message("GREEN GEM   - AVOID FORGETTING   OPAL EYE    - CURES BLINDNESS\n");
    print_message("BLUE FLAME  - DISSOLVES BOOKS    NORN STONE  - NO BENEFIT\n");
    print_message("PALANTIR    - NO BENEFIT         SILMARIL    - NO BENEFIT\n\n");

    print_message("PRESS ENTER TO CONTINUE...");
    while (getchar() != '\n');  // Wait for Enter key
}

// Game ending functions
int check_game_over(Player *player) {
    // Check if player has died
    if (player->strength <= 0 || player->intelligence <= 0 || player->dexterity <= 0) {
        return 1;
    }
    
    // Check if player has won (e.g., found the Orb of Zot and exited)
    if (player->orb_flag && player->x == 1 && player->y == 4 && player->level == 1) {
        return 1;
    }
    
    // Game is not over
    return 0;
}

char* get_user_input_main() {
    static char input[100];  // Buffer to store user input
    while (1) {
        print_message("ENTER YOUR COMMAND: ");
        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            print_message("Error reading input. Please try again.\n");
            continue;
        }
        // Remove newline character if present
        input[strcspn(input, "\n")] = '\0';
        // Convert input to uppercase
        for (int i = 0; input[i]; i++) {
            input[i] = toupper((unsigned char)input[i]);
        }
        // Check if input is empty
        if (input[0] == '\0') {
            print_message("\n\nPlease enter a command.\n\n");
            continue;
        }
        // Get the first character
        char firstChar = input[0];
        // Validate commands
        if (firstChar == 'D' && input[1] == 'R') {
            return "DR";  // Return "DR" for DRINK
        } else if (strchr("ADEFGHILMNOQSTUWYZ", firstChar) != NULL) {
            return input;  // Return the single letter command
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}


int get_user_input_number() 
{
    int number;
    char input[100];

    while (1) {
        printf("Enter a number: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        if (sscanf(input, "%d", &number) == 1) {
            return number;
        } else {
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
}

// Input/Output functions
char get_user_input()
{
    char input[100];  // Buffer to store user input
    char command;

    while (1) {
        print_message("ENTER YOUR COMMAND: ");
        
        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle input error or EOF
            print_message("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;

        // Convert input to uppercase
        for (int i = 0; input[i]; i++) {
            input[i] = toupper((unsigned char)input[i]);
        }

        // Check if input is empty
        if (strlen(input) == 0) {
            print_message("Please enter a command.\n");
            continue;
        }

        // Get the first character of the input
        command = input[0];

        // Check if it's a valid command
        if (strchr("1234567890ABCDEFGHILMNOQRSTUWY", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}

char get_user_input_yn()
{
    char input[100];  // Buffer to store user input
    char command;

    while (1) {
        
        // Get user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle input error or EOF
            print_message("Error reading input. Please try again.\n");
            continue;
        }

        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;

        // Convert input to uppercase
        for (int i = 0; input[i]; i++) {
            input[i] = toupper((unsigned char)input[i]);
        }

        // Check if input is empty
        if (strlen(input) == 0) {
            print_message("PLEASE ENTER Y OR NO.\n");
            continue;
        }

        // Get the first character of the input
        command = input[0];

        // Check if it's a valid command
        if (strchr("YN", command) != NULL) {
            return command;
        } else {
            print_message("Invalid command. Type 'H' for help.\n");
        }
    }
}


void print_message(const char *message)
{
	printf("%s", message);
}

void open_book(Player *player, GameState *game)
{
    int effect = random_number(6);
    
    print_message("YOU OPEN THE BOOK AND ");
    
    switch(effect) {
        case 1:
            print_message("FLASH! OH NO! YOU ARE NOW A BLIND ");
            print_message(player->race == 3 ? "HUMAN" : "CREATURE");
            print_message("!\n");
            player->blindness_flag = 1;
            break;
        case 2:
            print_message("IT'S ANOTHER VOLUME OF ZOT'S POETRY! - YECH!!\n");
            break;
        case 3:
            printf("IT'S AN OLD COPY OF PLAY%s!\n", get_random_species());
            break;
        case 4:
            print_message("IT'S A MANUAL OF DEXTERITY!\n");
            player->dexterity = 18;
            break;
        case 5:
            print_message("IT'S A MANUAL OF STRENGTH!\n");
            player->strength = 18;
            break;
        case 6:
            print_message("THE BOOK STICKS TO YOUR HANDS -\n");
            print_message("NOW YOU ARE UNABLE TO DRAW YOUR WEAPON!\n");
            player->stickybook_flag = 1;
            break;
    }
    
    // Remove the book from the room
    set_room_content(game, player->x, player->y, player->level, 101);  // Empty room
}

const char* get_race_name(int race)
{
    switch(race)
    {
        case 1: return "HOBBIT";
        case 2: return "ELF";
        case 3: return "HUMAN";
        case 4: return "DWARF";
        default: return "UNKNOWN";
    }
}

void mark_room_discovered(GameState *game, int x, int y, int level)
{
    int index = CALCULATE_ROOM_INDEX(level, x, y);
    game->discovered_rooms[index] = 1;
}

int is_room_discovered(GameState *game, int x, int y, int level)
{
    int index = CALCULATE_ROOM_INDEX(level, x, y);
    return game->discovered_rooms[index];
}

void discover_adjacent_rooms(GameState *game, Player *player)
{
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int x = WRAP_COORDINATE(player->x + dx);
            int y = WRAP_COORDINATE(player->y + dy);
            mark_room_discovered(game, x, y, player->level);
        }
    }
}

// New helper function to get abbreviated room descriptions
void get_room_description(int room_content, char *desc)
{
    char full_desc[100];
    switch (room_content) {
        case EMPTY_ROOM:  strncpy(full_desc, "EMPTY   \0", 9); break;
        case ENTRANCE:    strncpy(full_desc, "ENTRANCE\0", 9); break;
        case STAIRS_UP:   strncpy(full_desc,"STAIRS U\0", 9); break;
        case STAIRS_DOWN: strncpy(full_desc,"STAIRS D\0",9); break;
        case POOL:        strncpy(full_desc,"POOL    \0", 9); break;
        case CHEST:       strncpy(full_desc,"CHEST   \0",9); break;
        case GOLD:        strncpy(full_desc,"GOLD    \0",9); break;
        case FLARES:      strncpy(full_desc,"FLARES  \0",9); break;
        case WARP:        strncpy(full_desc,"WARP    \0",9); break;
        case SINKHOLE:    strncpy(full_desc,"SINKHOLE\0",9); break;
        case CRYSTAL_ORB: strncpy(full_desc,"CRYSTAL \0",9); break;
        case BOOK:        strncpy(full_desc,"BOOK    \0",9); break;
        case KOBOLD:      strncpy(full_desc,"KOBOLD  \0",9); break;
        case ORC:         strncpy(full_desc,"ORC     \0",9); break;
        case WOLF:        strncpy(full_desc,"WOLF    \0",9); break;
        case GOBLIN:      strncpy(full_desc,"ORC     \0",9); break;
        case OGRE:        strncpy(full_desc,"OGRE    \0",9); break;
        case TROLL:       strncpy(full_desc,"TROLL   \0",9); break;
        case BEAR:        strncpy(full_desc,"BEAR    \0",9); break;
        case MINOTAUR:    strncpy(full_desc,"MINOTAUR\0",9); break;
        case GARGOYLE:    strncpy(full_desc,"GARGOYLE\0",9); break;
        case CHIMERA:     strncpy(full_desc,"CHIMERA \0",9); break;
        case BALROG:      strncpy(full_desc,"BALROG  \0",9); break;
        case DRAGON:      strncpy(full_desc,"DRAGON  \0",9); break;
        case VENDOR:      strncpy(full_desc,"VENDOR  \0", 9); break;
        case RUBY_RED:    strncpy(full_desc,"RUBY RED\0", 9); break;
        case NORN_STONE:  strncpy(full_desc,"NORN STN\0", 9); break;
        case PALE_PEARL:  strncpy(full_desc,"PALE PRL\0", 9); break;
        case GREEN_GEM:   strncpy(full_desc,"GREEN GM\0", 9); break;
        case BLUE_FLAME:  strncpy(full_desc,"BLUE FLM\0", 9); break;
        case PALANTIR:    strncpy(full_desc,"PALANTIR\0", 9); break;
        case SILMARIL:    strncpy(full_desc,"SILMARIL\0", 9); break;
        default:         strncpy(full_desc, "   ??   \0",9); break;
    }
    strncpy(desc, full_desc, 9);
}

char get_room_symbol(int room_content)
{
    switch (room_content) {
        case EMPTY_ROOM: return '.';  // Empty room
        case ENTRANCE: return 'E';  // Entrance
        case STAIRS_UP: return 'U';  // Stairs going up
        case STAIRS_DOWN: return 'D';  // Stairs going down
        case POOL: return 'P';  // Pool
        case CHEST: return 'C';  // Chest
        case GOLD: return 'G';  // Gold
        case FLARES: return 'F';  // Flares
        case WARP: return 'W';  // Warp
        case SINKHOLE: return 'S';  // Sinkhole
        case CRYSTAL_ORB: return 'O';  // Crystal Orb
        case BOOK: return 'B';  // Book
        case MONSTER_START ... MONSTER_END: return 'M';  // Monsters (all types)
        case VENDOR: return 'V';  // Vendor
        case TREASURE_START ... TREASURE_END: return 'T';  // Treasures
        default: return '?';  // Unknown
    }
}

void end_game(Player *player, GameState *game)
{
    printStars();
    
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    const char *armor_types[] = {"NO ARMOR", "LEATHER", "CHAINMAIL", "PLATE"};
    const char *weapon_types[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD"};

    if (player->strength <= 0 || player->intelligence <= 0 || player->dexterity <= 0) {
        printf("A NOBLE EFFORT, OH FORMERLY LIVING %s!\n\n", race_names[player->race - 1]);
        
        if (player->strength <= 0)
            print_message("YOU DIED DUE TO LACK OF STRENGTH.\n\n");
        else if (player->intelligence <= 0)
            print_message("YOU DIED DUE TO LACK OF INTELLIGENCE.\n\n");
        else
            print_message("YOU DIED DUE TO LACK OF DEXTERITY.\n\n");

        print_message("AT THE TIME YOU DIED, YOU HAD :\n");
    }
    else if (game->victory) {
        print_message("CONGRATULATIONS!\n\n");
        print_message("YOU LEFT THE CASTLE WITH THE ORB OF ZOT.\n\n");
        print_message("AN INCREDIBLY GLORIOUS VICTORY!!\n\n");
        print_message("IN ADDITION, YOU GOT OUT WITH THE FOLLOWING :\n");
    }
    else {
        print_message("YOU LEFT THE CASTLE WITHOUT THE ORB OF ZOT.\n\n");
        print_message("A LESS THAN AWE-INSPIRING DEFEAT.\n\n");
        print_message("WHEN YOU LEFT THE CASTLE, YOU HAD :\n");
    }

    // List treasures
    for (int i = 0; i < TREASURE_COUNT; i++) {
        if (game->treasure[i]) {
            printf("%s\n", get_treasure_name(i));
        }
    }

    // Print equipment
    printf("%s AND %s", weapon_types[player->weapon_type], armor_types[player->armor_type]);
    if (player->lamp_flag)
        print_message(" AND A LAMP");
    print_message("\n");

    // Print flares and gold
    printf("YOU ALSO HAD %d FLARES AND %d GOLD PIECES\n", player->flares, player->gold);

    // Print Runestaff status
    if (player->runestaff_flag)
        print_message("AND THE RUNESTAFF\n");

    // Print turn count
    printf("\nAND IT TOOK YOU %d TURNS!\n", game->turn_count);

    printStars();
}


const char* get_monster_name(int room_content)
{
    switch (room_content) {
        case KOBOLD: return "KOBOLD";
        case ORC: return "ORC";
        case WOLF: return "WOLF";
        case GOBLIN: return "GOBLIN";
        case OGRE: return "OGRE";
        case TROLL: return "TROLL";
        case BEAR: return "BEAR";
        case MINOTAUR: return "MINOTAUR";
        case GARGOYLE: return "GARGOYLE";
        case CHIMERA: return "CHIMERA";
        case BALROG: return "BALROG";
        case DRAGON: return "DRAGON";
        default: return "UNKNOWN MONSTER";
    }
}

void handle_treasure(Player *player, GameState *game, int room_content)
{
    int treasure_index = room_content - TREASURE_START;
    const char* treasure_name = get_treasure_name(treasure_index);

    printf("\nYOU FOUND %s!\n", treasure_name);
    
    game->treasure[treasure_index] = 1;
    player->treasure_count++;
    printf("YOU NOW HAVE %d TREASURES.\n", player->treasure_count);

    // Apply special effects of treasures
    switch (treasure_index) {
        case 0: // Ruby Red
        	print_message("THE RUBY RED WILL HELP YOU AVOID LETHARGY.\n");
	    break;
        case 1: // Norn Stone
	    print_message("THE NORN STONE GLOWS WITH AN OTHERWORLDLY LIGHT.\n");
    	break;
        case 2: // Pale Pearl
	    print_message("THE PALE PEARL WILL PROTECT YOU FROM LEECHES.\n");
        	break;
        case 3: // Opal Eye
	    print_message("THE OPAL EYE WILL CURE BLINDNESS.\n");
        	if (player->blindness_flag) {
	            player->blindness_flag = 0;
	        print_message("YOUR BLINDNESS IS CURED!\n");
        	}
	    break;
        case 4: // Green Gem
	    print_message("THE GREEN GEM WILL HELP YOU AVOID FORGETTING.\n");
        	break;
        case 5: // Blue Flame
	    print_message("THE BLUE FLAME WILL DISSOLVE BOOKS.\n");
        	if (player->stickybook_flag) {
	        player->stickybook_flag = 0;
	        print_message("THE STICKY BOOK DISSOLVES!\n");
        	}
	    break;
        case 6: // Palantir
        case 7: // Silmaril
	    print_message("ITS BEAUTY IS BEYOND COMPARE.\n");
        	break;
    }

    // Remove the treasure from the room
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
}
