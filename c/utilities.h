#ifndef UTILITIES_H
#define UTILITIES_H

#include "wizards-castle.h"

// Function prototypes
int random_number(int max_value);
void move_player_randomly(Player *player, GameState *game);
const char* get_random_body_part();
const char* get_random_species();
int min(int a, int b);
void printStars(void);
const char* get_weapon_name(int weapon_type);
// Helper function to get treasure names
const char* get_treasure_name(int index);
// New helper function to get abbreviated room descriptions
void get_room_description(int room_content, char *desc);
char get_room_symbol(int room_content);

// Item and treasure functions
void use_lamp(Player *player, GameState *game);
void use_flare(Player *player, GameState *game);
void open_chest(Player *player, GameState *game);
void drink_from_pool(Player *player, GameState *game);

// Magic and special abilities
void teleport(Player *player, GameState *game);
void gaze_into_orb(Player *player, GameState *game);

void open_book(Player *player, GameState *game);
void discover_adjacent_rooms(GameState *game, Player *player);


// Macro definitions
#define WRAP_COORDINATE(coord) (((coord) - 1 + 8) % 8 + 1)
#define CALCULATE_ROOM_INDEX(level, x, y) (64 * ((level) - 1) + 8 * ((x) - 1) + (y) - 1)

#endif // UTILITIES_H
