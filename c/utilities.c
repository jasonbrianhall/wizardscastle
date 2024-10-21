#include "utilities.h"
#include "combat.h"
#include "player.h"
#include "gamestate.h"
#include "wizardio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

const char* get_random_body_part()
{
    const char* body_parts[] = {" SANDWICH", " STEW", " SOUP", " BURGER", " ROAST", " FILET", " TACO", " PIE"};
    return body_parts[random_number(8) - 1];
}

const char* get_random_species()
{
    const char* body_parts[] = {"ELF", "HUMAN", "DWARF", "HOBBIT", "DROW"};
    return body_parts[random_number(4) - 1];
}

// Utility functions
int random_number(int max_value)
{
    return 1 + rand() % max_value;
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
            print_message_formatted("Congratulations! You've escaped the castle with the Orb of Zot!\n");
            // Set a flag or call a function to end the game with victory
            game->game_over = 1;
            game->victory = 1;
            return;
        } else {
            print_message_formatted("You're at the entrance. Are you sure you want to leave without the Orb of Zot? (Y/N) ");
            char choice = get_user_input_yn();
            if (choice == 'Y') {
                print_message_formatted("You leave the castle empty-handed. Game over!\n");
                // Set a flag or call a function to end the game
                game->game_over = 1;
                return;
            } else {
                print_message_formatted("You decide to stay in the castle.\n");
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
            print_message_formatted("Invalid direction!\n");
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
        print_message_formatted(message);
    }
    // Check for special room events (like warp or sinkhole)
    int room_content = get_room_content(game, player->x, player->y, player->level);
    mark_room_discovered(game, player->x, player->y, player->level);
    
    if (room_content == WARP) {  // Warp
        player->x = random_number(8);
        player->y = random_number(8);
        player->level = random_number(8);
        print_message_formatted("You've been warped to a random location!\n");
    } else if (room_content == SINKHOLE) {  // Sinkhole
        if (player->level < 8) {
            player->level++;
        } else {
            player->level=1;
        }
        print_message_formatted("You've fallen through a sinkhole to the level below!\n");

    }

    // Print the new room description
}


void move_player_randomly(Player *player, GameState *game) 
{
    char directions[] = {'N', 'S', 'E', 'W'};
    char direction = directions[random_number(4) - 1];
    move_player(player, game, direction);
}

// Helper function for minimum of two integers
int min(int a, int b)
{
    return (a < b) ? a : b;
}

void printStars(void)
{
	print_message_formatted("****************************************************************\n");

}

const char* get_weapon_name(int weapon_type)
{
    const char* weapon_names[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD"};

    if (weapon_type<4)
    {
        return weapon_names[weapon_type];

    }
    else
    {
        return weapon_names[0];
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

// New helper function to get abbreviated room descriptions
void get_room_description(int room_content, char *desc)
{
    char full_desc[10];
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
        case GOBLIN:      strncpy(full_desc,"GOBLIN  \0",9); break;
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
        case OPAL_EYE:    strncpy(full_desc,"OPAL EYE\0", 9); break;
        case GREEN_GEM:   strncpy(full_desc,"GREEN GM\0", 9); break;
        case BLUE_FLAME:  strncpy(full_desc,"BLUE FLM\0", 9); break;
        case PALANTIR:    strncpy(full_desc,"PALANTIR\0", 9); break;
        case SILMARIL:    strncpy(full_desc,"SILMARIL\0", 9); break;
        default:         strncpy(full_desc, "   ??   \0",9); break;
    }
    strncpy(desc, full_desc, 10);
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

void use_lamp(Player *player, GameState *game)
{
    if (!player->lamp_flag) {
        print_message_formatted("You don't have a lamp!\n");
        return;
    }

    print_message_formatted("Which direction do you want to shine the lamp? (N/S/E/W) ");
    char direction = get_user_input();
    int dx = 0, dy = 0;

    switch(direction) {
        case 'N': dx = -1; break;
        case 'S': dx = 1; break;
        case 'W': dy = -1; break;
        case 'E': dy = 1; break;
        default:
            print_message_formatted("Invalid direction!\n");
            return;
    }

    int x = WRAP_COORDINATE(player->x + dx);
    int y = WRAP_COORDINATE(player->y + dy);
    
    mark_room_discovered(game, x, y, player->level);
    int room_content = get_room_content(game, x, y, player->level);
    char room_desc[9];
    get_room_description(room_content, room_desc);

    print_message_formatted("\nThe lamp reveals: ");
    print_message_formatted("%s\n", room_desc);
}


void use_flare(Player *player, GameState *game)
{
    if (player->flares <= 0) {
        print_message_formatted("You don't have any flares!\n");
        return;
    }

    player->flares--;
    print_message_formatted("\nYou light a flare. It illuminates the surrounding rooms:\n\n");

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int x = WRAP_COORDINATE(player->x + dx);
            int y = WRAP_COORDINATE(player->y + dy);
            
            mark_room_discovered(game, x, y, player->level);
            int room_content = get_room_content(game, x, y, player->level);
            char room_desc[9];
            get_room_description(room_content, room_desc);

            if (dx == 0 && dy == 0) {
                print_message_formatted("  [YOU]   ");
            } else {
                print_message_formatted(" %s ", room_desc);
            }
        }
        print_message_formatted("\n");
    }
}

void open_chest(Player *player, GameState *game)
{
    print_message_formatted("\nYou open the chest and ");

    int event = random_number(4), damage, gold;
    switch(event) {
        case 1:
            print_message_formatted("KABOOM! IT EXPLODES!!\n");
            damage = random_number(6);
            player->strength -= damage;
            print_message_formatted("You take %d damage.\n", damage);
            if (player->strength <= 0) {
                print_message_formatted("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                game->game_over = 1;
            }
            break;

        case 2:
        case 4:
            {
                gold = random_number(1000);
                print_message_formatted("find %d gold pieces!\n", gold);
                player->gold += gold;
            }
            break;

        case 3:
            print_message_formatted("GAS!! YOU STAGGER FROM THE ROOM!\n");
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
    print_message_formatted("\nYou take a drink and ");

    int effect = random_number(8);
    switch(effect) {
        case 1:
            player->strength = min(18, player->strength + random_number(3));
            print_message_formatted("feel STRONGER.\n");
            break;
        case 2:
            player->strength -= random_number(3);
            print_message_formatted("feel WEAKER.\n");
            if (player->strength <= 0) {
                print_message_formatted("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                game->game_over = 1;
            }
            break;
        case 3:
            player->intelligence = min(18, player->intelligence + random_number(3));
            print_message_formatted("feel SMARTER.\n");
            break;
        case 4:
            player->intelligence -= random_number(3);
            print_message_formatted("feel DUMBER.\n");
            if (player->intelligence <= 0) {
                print_message_formatted("\nYOU DIED DUE TO LACK OF INTELLIGENCE.\n");
                game->game_over = 1;
            }
            break;
        case 5:
            player->dexterity = min(18, player->dexterity + random_number(3));
            print_message_formatted("feel NIMBLER.\n");
            break;
        case 6:
            player->dexterity -= random_number(3);
            print_message_formatted("feel CLUMSIER.\n");
            if (player->dexterity <= 0) {
                print_message_formatted("\nYOU DIED DUE TO LACK OF DEXTERITY.\n");
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
                print_message_formatted("become a %s.\n", get_race_name(player->race));
            }
            break;
        case 8:
            player->sex = 1 - player->sex;  // Toggle between 0 and 1
            print_message_formatted("turn into a %s %s!\n", 
                   player->sex ? "MALE" : "FEMALE", 
                   get_race_name(player->race));
            break;
    }

    // The pool remains in the room, so we don't remove it
}

void teleport(Player *player, GameState *game)
{
    if (!player->runestaff_flag) {
        print_message_formatted("\n** YOU CAN'T TELEPORT WITHOUT THE RUNESTAFF!\n");
        return;
    }

    int new_x, new_y, new_level;

    print_message_formatted("\nEnter X-coordinate (1-8): ");
    new_x=get_user_input_number();
    if (new_x < 1 || new_x > 8) {
        print_message_formatted("Invalid coordinate. Teleportation failed.\n");
        return;
    }

    print_message_formatted("Enter Y-coordinate (1-8): ");
    new_y=get_user_input_number();
        if (new_y < 1 || new_y > 8) {
        print_message_formatted("Invalid coordinate. Teleportation failed.\n");
        return;
    }

    print_message_formatted("Enter Z-coordinate (level 1-8): ");
    new_level=get_user_input_number();
        if (new_level < 1 || new_level > 8) {
        print_message_formatted("Invalid level. Teleportation failed.\n");
        return;
    }


    // X and Y are switched
    player->x = new_y;
    player->y = new_x;
    player->level = new_level;

    print_message_formatted("\nYou have teleported to (%d, %d) on level %d.\n", player->y, player->x, player->level);

    // Check if the player teleported to the Orb of Zot
    if (player->x == game->orb_location[0] && 
        player->y == game->orb_location[1] && 
        player->level == game->orb_location[2]) {
        print_message_formatted("\nGREAT UNMITIGATED ZOT!\n");
        print_message_formatted("\nYOU JUST FOUND ***THE ORB OF ZOT***!\n");
        print_message_formatted("\nTHE RUNESTAFF HAS DISAPPEARED!\n");
        player->runestaff_flag = 0;
        player->orb_flag = 1;
        game->orb_location[0] = 0;  // Mark as found
    }
    move_player(player, game, 'T');
   
}

void gaze_into_orb(Player *player, GameState *game)
{
    if (get_room_content(game, player->x, player->y, player->level) != CRYSTAL_ORB) {
        print_message_formatted("\n** IT'S HARD TO GAZE WITHOUT AN ORB!\n");
        return;
    }

    print_message_formatted("\nYou gaze into the crystal orb and see ");

    int vision = random_number(6);
    switch(vision) {
        case 1:
            print_message_formatted("yourself in a bloody heap!\n");
            player->strength -= random_number(2);
            if (player->strength <= 0) {
                print_message_formatted("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                game->game_over = 1;
            }
            break;
        case 2:
            print_message_formatted("yourself drinking from a pool and becoming a %s!\n", 
                   strip(get_monster_name(MONSTER_START + random_number(12) - 1)));
            break;
        case 3:
            print_message_formatted("%s gazing back at you!\n", 
                   strip(get_monster_name(MONSTER_START + random_number(12) - 1)));
            break;
        case 4:
            {
                int x = random_number(8);
                int y = random_number(8);
                int z = random_number(8);
                int content = get_room_content(game, x, y, z);
                char room_desc[100];  // Adjust size as needed
                get_room_description(content, room_desc);
                print_message_formatted("%s at (%d,%d) Level %d.\n", strip(room_desc), x, y, z);
            }
            break;
        case 5:
            {
                int x, y, z;
                if (random_number(8) < 4) {
                    x = game->orb_location[1];
                    y = game->orb_location[0];
                    z = game->orb_location[2];
                } else {
                    x = random_number(8);
                    y = random_number(8);
                    z = random_number(8);
                }
                print_message_formatted("***THE ORB OF ZOT*** at (%d,%d) Level %d!\n", x, y, z);
            }
            break;
        case 6:
            print_message_formatted("a soap opera rerun!\n");
            break;
    }
}

void open_book(Player *player, GameState *game)
{
    int effect = random_number(7);
    
    print_message_formatted("YOU OPEN THE BOOK AND ");
    
    switch(effect) {
        case 1:
            print_message_formatted("FLASH! OH NO! YOU ARE NOW A BLIND ");
            print_message_formatted(player->race == HUMAN ? "HUMAN" : "CREATURE");
            print_message_formatted("!\n");
            player->blindness_flag = 1;
            break;
        case 2:
            print_message_formatted("IT'S ANOTHER VOLUME OF ZOT'S POETRY! - YECH!!\n");
            break;
        case 3:
            print_message_formatted("IT'S AN OLD COPY OF PLAY%s!\n", get_random_species());
            break;
        case 4:
            print_message_formatted("IT'S A MANUAL OF DEXTERITY!\n");
            player->dexterity = 18;
            break;
        case 5:
            print_message_formatted("IT'S A MANUAL OF STRENGTH!\n");
            player->strength = 18;
            break;
        case 6:
            print_message_formatted("THE BOOK STICKS TO YOUR HANDS -\n");
            print_message_formatted("NOW YOU ARE UNABLE TO DRAW YOUR WEAPON!\n");
            player->stickybook_flag = 1;
            break;
        case 7:
            print_message_formatted("IT'S A MANUAL OF INTELLIGENCE!\n");
            player->intelligence = 18;
            break;

    }
    
    // Remove the book from the room
    set_room_content(game, player->x, player->y, player->level, 101);  // Empty room
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

void display_map(GameState *game, Player *player)
{
    if(player->blindness_flag == 0)
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
                    char room_str[10] = "        \0";
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

        #ifdef MSDOS
        print_message("PRESS ENTER TO CONTINUE...");
        while (getchar() != '\n');  // Wait for Enter key
        #endif

        // Map is too large for the default font for MS-DOS (Same Information is available in help)
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
     else
     {
         print_message("\nBlind %s can't see maps.\n", get_race_name(player->race));
     }
}

char* strip(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    const char* start = str;
    while (*start && (*start == ' ' || *start == '\t')) {
        start++;
    }
    if (*start == '\0') {
        return strdup("");
    }
    const char* end = str + strlen(str) - 1;
    while (end > start && (*end == ' ' || *end == '\t')) {
        end--;
    }
    size_t len = end - start + 1;
    char* result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    strncpy(result, start, len);
    result[len] = '\0';
    return result;
}

void capitalize_sentences(char* str) {
    if (str == NULL || *str == '\0') {
        return;
    }
    bool capitalize_next = true;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha((unsigned char)str[i])) {
            if (capitalize_next) {
                str[i] = toupper((unsigned char)str[i]);
                capitalize_next = false;
            } else {
                str[i] = tolower((unsigned char)str[i]);
            }
        } else if (str[i] == '.' || str[i] == '?' || str[i] == '!') {
            capitalize_next = true;
        } else if (!isspace((unsigned char)str[i])) {
            capitalize_next = false;
        }
    }
}



