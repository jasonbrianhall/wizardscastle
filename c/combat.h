#ifndef COMBAT_H
#define COMBAT_H

#include "wizards-castle.h"

// Function prototypes
void fight_monster(Player *player, GameState *game);
void handle_combat_victory(Player *player, GameState *game, int is_vendor, const char *enemy_name);
int handle_bribe(Player *player, GameState *game, const char *enemy_name);
int handle_spell(Player *player, GameState *game, int *enemy_strength, const char *enemy_name);
void dragon_fireball_attack(Player *player, GameState *game);
void balrog_flame_whip_attack(Player *player, GameState *game);
const char* get_monster_name(int room_content);

int cast_heal_spell(Player *player);
int cast_bright_spell(Player *player);
int cast_haste_spell(Player *player);


#endif // COMBAT_H
