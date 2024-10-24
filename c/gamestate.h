#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "wizards-castle.h"

#define CALCULATE_ROOM_INDEX(level, x, y) (CASTLE_SIZE*CASTLE_SIZE * ((level) - 1) + CASTLE_SIZE * ((x) - 1) + (y) - 1)


// Function prototypes
int check_game_over(Player *player, GameState *game);
void end_game(Player *player, GameState *game);
void generate_castle(GameState *game);
int get_room_content(GameState *game, int x, int y, int level);
void set_room_content(GameState *game, int x, int y, int level, int content);
void mark_room_discovered(GameState *game, int x, int y, int level);
int is_room_discovered(GameState *game, int x, int y, int level);
void handle_treasure(Player *player, GameState *game, int room_content);

#endif // GAME_STATE_H
