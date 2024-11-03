#include "wizards-castle.h"
#include <stdio.h>
#include <string.h>

bool save_game(const char *filename, const Player *player,
               const GameState *game);

bool load_game(const char *filename, Player *player, GameState *game);
