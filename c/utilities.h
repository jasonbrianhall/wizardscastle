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

// Macro definitions
#define WRAP_COORDINATE(coord) (((coord) - 1 + 8) % 8 + 1)
#define CALCULATE_ROOM_INDEX(level, x, y) (64 * ((level) - 1) + 8 * ((x) - 1) + (y) - 1)

#endif // UTILITIES_H