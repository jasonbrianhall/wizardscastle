#include "utilities.h"
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

