#ifndef VENDOR_H
#define VENDOR_H

#include "wizards-castle.h"

// Function prototypes
void handle_vendor(Player *player, GameState *game);
void trade_with_vendor(Player *player, GameState *game);
void attack_vendor(Player *player, GameState *game);
void buy_armor(Player *player);
void buy_weapon(Player *player);
void buy_flares(Player *player);

#endif // VENDOR_H
