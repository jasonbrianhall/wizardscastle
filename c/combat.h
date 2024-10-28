#ifndef COMBAT_H
#define COMBAT_H

#include "wizards-castle.h"

// Function prototypes
void fight_monster(Player *player, GameState *game);
void handle_combat_victory(Player *player, GameState *game, int is_vendor, const char *enemy_name);
int handle_bribe(Player *player, GameState *game, const char *enemy_name);
int handle_spell(Player *player, GameState *game, int *enemy_strength, int *enemy_intelligence, int *enemy_dexterity, const char *enemy_name);
void dragon_fireball_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity, int enemy_intelligence);
void balrog_flame_whip_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);
void troll_crushing_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);
void minotaur_charge_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);
void chimera_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity, int enemy_intelligence);
const char* get_monster_name(int room_content);

int cast_heal_spell(Player *player);
int cast_bright_spell(Player *player);
int cast_haste_spell(Player *player);
int cast_stone_skin_spell(Player *player);

int calculate_first_strike(int player_dex, int player_int, int player_str, int enemy_dex, int enemy_int, int enemy_str);
int calculate_death_spell(int caster_int, int caster_str, int caster_dex, int target_int, int target_str, int target_dex);
int calculate_damage(Player *player, int enemy_strength, int enemy_dexterity);
int calculate_damage_enemy(Player *player, int enemy_strength, int enemy_dexterity, int basedamage);
int enemy_attack_hits(Player *player, int enemy_dexterity);
int cast_mischief_blast(Player *player, int *enemy_strength, int *enemy_dexterity, int *enemy_intelligence);
void wolf_frenzy_attack(Player *player, int enemy_strength, int enemy_dexterity);
void gargoyle_stone_form_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);
void bear_maul_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);
void ogre_rage_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);
void orc_battle_fury_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);
void goblin_dirty_tricks_attack(Player *player, GameState *game, int enemy_strength, int enemy_dexterity);

bool should_drop_weapon(int monster_type);
bool should_drop_armor(int monster_type);

// Returns armor type based on monster and chance roll
int get_monster_armor_drop(int monster_type);

// Returns weapon type based on monster and chance roll
int get_monster_weapon_drop(int monster_type);
int can_cast_magic(Player* player, int muted);
int can_escape(Player* player, int is_vendor, const char* enemy_name);
#endif // COMBAT_H
