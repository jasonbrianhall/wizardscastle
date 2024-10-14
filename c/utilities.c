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


