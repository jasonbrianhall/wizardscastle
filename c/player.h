#ifndef PLAYER_H
#define PLAYER_H

#include "wizards-castle.h" // This includes the Player struct definition

// Function prototypes
void initialize_player(Player *player);
void choose_race(Player *player);
void choose_sex(Player *player);
void allocate_attributes(Player *player);
void buy_equipment(Player *player);
void buy_lamp_and_flares(Player *player);
const char *get_race_name(int race);
const char *get_race_name_flc(int race);

void print_status(Player *player, GameState *game); // Added GameState parameter
void fight_monster_normalize(Player *player, GameState *game);

#endif // PLAYER_H
