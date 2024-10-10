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
#define TREASURE_START 126
#define RUBY_RED 126
#define NORN_STONE 127
#define PALE_PEARL 128
#define OPAL_EYE 129
#define GREEN_GEM 130
#define BLUE_FLAME 131
#define PALANTIR 132
#define SILMARIL 133
#define TREASURE_END 133

// Structures
typedef struct {
    int8_t race:4;
    int8_t sex:4;
    int16_t strength;
    int16_t intelligence;
    int16_t dexterity;
    int32_t gold;
    int32_t flares;
    int8_t armor_type:4;
    int8_t armor_points;
    int8_t weapon_type:4;
    int8_t lamp_flag;
    int8_t x:4;
    int8_t y:4;
    int8_t level;
    int8_t runestaff_flag;
    int8_t orb_flag;
    
    uint8_t book_flag:1;
    uint8_t blindness_flag:1;
    uint8_t stickybook_flag:1;
    uint8_t has_runestaff:1;
    uint8_t has_orb:1;
    uint8_t treasure_count:4;
    uint8_t web_count:8;

} Player;

typedef struct {
    uint8_t location_map[MAP_SIZE];
    uint8_t discovered_rooms[MAP_SIZE];  // No sane way to make this one bit integer
    uint8_t treasure[TREASURE_COUNT];
    uint8_t orb_location[3];
    uint8_t runestaff_location[3];
    uint32_t turn_count;
    uint32_t monster_count;
    uint8_t game_over;
    uint8_t victory;
    uint8_t vendor_attacked;

} GameState;

// Main game functions
void initialize_player(Player *player);

void print_introduction(void);
// Returns True or False if the player wants to play again
bool main_game_loop(Player *player, GameState *game);

// Player creation and attribute functions
void choose_race(Player *player);
void choose_sex(Player *player);
void buy_lamp_and_flares(Player *player);
void allocate_attributes(Player *player);

// Map and room functions
void generate_castle(GameState *game);
uint8_t get_room_content(GameState *game, uint8_t x, uint8_t y, uint8_t level);
void set_room_content(GameState *game, uint8_t x, uint8_t y, uint8_t level, uint8_t content);

// Movement and action functions
void move_player(Player *player, GameState *game, char direction);
void fight_monster(Player *player, GameState *game);
void handle_vendor(Player *player, GameState *game);
void trade_with_vendor(Player *player, GameState *game);
void attack_vendor(Player *player, GameState *game);
const char* get_treasure_name(uint8_t index);
int32_t min(int32_t a, int32_t b);
void buy_armor(Player *player);
void buy_flares(Player *player);

void buy_weapon(Player *player);


// Item and treasure functions
void buy_equipment(Player *player);
void use_lamp(Player *player, GameState *game);
void use_flare(Player *player, GameState *game);
void open_chest(Player *player, GameState *game);
void drink_from_pool(Player *player, GameState *game);

// Magic and special abilities
void teleport(Player *player, GameState *game);
void gaze_into_orb(Player *player, GameState *game);

// Utility functions
uint32_t random_number(uint32_t max_value);
void print_status(Player *player, GameState *game);
void display_map(GameState *game, Player *player);
void print_help();

// Game ending functions
uint8_t check_game_over(Player *player);
void end_game(Player *player, GameState *game);

// Input/Output functions
char get_user_input();
char *get_user_input_main();
char get_user_input_yn();

void print_message(const char *message);
void printStars(void);

void open_book(Player *player, GameState *game);

const char* get_race_name(uint8_t race);

void mark_room_discovered(GameState *game, uint8_t x, uint8_t y, uint8_t level);
uint8_t is_room_discovered(GameState *game, uint8_t x, uint8_t y, uint8_t level);
void discover_adjacent_rooms(GameState *game, Player *player);

char get_room_symbol(uint8_t room_content);
void get_room_description(uint8_t room_content, char *desc);

void handle_combat_victory(Player *player, GameState *game, uint8_t is_vendor, const char *enemy_name);
void handle_treasure(Player *player, GameState *game, uint8_t roomcontent);

uint8_t handle_bribe(Player *player, GameState *game, const char *enemy_name);
uint8_t handle_spell(Player *player, GameState *game, int16_t *enemy_strength, const char *enemy_name);
void move_player_randomly(Player *player, GameState *game);
const char* get_weapon_name(uint8_t weapon_type);
const char* get_random_body_part();
const char* get_random_species();
uint32_t get_user_input_number();
const char* get_monster_name(uint8_t room_content);

#endif 
