#include "utilities.h"
#include "combat.h"
#include "player.h"
#include "gamestate.h"
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
    const char* body_parts[] = {"ELF", "HUMAN", "DWARF", "HOBBIT"};
    return body_parts[random_number(4) - 1];
}

// Utility functions
int random_number(int max_value)
{
    return 1 + rand() % max_value;
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
	print_message("****************************************************************\n");

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
        case OPAL_EYE:    strncpy(full_desc,"OPAL EYE\0", 9); break;
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
                    x = game->orb_location[1];
                    y = game->orb_location[0];
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

