#include "wizards-castle.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_INT(max) (1 + rand() % (max))
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

}

void initialize_game(GameState *game)
{
    int content, level, x, y, z;
    game->game_over=0;
    game->victory=0;
    game->vendor_attacked=0;
    // Seed the random number generator
    srand(time(NULL));

    // Initialize the castle map
    for (int i = 0; i < MAP_SIZE; i++) {
        game->location_map[i] = 101;  // Empty room
    }
    
    // Set the entrance
    x = 1; y = 4; level = 1;
    game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = 2;  // Entrance

    // Place stairs
    for (level = 1; level <= 7; level++) {
        for (int i = 1; i <= 2; i++) {
            do {
                x = RANDOM_INT(8); y = RANDOM_INT(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] != 101);
            game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = 103;  // Stairs going down
            game->location_map[CALCULATE_ROOM_INDEX(level + 1, x, y)] = 104;  // Stairs going up
        }
    }

    // Place other elements
    for (level = 1; level <= 8; level++) {
        // Place monsters, treasures, etc.
        for (content = 113; content <= 124; content++) {
            do {
                x = RANDOM_INT(8); y = RANDOM_INT(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] != 101);
            game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = content;
        }

        // Place pools, chests, and gold
        for (int i = 1; i <= 3; i++) {
            for (content = 105; content <= 112; content++) {
                do {
                    x = RANDOM_INT(8); y = RANDOM_INT(8);
                } while (game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] != 101);
                game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = content;
            }
            
            // Place vendor
            do {
                x = RANDOM_INT(8); y = RANDOM_INT(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] != 101);
            game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = 125;
        }
    }

    // Place special items (crystal orb, ruby red, etc.)
    for (content = 126; content <= 133; content++) {
        do {
            x = RANDOM_INT(8); y = RANDOM_INT(8); level = RANDOM_INT(8);
        } while (game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] != 101);
        game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = content;
    }

    // Place the Runestaff
    do {
        x = RANDOM_INT(8); y = RANDOM_INT(8); level = RANDOM_INT(8);
    } while (game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] != 101);
    game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = 112 + RANDOM_INT(12);
    game->runestaff_location[0] = x;
    game->runestaff_location[1] = y;
    game->runestaff_location[2] = level;

    // Place the Orb of Zot
    do {
        x = RANDOM_INT(8); y = RANDOM_INT(8); level = RANDOM_INT(8);
    } while (game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] != 101);
    game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = 109;
    game->orb_location[0] = x;
    game->orb_location[1] = y;
    game->orb_location[2] = level;

    // Initialize other game state variables
    game->turn_count = 1;
    game->monster_count = 0;  // You might want to count monsters while placing them

    // Initialize treasures (all not found at start)
    for (int i = 0; i < TREASURE_COUNT; i++) {
        game->treasure[i] = 0;
    }

    // Set the entrance
    x = 1; y = 4; z = 1;
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = 102;
    for (int i = 0; i < MAP_SIZE; i++) {
        game->discovered_rooms[i] = 0;
    }
    
    // Mark the entrance as discovered
    mark_room_discovered(game, 1, 4, 1);

}

void main_game_loop(Player *player, GameState *game)
{
    char user_command;
    int game_over = 0;
    const char *room_contents[] = {
        "AN EMPTY ROOM", "THE ENTRANCE", "STAIRS GOING UP", "STAIRS GOING DOWN",
        "A POOL", "A CHEST", "GOLD PIECES", "FLARES", "A WARP", "A SINKHOLE",
        "A CRYSTAL ORB", "A BOOK", "A KOBOLD", "AN ORC", "A WOLF", "A GOBLIN",
        "AN OGRE", "A TROLL", "A BEAR", "A MINOTAUR", "A GARGOYLE", "A CHIMERA",
        "A BALROG", "A DRAGON", "A VENDOR"
    };

    while (!game_over) {
        game->turn_count++;

        // Handle curses
        if (player->runestaff_flag == 0 && player->orb_flag == 0) {
            // Implement curse effects here (similar to lines 1950-2005 in BASIC)
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
        print_status(player);
        
        int room_content = get_room_content(game, player->x, player->y, player->level);
        if (room_content == 102) {  // The Entrance
            char message[100];
            snprintf(message, sizeof(message), "OK, %s, YOU ARE NOW ENTERING THE CASTLE!\n", get_race_name(player->race));
            print_message(message);
        } else if (room_content >= 101 && room_content <= 133) {
            char message[100];
            snprintf(message, sizeof(message), "HERE YOU FIND %s.\n", room_contents[room_content - 101]);
            print_message(message);
            if (room_content==107)
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
                char message[100];
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
                printf("Fix me, Get Treasure"); 
           }
           
        } else {
            print_message("HERE YOU FIND AN UNKNOWN ROOM.\n");
            printf("%i\n", room_content);
        }
        user_command = get_user_input();

        switch (user_command) {
            case 'N': case 'S': case 'E': case 'W':
                move_player(player, game, user_command);
                break;
            case 'U':
                if (room_content == 103) {  // Stairs going up
                    player->level--;
                    if (player->level<1)
                    {
                        player->level=8;
                    }
                    print_message("YOU CLIMB UP THE STAIRS.\n");
                } else {
                    print_message("THERE ARE NO STAIRS GOING UP FROM HERE!\n");
                }
                break;
            case 'D':
                if (room_content == 104) {  // Stairs going down
                    player->level++;
                    if (player->level>8)
                    {
                        player->level=1;
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
                if (room_content == 106) {  // Chest
                    open_chest(player, game);
                } else if (room_content == 112) {  // Book
                    open_book(player, game);
                } else {
                    print_message("THERE'S NOTHING HERE TO OPEN!\n");
                }
                break;
            case 'G':
                if (room_content == 111) {  // Crystal orb
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
            case 'H':
                print_help();
                break;
            default:
                print_message("INVALID COMMAND. TYPE 'H' FOR HELP.\n");
        }

        if (!game_over) {
            handle_room_event(player, game);
            game_over = check_game_over(player);
        }
    }

    end_game(player, game);
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
        printf("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR STRENGTH? ");
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
        printf("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR INTELLIGENCE? ");
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

    printf("\nYOUR ATTRIBUTES ARE NOW:\n");
    printf("STRENGTH = %d    INTELLIGENCE = %d    DEXTERITY = %d\n", 
           player->strength, player->intelligence, player->dexterity);
}
          

// Map and room functions
void generate_castle(GameState *game)
{
    int x, y, z, q, q1;
    
    // Seed the random number generator
    srand(time(NULL));

    // Initialize all rooms to empty (101)
    for (q = 0; q < MAP_SIZE; q++) {
        game->location_map[q] = 101;
    }

    // Place stairs
    for (z = 1; z <= 7; z++) {
        for (q1 = 1; q1 <= 2; q1++) {
            do {
                x = RANDOM_INT(8);
                y = RANDOM_INT(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != 101);
            game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = 103;  // Stairs down
            game->location_map[CALCULATE_ROOM_INDEX(z+1, x, y)] = 104;  // Stairs up
        }
    }

    // Place other elements
    for (z = 1; z <= 8; z++) {
        // Place monsters, treasures, etc.
        for (q = 113; q <= 124; q++) {
            do {
                x = RANDOM_INT(8);
                y = RANDOM_INT(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != 101);
            game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
        }

        // Place pools, chests, and gold
        for (q1 = 1; q1 <= 3; q1++) {
            for (q = 105; q <= 112; q++) {
                do {
                    x = RANDOM_INT(8);
                    y = RANDOM_INT(8);
                } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != 101);
                game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
            }
            
            // Place vendor
            do {
                x = RANDOM_INT(8);
                y = RANDOM_INT(8);
            } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != 101);
            game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = 125;
        }
    }

    // Place special items (crystal orb, ruby red, etc.)
    for (q = 126; q <= 133; q++) {
        do {
            z = RANDOM_INT(8);
            x = RANDOM_INT(8);
            y = RANDOM_INT(8);
        } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != 101);
        game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
    }

    // Place the Runestaff
    do {
        z = RANDOM_INT(8);
        x = RANDOM_INT(8);
        y = RANDOM_INT(8);
    } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != 101);
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = 112 + RANDOM_INT(12);
    game->runestaff_location[0] = x;
    game->runestaff_location[1] = y;
    game->runestaff_location[2] = z;

    // Place the Orb of Zot
    do {
        z = RANDOM_INT(8);
        x = RANDOM_INT(8);
        y = RANDOM_INT(8);
    } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != 101);
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = 109;
    game->orb_location[0] = x;
    game->orb_location[1] = y;
    game->orb_location[2] = z;

    // Set the entrance
    x = 1; y = 4; z = 1;
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = 102;


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
    char message[100];
    snprintf(message, sizeof(message), "You move %s to (%d, %d) on level %d.\n", 
             direction == 'N' ? "North" :
             direction == 'S' ? "South" :
             direction == 'W' ? "West" : "East",
             player->x, player->y, player->level);
    print_message(message);

    // Check for special room events (like warp or sinkhole)
    int room_content = get_room_content(game, player->x, player->y, player->level);
    mark_room_discovered(game, player->x, player->y, player->level);
    
    if (room_content == 109) {  // Warp
        player->x = 1 + rand() % 8;
        player->y = 1 + rand() % 8;
        player->level = 1 + rand() % 8;
        print_message("You've been warped to a random location!\n");
    } else if (room_content == 110) {  // Sinkhole
        if (player->level < 8) {
            player->level++;
            print_message("You've fallen through a sinkhole to the level below!\n");
        } else {
            print_message("You've fallen through a sinkhole, but you're already at the bottom level!\n");
        }
    }

    // Print the new room description
    handle_room_event(player, game);
}

void handle_room_event(Player *player, GameState *game)
{

}

void fight_monster(Player *player, GameState *game)
{

}

void handle_vendor(Player *player, GameState *game)
{
    print_message("\nYou've encountered a vendor!\n");
    
    // Check if the player has attacked the vendor before
    if (game->vendor_attacked) {
        print_message("The vendor refuses to trade with you due to your previous attack.\n");
        return;
    }

    char choice;
    do {
        print_message("\nDo you want to (T)rade, (A)ttack, or (I)gnore the vendor? ");
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
    while (player->gold >= 1000) {
        print_message("\nWhat would you like to buy?\n");
        print_message("1. Improve Strength (1000 GP)\n");
        print_message("2. Improve Intelligence (1000 GP)\n");
        print_message("3. Improve Dexterity (1000 GP)\n");
        print_message("4. Armor (1250-2000 GP)\n");
        print_message("5. Weapon (1250-2000 GP)\n");
        print_message("6. Lamp (1000 GP)\n");
        print_message("7. Nothing more\n");

        char purchase_choice = get_user_input();
        switch(purchase_choice) {
            case '1':
                player->strength = min(player->strength + random_number(6), 18);
                player->gold -= 1000;
                break;
            case '2':
                player->intelligence = min(player->intelligence + random_number(6), 18);
                player->gold -= 1000;
                break;
            case '3':
                player->dexterity = min(player->dexterity + random_number(6), 18);
                player->gold -= 1000;
                break;
            case '4':
                if (player->gold<1250)
                {
                    print_message("YOU DO NOT HAVE ENOUGH GOLD TO BUY ARMOR!\n");
                }
                else
                {
                    buy_armor(player);
                }
                break;
            case '5':
                if (player->gold<1250)
                {
                    print_message("YOU DO NOT HAVE ENOUGH GOLD TO BUY WEAPONS!\n");
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
                    print_message("You already have a lamp!\n");
                    continue;
                } else {
                    print_message("You don't have enough gold for a lamp.\n");
                    continue;
                }
                break;
            case '7':
                return;
            default:
                print_message("Invalid choice.\n");
                continue;
        }

        print_message("Purchase successful!\n");
    }

    print_message("You don't have enough gold for any more purchases.\n");
}

void attack_vendor(Player *player, GameState *game)
{
    print_message("You attack the vendor!\n");
    game->vendor_attacked = 1;

    // The vendor has high stats and good equipment
    int vendor_strength = 15;
    int vendor_dexterity = 15;
    int vendor_armor = 3;
    int vendor_weapon = 3;

    // Simple combat resolution
    if (random_number(20) + player->dexterity > random_number(20) + vendor_dexterity) {
        int damage = random_number(player->weapon_type * 2 + 2) - vendor_armor;
        if (damage > 0) {
            print_message("You hit the vendor!\n");
            vendor_strength -= damage;
            if (vendor_strength <= 0) {
                print_message("You've defeated the vendor!\n");
                // Give player vendor's inventory
                player->gold += 2000;
                player->armor_type = 3;
                player->armor_points = 21;
                player->weapon_type = 3;
                if (!player->lamp_flag) {
                    player->lamp_flag = 1;
                    print_message("You found a lamp!\n");
                }
                player->strength = min(player->strength + random_number(6), 18);
                player->intelligence = min(player->intelligence + random_number(6), 18);
                player->dexterity = min(player->dexterity + random_number(6), 18);
                return;
            }
        } else {
            print_message("Your attack was ineffective.\n");
        }
    } else {
        print_message("You missed the vendor!\n");
    }

    // Vendor's counterattack
    if (random_number(20) + vendor_dexterity > random_number(20) + player->dexterity) {
        int damage = random_number(vendor_weapon * 2 + 2) - player->armor_points / 7;
        if (damage > 0) {
            print_message("The vendor hits you!\n");
            player->strength -= damage;
            if (player->strength <= 0) {
                print_message("The vendor has defeated you!\n");
                game->game_over = 1;
                return;
            }
        } else {
            print_message("The vendor's attack was ineffective.\n");
        }
    } else {
        print_message("The vendor missed you!\n");
    }

    print_message("The vendor disappears in a puff of smoke!\n");
}

void buy_armor(Player *player)
{
    print_message("\nArmor Options:\n");
    print_message("1. Leather (1250 GP)\n");
    print_message("2. Chainmail (1500 GP)\n");
    print_message("3. Plate (2000 GP)\n");
    print_message("4. Nothing\n");

    char armor_choice = get_user_input();
    switch(armor_choice) {
        case '1':
            if (player->gold >= 1250) {
                player->armor_type = 1;
                player->armor_points = 7;
                player->gold -= 1250;
            } else {
                print_message("Not enough gold for Leather Armor.\n");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                player->armor_type = 2;
                player->armor_points = 14;
                player->gold -= 1500;
            } else {
                print_message("Not enough gold for Chainmail.\n");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                player->armor_type = 3;
                player->armor_points = 21;
                player->gold -= 2000;
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
    print_message("Armor purchased successfully!\n");
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
                player->weapon_type = 1;
                player->gold -= 1250;
            } else {
                print_message("Not enough gold for a Dagger.\n");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                player->weapon_type = 2;
                player->gold -= 1500;
            } else {
                print_message("Not enough gold for a Mace.\n");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                player->weapon_type = 3;
                player->gold -= 2000;
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
    print_message("Weapon purchased successfully!\n");
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
    printf("\nOK, %s, YOU HAVE %d GP'S LEFT.\n\n", race_names[player->race - 1], player->gold);
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
        do {
            fgets(user_input, sizeof(user_input), stdin);
            flares_to_buy = atoi(user_input);

            if (flares_to_buy >= 0 && flares_to_buy <= player->gold) {
                player->flares += flares_to_buy;
                player->gold -= flares_to_buy;
                printf("\nOK, YOU NOW HAVE %d FLARES AND %d GOLD PIECES LEFT.\n", player->flares, player->gold);
                break;
            } else if (flares_to_buy > player->gold) {
                printf("** YOU CAN ONLY AFFORD %d.\n", player->gold);
            } else {
                print_message("** IF YOU DON'T WANT ANY, JUST TYPE 0 (ZERO).\n");
            }
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

}

void drink_from_pool(Player *player)
{

}

// Magic and special abilities
void cast_spell(Player *player, GameState *game, char spell_type)
{

}

void teleport(Player *player, GameState *game)
{

}

void gaze_into_orb(Player *player, GameState *game)
{

}

// Utility functions

int random_number(int max_value)
{
    return 1 + rand() % max_value;
}

void print_status(Player *player)
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
             player->level, player->x, player->y);
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

    print_message("======================\n\n");
}

void display_map(GameState *game, Player *player)
{
    print_message("\n=== MAP OF LEVEL ");
    char level_str[3];
    snprintf(level_str, sizeof(level_str), "%d", player->level);
    print_message(level_str);
    print_message(" ===\n\n");

    for (int x = 1; x <= 8; x++) {
        for (int y = 1; y <= 8; y++) {
            if (x == player->x && y == player->y) {
                print_message("[@]");
            } else if (is_room_discovered(game, x, y, player->level)) {
                int room_content = get_room_content(game, x, y, player->level);
                char symbol = get_room_symbol(room_content);
                char room_str[4] = " ? ";
                room_str[1] = symbol;
                print_message(room_str);
            } else {
                print_message(" ? ");
            }
        }
        print_message("\n");
    }

    print_message("\nLEGEND:\n");
    print_message("[@] = You    . = Empty   E = Entrance  U/D = Stairs\n");
    print_message("P = Pool     C = Chest   F = Flares    G = Gold\n");
    print_message("S = Sinkhole W = Warp    O = Orb       B = Book\n");
    print_message("M = Monster  V = Vendor  T = Treasure  ? = Unknown\n");
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
    print_message("Q/UIT     - End the game\n\n");

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


void end_game(Player *player, GameState *game)
{

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
        if (strchr("NSEWUDMFLOGTHQYN", command) != NULL) {
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
            print_message("IT'S AN OLD COPY OF PLAYBOY!\n");
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

char get_room_symbol(int room_content)
{
    switch (room_content) {
        case 101: return '.';  // Empty room
        case 102: return 'E';  // Entrance
        case 103: return 'U';  // Stairs going up
        case 104: return 'D';  // Stairs going down
        case 105: return 'P';  // Pool
        case 106: return 'C';  // Chest
        case 107: return 'G';  // Gold
        case 108: return 'F';  // Flares
        case 109: return 'W';  // Warp
        case 110: return 'S';  // Sinkhole
        case 111: return 'O';  // Crystal Orb
        case 112: return 'B';  // Book
        case 113 ... 124: return 'M';  // Monsters (all types)
        case 125: return 'V';  // Vendor
        case 126 ... 133: return 'T';  // Treasures
        default: return '?';  // Unknown
    }
}
