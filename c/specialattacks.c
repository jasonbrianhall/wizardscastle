#include "wizards-castle.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void dragon_fireball_attack(Player *player, GameState *game) {
    print_message("\nThe dragon breathes a massive fireball at you!\n");
    
    // Dexterity-based avoidance check
    if (random_number(20) + player->dexterity > random_number(20) + 15) {  // Dragon has high dexterity
        print_message("You manage to dodge the fireball!\n");
        return;
    }
    
    int damage = random_number(10) + 5;  // 6 to 15 damage
    printf("The fireball hits you for %d damage!\n", damage);
    
    // Apply armor reduction
    if (player->armor_type != 0) {
        int armor_protection = player->armor_type + random_number(3)-1;
        damage -= armor_protection;
        player->armor_points -= armor_protection;
        
        if (damage < 0) {
            player->armor_points += damage;  // Adjust for overkill protection
            damage = 0;
        }
        
        printf("Your armor absorbs %d damage.\n", armor_protection);
        
        if (player->armor_points <= 0) {
            player->armor_points = 0;
            player->armor_type = 0;
            print_message("YOUR ARMOR HAS BEEN DESTROYED BY THE DRAGON'S FIRE!\n");
        }
    }
    
    player->strength -= damage;
    printf("You take %d final damage from the fireball!\n", damage);
    
    if (player->strength <= 0) {
        print_message("\nYOU HAVE BEEN INCINERATED BY THE DRAGON'S FIRE!  GOOD LUCK ADVENTURER.\n");
        game->game_over = 1;
    }
}

void balrog_flame_whip_attack(Player *player, GameState *game) {

    print_message("\nThe Balrog lashes out with its flame whip!\n");
    
    // Dexterity-based avoidance check
    if (random_number(20) + player->dexterity > random_number(20) + 18) {  // Balrog has very high dexterity
        print_message("You narrowly avoid the searing flames!\n");
        return;
    }
    
    int damage = random_number(8) + 4;  // 5 to 12 damage
    printf("The flame whip strikes you for %d damage!\n", damage);
    
    // Apply armor reduction
    if (player->armor_type != 0) {
        int armor_protection = player->armor_type + random_number(3)-1;
        damage -= (armor_protection + random_number(3)-1);
        
        player->armor_points -= armor_protection;
        if (damage < 0) {
            player->armor_points += damage;  // Adjust for overkill protection
            damage = 0;
        }
        
        printf("Your armor absorbs %d damage.\n", armor_protection);
        
        if (player->armor_points <= 0) {
            player->armor_points = 0;
            player->armor_type = 0;
            print_message("YOUR ARMOR HAS BEEN MELTED BY THE BALROG'S FLAME WHIP!  GOOD LUCK ADVENTURER.\n");
        }
    }
    
    player->strength -= damage;
    printf("You take %d final damage from the flame whip!\n", damage);
    
    if (player->strength <= 0) {
        print_message("\nYOU HAVE BEEN INCINERATED BY THE BALROG'S FLAME WHIP!\n");
        game->game_over = 1;
    }
}
