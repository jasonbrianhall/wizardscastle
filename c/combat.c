#include "combat.h"
#include "gamestate.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

void fight_monster(Player *player, GameState *game)
{
    int room_content = get_room_content(game, player->x, player->y, player->level);
    int is_vendor = (room_content == VENDOR);
    int enemy_strength, enemy_dexterity;
    int can_bribe = 1;
    const char *enemy_name = is_vendor ? "VENDOR" : get_monster_name(room_content);
    player->web_count=0;
    // Set enemy stats based on room content
    if (is_vendor) {
        enemy_strength = 15;
        enemy_dexterity = 15;
    } else {
        enemy_strength = (room_content - MONSTER_START + 1) * 2;
        enemy_dexterity = room_content - MONSTER_START + 8;
    }

    while (1) {
        printf("\nYOU'RE FACING %s!\n\n", enemy_name);
        print_message("YOU MAY (A)TTACK OR (R)ETREAT.\n");
        if (can_bribe) {
            print_message("YOU CAN ALSO ATTEMPT A (B)RIBE.\n");
        }
        if (player->intelligence > 14) {
            print_message("YOU CAN ALSO (C)AST A SPELL.\n");
        }
        print_message("\n");
        printf("YOUR STRENGTH IS %d, YOUR DEXTERITY IS %d, AND YOUR INTELLIGENCE IS %d.\n", 
               player->strength, player->dexterity, player->intelligence);

        char choice = get_user_input();

        switch (choice) {
            case 'A':
                if (player->weapon_type == 0) {
                    printf("\n** POUNDING ON %s WON'T HURT IT!\n", enemy_name);
                } else if (player->stickybook_flag) {
                    print_message("\n** YOU CAN'T BEAT IT TO DEATH WITH A BOOK!\n");
                } else if (random_number(20) + player->dexterity <= random_number(20) + (3 * player->blindness_flag)) {
                    print_message("\nYOU MISSED, TOO BAD!\n");
                } else {
                    printf("\nYOU HIT THE EVIL %s!\n", enemy_name);
                    enemy_strength -= player->weapon_type;
                    if ((room_content == GARGOYLE || room_content == DRAGON) && random_number(8) == 1) {
                        printf("\nOH NO! YOUR %s BROKE!\n", get_weapon_name(player->weapon_type));
                        player->weapon_type = 0;
                    }
                    if (enemy_strength <= 0) {
                        handle_combat_victory(player, game, is_vendor, enemy_name);
                        return;
                    }
                }
                break;

            case 'R':
                if (random_number(20) + player->dexterity > random_number(20) + enemy_dexterity) {
                    print_message("\nYOU HAVE ESCAPED!\n");
                    move_player_randomly(player, game);
                    return;
                } else {
                    print_message("\nYOU FAILED TO RETREAT!\n");
                }
                break;

            case 'B':
                if (!can_bribe) {
                    print_message("\n** CHOOSE ONE OF THE OPTIONS LISTED.\n");
                    continue;
                }
                if (handle_bribe(player, game, enemy_name)) {
                    return;
                }
                can_bribe = 0;
                break;

            case 'C':
                if (player->intelligence <= 14) {
                    print_message("\n** YOU CAN'T CAST A SPELL NOW!\n");
                    continue;
                }
                if (handle_spell(player, game, &enemy_strength, enemy_name)) {
                    return;
                }
                break;

            default:
                print_message("\n** CHOOSE ONE OF THE OPTIONS LISTED.\n");
                continue;
        }

        // Enemy's turn
        if (choice != 'R' || (choice == 'R' && random_number(20) + player->dexterity <= random_number(20) + enemy_dexterity)) {
            if (player->web_count > 0) {
                player->web_count--;
                if (player->web_count == 0) {
                    print_message("\nTHE WEB JUST BROKE!\n");
                } else {
                    printf("\nTHE %s IS STUCK AND CAN'T ATTACK NOW!\n", enemy_name);
                    continue;  // Skip the enemy's attack
                }
            }
            else if (room_content == DRAGON && random_number(3) == 1) {  // 1 in 3 chance for fireball
                    printf("\nTHE %s ATTACKS!\n", enemy_name);

                    dragon_fireball_attack(player, game);
                    if (game->game_over) {
                        return;
                    }
            }
            else if (room_content == BALROG && random_number(5) == 1) {
                balrog_flame_whip_attack(player, game);
                if (game->game_over) {
                    return;
                }
            }
            else if (random_number(7) + random_number(7) + random_number(7) + 3 * player->blindness_flag >= player->dexterity) {

                print_message("\nOUCH! HE HIT YOU!\n");
                int damage = (enemy_strength / 2) + 1;
                
                // Apply armor reduction
                if (player->armor_type != 0) {
                    damage -= player->armor_type;
                    player->armor_points -= player->armor_type;
                    if (damage < 0) {
                        player->armor_points -= damage;  // Absorb excess damage
                        damage = 0;
                    }
                    if (player->armor_points < 0) {
                        player->armor_points = 0;
                        player->armor_type = 0;
                        print_message("\nYOUR ARMOR HAS BEEN DESTROYED... GOOD LUCK!\n");
                    }
                }
                
                player->strength -= damage;
                printf("YOU TAKE %d DAMAGE!\n", damage);
                
                if (player->strength <= 0) {
                    print_message("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                    game->game_over = 1;
                    return;
                }
            } else {
                print_message("\nWHAT LUCK, HE MISSED YOU!\n");
            }
        }
    }
}

void handle_combat_victory(Player *player, GameState *game, int is_vendor, const char *enemy_name)
{
    printf("\n%s LIES DEAD AT YOUR FEET!\n", enemy_name);
    
    if (random_number(5) == 1) {  // 20% chance of eating
        printf("\nYOU SPEND AN HOUR EATING %s%s.\n", enemy_name, get_random_body_part());
    }

    if (is_vendor) {
        print_message("\nYOU GET ALL HIS WARES:\n");
        print_message("PLATE ARMOR\n");
        player->armor_type = 3;
        player->armor_points = 21;
        print_message("A SWORD\n");
        player->weapon_type = 3;
        print_message("A STRENGTH POTION\n");
        player->strength = min(player->strength + random_number(6), 18);
        print_message("AN INTELLIGENCE POTION\n");
        player->intelligence = min(player->intelligence + random_number(6), 18);
        print_message("A DEXTERITY POTION\n");
        player->dexterity = min(player->dexterity + random_number(6), 18);
        if (!player->lamp_flag) {
            print_message("A LAMP\n");
            player->lamp_flag = 1;
        }
    } else {
        // Check if this was the room with the Runestaff
        if (player->x == game->runestaff_location[0] &&
            player->y == game->runestaff_location[1] &&
            player->level == game->runestaff_location[2]) {
            print_message("\nGREAT ZOT! YOU'VE FOUND THE RUNESTAFF!\n");
            player->runestaff_flag = 1;
            game->runestaff_location[0] = 0;  // Mark as found
        }
    }

    int gold_found = random_number(1000);
    printf("\nYOU NOW GET HIS HOARD OF %d GP'S!\n", gold_found);
    player->gold += gold_found;

    // Clear the room
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
}

int handle_bribe(Player *player, GameState *game, const char *enemy_name)
{
    if (player->treasure_count == 0) {
        print_message("\nALL I WANT IS YOUR LIFE!\n");
        return 0;
    }

    int treasure_index = random_number(8) - 1;
    while (!game->treasure[treasure_index]) {
        treasure_index = random_number(8) - 1;
    }

    printf("\nI WANT %s. WILL YOU GIVE IT TO ME? ", get_treasure_name(treasure_index));
    char choice = get_user_input_yn();

    if (choice == 'Y') {
        game->treasure[treasure_index] = 0;
        player->treasure_count--;
        print_message("\nOK, JUST DON'T TELL ANYONE ELSE.\n");
        if (strcmp(enemy_name, "VENDOR") == 0) {
            game->vendor_attacked = 1;  // Vendor won't trade anymore
        }
        return 1;
    }
    return 0;
}

int handle_spell(Player *player, GameState *game, int *enemy_strength, const char *enemy_name)
{
    print_message("\nWHICH SPELL (WEB, FIREBALL, DEATHSPELL)? \n\n");
    char spell = get_user_input();

    switch (spell) {
        case 'W':
            player->strength--;
            if (player->strength <= 0) {
                game->game_over = 1;
                return 1;
            }
            player->web_count = random_number(8) + 1;  // Set web count to 1-9 turns
            printf("\nTHE %s IS STUCK AND CAN'T ATTACK FOR %d TURNS!\n", enemy_name, player->web_count);
            return 0;
        case 'F':
            player->strength--;
            player->intelligence--;
            if (player->strength <= 0 || player->intelligence <= 0) {
                game->game_over = 1;
                return 1;
            }
            int damage = random_number(7) + random_number(7);
            printf("\nIT DOES %d POINTS WORTH OF DAMAGE.\n", damage);
            *enemy_strength -= damage;
            if (*enemy_strength <= 0) {
                handle_combat_victory(player, game, 0, enemy_name);
                return 1;
            }
            return 0;

        case 'D':
            print_message("\nDEATH . . . ");
            if (player->intelligence < random_number(4) + 15) {
                print_message("YOURS!\n");
                player->intelligence = 0;
                game->game_over = 1;
                return 1;
            } else {
                print_message("HIS!\n");
                handle_combat_victory(player, game, 0, enemy_name);
                return 1;
            }

        default:
            print_message("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
            return 0;
    }
}

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

const char* get_monster_name(int room_content)
{
    switch (room_content) {
        case KOBOLD: return "KOBOLD";
        case ORC: return "ORC";
        case WOLF: return "WOLF";
        case GOBLIN: return "GOBLIN";
        case OGRE: return "OGRE";
        case TROLL: return "TROLL";
        case BEAR: return "BEAR";
        case MINOTAUR: return "MINOTAUR";
        case GARGOYLE: return "GARGOYLE";
        case CHIMERA: return "CHIMERA";
        case BALROG: return "BALROG";
        case DRAGON: return "DRAGON";
        default: return "UNKNOWN MONSTER";
    }
}