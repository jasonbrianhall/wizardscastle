#ifndef WIZARDS_CASTLE_H
#define WIZARDS_CASTLE_H

#include <stdbool.h>
#include <stdint.h>


#define MAP_SIZE 512
#define TREASURE_COUNT 8

// Room type definitions
#define EMPTY_ROOM 101
#define ENTRANCE 102
#define STAIRS_UP 103
#define STAIRS_DOWN 104
#define START_ITEMS
#define POOL 105
#define CHEST 106
#define GOLD 107
#define FLARES 108
#define WARP 109
#define SINKHOLE 110
#define CRYSTAL_ORB 111
#define BOOK 112
#define END_ITEMS

// Monster rooms (113-124)
#define MONSTER_START 113
#define KOBOLD 113
#define ORC 114
#define WOLF 115
#define GOBLIN 116
#define OGRE 117
#define TROLL 118
#define BEAR 119
#define MINOTAUR 120
#define GARGOYLE 121
#define CHIMERA 122
#define BALROG 123
#define DRAGON 124
#define MONSTER_END 124

#define VENDOR 125

// Treasures (126-133)
#define TREASURE_START  126
#define RUBY_RED        126
#define NORN_STONE      127
#define PALE_PEARL      128
#define OPAL_EYE        129
#define GREEN_GEM       130
#define BLUE_FLAME      131
#define PALANTIR        132
#define SILMARIL        133
#define TREASURE_END    133

#define HOBBIT 1
#define ELF    2        
#define HUMAN  3
#define DWARF  4

// Structures
typedef struct {
    int race:4;
    int sex:4;
    int strength;
    int temp_strength;
    int intelligence;
    int temp_intelligence;
    int dexterity;
    int temp_dexterity;

    int gold;
    int flares;
    int armor_type;
    int armor_points;
    int weapon_type;
    int lamp_flag;
    int x;  // Keep unsigned (four bits to save memory)
    int y;  // Keep unsigned (four bits to save memory)
    int level;  // Keep unsigned
    int runestaff_flag;
    int orb_flag;
    
    int book_flag;
    int blindness_flag;
    int stickybook_flag;
    int has_runestaff;
    int has_orb;
    int treasure_count;
    int web_count;
    

} Player;

typedef struct {
    int location_map[MAP_SIZE];
    int discovered_rooms[MAP_SIZE];  // No sane way to make this one bit integer
    int treasure[TREASURE_COUNT];
    int orb_location[3];
    int runestaff_location[3];
    int turn_count;
    int monster_count;
    int game_over;
    int victory;
    int vendor_attacked;

} GameState;

void print_introduction(void);
// Returns True or False if the player wants to play again
bool main_game_loop(Player *player, GameState *game);
// Utility functions
void display_map(GameState *game, Player *player);
void print_help();

#endif 
