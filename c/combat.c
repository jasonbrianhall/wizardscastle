#include "combat.h"
#include "gamestate.h"
#include "utilities.h"
#include "wizardio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

void fight_monster(Player *player, GameState *game)
{
    int room_content = get_room_content(game, player->x, player->y, player->level);
    int is_vendor = (room_content == VENDOR);
    int enemy_strength, enemy_dexterity, enemy_intelligence, muted=0, spellcasted, temp, base_chance, success_chance, random_factor, avoidance_chance;
    int can_bribe = 1;
    const char *enemy_name = is_vendor ? "VENDOR" : get_monster_name(room_content);
    int firststrike, chance;  
    char choice;
    int max_increase;
    player->web_count=0;
    player->temp_strength=0;
    player->temp_intelligence=0;
    player->temp_dexterity=0;
    
    if (!is_vendor && player->race == HOBBIT && (random_number(10) == 1 || random_number(24 - player->dexterity)==1)) {
        print_message("Using your hobbit stealth (and the fact that you are very short), you sneak past the %s!\n", enemy_name);
        return;  // End combat successfully
    }
    
    if (!is_vendor && player->race == DROW && (random_number(12) == 1 || random_number(26 - player->dexterity)==1)) {
        print_message("Using your Drow stealth, you slip past the %s unnoticed!\n", enemy_name);
        return;  // End combat successfully
    }  

    if (!is_vendor && player->race == ELF && (random_number(13) == 1 || random_number(27 - player->dexterity)==1)) {
        print_message("Using your legendary Elf stealth, you slip past the %s unnoticed!\n", enemy_name);
        return;  // End combat successfully
    }  

    if (!is_vendor && player->race == HUMAN && (random_number(14) == 1 || random_number(27 - player->dexterity)==1)) {
        print_message("You slip past the %s unnoticed!\n", enemy_name);
        return;  // End combat successfully
    }  

    if (!is_vendor && player->race == DWARF && (random_number(15) == 1 || random_number(26 - player->dexterity) == 1)) {
        print_message("Using your Dwarven... er, 'aerodynamic' build, you somehow manage to roll past the %s unnoticed. Who knew being stout had its advantages?\n", enemy_name);
        return;  // End combat successfully
    }

    
    // Set enemy stats based on room content
    if (is_vendor) {
        enemy_strength = 15;
        enemy_dexterity = 15;
        enemy_intelligence = 15;
    } else {
        enemy_strength = (room_content - MONSTER_START + 1) * 2;
        enemy_dexterity = room_content - MONSTER_START + 8;
        enemy_intelligence = room_content - MONSTER_START + 8;
    }
    if (enemy_strength<=10)
    {
        enemy_strength=10;
    }

    firststrike=calculate_first_strike(player->dexterity, player->intelligence, player->strength, enemy_dexterity, enemy_intelligence, enemy_strength);
    
    if (firststrike==0)
    {
        print_message_formatted("ENEMY GETS FIRST STRIKE\n\n");
    }
    int rally_used=0;
    while (1) {
        if (firststrike==1)
        {
            print_message("\nYou're facing a "); 
            print_message_formatted("%s!\n\n", enemy_name);
            print_message("You may (A)ttack or (R)etreat.\n");
            if (can_bribe) {
                print_message("You can also attempt a (B)ribe.\n");
            }
            if (muted == 0 && (
                (player->intelligence >= 14) || // Base intelligence requirement for all races
                ((player->race == DWARF || player->race == ELF || player->race == DROW) && player->intelligence >= 10) || // Special races with lower requirement
                (player->race == HOBBIT && player->intelligence >= 11) // Hobbit specific requirement
            )) {
                print_message("You can also (C)ast a spell.\n");
            }
            if (player->race == HOBBIT) {
                print_message("You can also (T)hrow a stone.\n");
            }  
            if ((player->race == ELF || player->race == DROW) && player->intelligence>=8) {
                print_message("You can also (S)hoot a magical bow with extremely high accuracy.\n");
            }  

           if (player->race == HUMAN && rally_used == 0) {
                print_message("You can also Ra(L)ly.\n");
           }       

            print_message_formatted("\n");
            print_message_formatted("YOUR STRENGTH IS %d, YOUR DEXTERITY IS %d, AND YOUR INTELLIGENCE IS %d.\n", 
                   player->strength, player->dexterity, player->intelligence);

            print_message_formatted("THE ENEMIES STRENGTH IS %d, DEXTERITY IS %d, AND INTELLIGENCE IS %d.\n\n", 
                   enemy_strength, enemy_dexterity, enemy_intelligence);


            choice = get_user_input_custom_prompt("Fight command:  ");

            switch (choice) {
                case 'A':
                    if (player->weapon_type == 0) {
                        print_message_formatted("\n** POUNDING ON %s WON'T HURT IT!\n", enemy_name);
                    } else if (player->stickybook_flag) {
                        print_message_formatted("\n** YOU CAN'T BEAT IT TO DEATH WITH A BOOK!\n");
                    } else if (random_number(20) + player->dexterity <= random_number(20) + enemy_dexterity + (3 * player->blindness_flag)) {
                        print_message_formatted("\nYOU MISSED, TOO BAD!\n");
                    } else {
                        temp=calculate_damage(player, enemy_strength, enemy_dexterity);

                        // 20% chance of a critical hit as a Dwarf.
                        chance = player->blindness_flag == 1 ? 9 : 5;

                        if (player->race == DWARF && random_number(chance) == 1) {  // 20% chance
                            int extra_damage = random_number(player->strength / 3);
                            temp+=extra_damage;
                            print_message("You land a mighty blow on the ");
                            print_message_formatted("%s, dealing %d extra damage!  Total damage was %d.\n", enemy_name, extra_damage, temp);
                        }
                        else
                        {
                            print_message("\nYou hit the evil ");
                            print_message_formatted("%s AND DID %i DAMAGE!\n", enemy_name, temp);
                        }
                        enemy_strength -= temp;
                        if ((room_content == GARGOYLE || room_content == DRAGON) && random_number(8) == 1) {
                            if (player->weapon_type < 4 || random_number(8) == 1) // Excalibur can break but it's rare
                            {
                                print_message_formatted("\nOH NO! YOUR %s BROKE!\n", get_weapon_name(player->weapon_type));
                                player->weapon_type = 0;
                            }
                        }
                        if (enemy_strength <= 0) {
                            handle_combat_victory(player, game, is_vendor, enemy_name);
                            return;
                        }
                    }
                    break;
                case 'R':
                    temp=0;
                    if (player->race==HOBBIT)
                    {
                         // Hobbits are sneaky and more able to escape.
                         temp=player->dexterity/6;
                    }
                    if (random_number(20) + player->dexterity + temp > random_number(20) + enemy_dexterity) {
                        print_message("\nYou have escaped!\n");
                        if (player->race==HOBBIT)
                        {
                            print_message("Hobbitsons are sneaky.\n");
                        }
                        move_player_randomly(player, game);
                        return;
                    } else {
                        print_message_formatted("\nYOU FAILED TO RETREAT!\n");
                    }
                    break;
                case 'B':
                    if (!can_bribe) {
                        print_message_formatted("\n** CHOOSE ONE OF THE OPTIONS LISTED.\n");
                        continue;
                    }
                    if (handle_bribe(player, game, enemy_name)) {
                        return;
                    }
                    can_bribe = 0;
                    break;
                case 'C':
                   if (muted == 0 && (
                       (player->intelligence >= 14) || // Base intelligence requirement for all races
                       ((player->race == DWARF || player->race == ELF || player->race == DROW) && player->intelligence >= 10) || // Special races with lower requirement
                       (player->race == HOBBIT && player->intelligence >= 11) // Hobbit specific requirement
                   )) {
                        // Player can cast a spell
                        if (handle_spell(player, game, &enemy_strength, &enemy_intelligence, &enemy_dexterity, enemy_name)) {
                            if (game->game_over)
                            {
                                return;
                            }
                            if (enemy_strength <= 0) {
                                handle_combat_victory(player, game, is_vendor, enemy_name);
                                return;
                            }
                        }
                    } else {
                        // Player cannot cast a spell
                        print_message_formatted("\n** YOU CAN'T CAST A SPELL NOW!\n");
                        continue;  // Skip enemy's turn if player couldn't cast a spell
                    }
                    break;
                case 'S':  // Shoot magical bows
                    if ((player->race == ELF || player->race == DROW) && player->intelligence>=8) 
                    {
                        chance = player->blindness_flag == 1 ? 4 : 5;

                        if (random_number(chance) >= 4 || random_number(chance) >= 4 || random_number(24-min(18, player->dexterity))==1)  // 3/5 twice plus dexterity based chance
                        {
                            chance = player->blindness_flag == 1 ? 12 : 8;

                            if (player->race == DROW && random_number(chance) == 1) {  // 12.5% chance
                                temp = random_number(4)+1;  // 2-5 damage
                                print_message("Dark Elf Precision! Your strike finds a vital spot!  You hit the enemy for %d damage!\n", temp);
                            }
                            else if (player->race == ELF)
                            {
                               temp = random_number(3) + 1;  // 2-4 damage
                               print_message("With elven grace, your arrow flies true! You hit the enemy for %d damage!\n", temp);
                            }
                            else
                            {
                                temp = random_number(3);  // 1-3 damage
                                print_message_formatted("You pull out your magical bow and hit the enemy for %d damage!\n", temp);
                            }
                            enemy_strength-=temp;
                            if (enemy_strength <= 0) {
                                handle_combat_victory(player, game, is_vendor, enemy_name);
                                return;
                            }
                        }
                        else
                        {
                           if (player->race == ELF)
                           {
                               print_message("Despite your elven skill, your arrow misses its mark. Even the best archers have off days.\n");
                           }
                           else
                           {
                               print_message("You pull out your magic bow and... you miss the enemy. Your aim wasn't true.\n");
                           }                        
                        }
                        break;
                    }
                    print_message("You don't have any magical arrows.\n");
                    continue;
                case 'T':
                    if (player->race == HOBBIT) 
                    {
                        chance = player->blindness_flag == 1 ? 9 : 6;
                        if (random_number(chance) == 1 || random_number(chance) == 1 || random_number(24-min(18, player->dexterity))==1) {  // Two 20% chances plus dexterity-based chance
                            temp = random_number(3);  // 1-3 damage
                            print_message_formatted("You throw a stone and hit the enemy for %d damage!\n", temp);
                            enemy_strength  -= temp;
                            if (enemy_strength <= 0) {
                                handle_combat_victory(player, game, is_vendor, enemy_name);
                                return;
                            }
                        }
                        else 
                        {
                             print_message("Your thrown stone misses the enemy.\n");
                        }
                    }
                    else
                    {
                         print_message("Do you look like a hobbit?  You can't cast stones.\n");
                    }
                    continue;
                 case 'L':  // Rally
                     if (player->race == HUMAN && rally_used==0) {
                         int rally_bonus = random_number(2) + 1;
                         player->temp_strength = player->strength+1;
                         player->temp_intelligence = player->intelligence+1;
                         player->temp_dexterity = player->dexterity+1;
                         player->strength += rally_bonus;
                         player->dexterity+= rally_bonus;
                         player->intelligence += rally_bonus;
                         print_message("You rally your spirit, gaining %d to all stats temporarily!\n", rally_bonus);
                         rally_used=1;
                         break;
                     } else if (player->race != HUMAN) {
                         print_message("Only humans can use Rally.\n");
                     } else {
                         print_message("You've already used Rally this battle.\n");
                     }
                     continue;
                default:
                    print_message_formatted("\n** CHOOSE ONE OF THE OPTIONS LISTED.\n");
                    continue;
            }
        } else {
            firststrike=1;
            choice='\0'; // Enemy has first strike
        }
        
        // Enemy's turn
        if (player->web_count > 0) {
            player->web_count--;
            if (player->web_count == 0) {
                print_message_formatted("\nTHE WEB JUST BROKE!\n");
            } else {
                print_message_formatted("\nTHE %s IS STUCK AND CAN'T ATTACK NOW!\n", enemy_name);
                continue;  // Skip the enemy's attack
            }
        }
        else if (room_content == DRAGON && random_number(3) == 1) {  // 1 in 3 chance for fireball
            print_message_formatted("\nTHE %s ATTACKS!\n", enemy_name);
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
        else if ((room_content == KOBOLD && random_number(4)==1) || (room_content == DRAGON && random_number(3)==1))
        {
            spellcasted=random_number(7);
            switch (room_content)
            {
                case KOBOLD:
                    max_increase=3;
                    break;
                case DRAGON:
                    max_increase=5;
                    break;
            }
            switch (spellcasted)
            {
                 case 1:
                     temp=random_number(max_increase);
                     print_message_formatted("THE %s CASTS HEAL AND GAINED %i STRENGTH POINTS\n", enemy_name, temp);
                     enemy_strength+=temp;
                     break;                         
                 case 2:
                     temp=random_number(max_increase);
                     print_message_formatted("THE %s CASTS HASTE AND GAINED %i DEXTERITY POINTS\n", enemy_name, temp);
                     enemy_dexterity+=temp;
                     break;                         
                 case 3:
                     temp=random_number(max_increase);
                     print_message_formatted("THE %s CASTS BRIGHT AND GAINED %i INTELLIGENCE POINTS\n", enemy_name, temp);
                     enemy_intelligence+=temp;
                     break;                         
                 case 4:
                     print_message_formatted("THE %s CASTS SILENCE; ", enemy_name);
                     base_chance = 50 + (player->intelligence - enemy_intelligence) * 5;

                     // Add a random factor (-10 to +10)
                    random_factor = (rand() % 21) - 10;

                    // Calculate final success chance
                    success_chance = base_chance + random_factor;

                    // Ensure the success chance is within a reasonable range (5 to 95)
                    if (success_chance < 5) success_chance = 5;
                    if (success_chance > 95) success_chance = 95;

                    // Determine if the spell succeeds
                    if (rand() % 100 < success_chance) {
                         print_message_formatted("YOU'VE BEEN MUTED.  YOU ARE NOW UNABLE TO CAST SPELLS UNTIL THE END OF COMBAT.\n");
                         muted=1;
                    } else {
                        print_message_formatted("THE SPELL FAILED.  YOU SUCCESSFULLY RESISTED THE SPELL\n");
                        muted=0; // Spell fails, can still cast
                    }
                    break;
                 case 5:
                     print_message_formatted("THE %s CASTS WEAKNESS!\n", enemy_name);
    
                     // Calculate avoidance chance based on player stats
                     avoidance_chance = (player->intelligence * 2 + player->strength + player->dexterity) / 4;
    
                     // Add a random factor
                     avoidance_chance += random_number(10) - 5;  // -5 to +5 random adjustment
    
                     // Ensure the avoidance chance is within a reasonable range (5% to 95%)
                     if (avoidance_chance < 5) avoidance_chance = 5;
                     if (avoidance_chance > 95) avoidance_chance = 95;
    
                     if (random_number(100) < avoidance_chance) {
                         print_message_formatted("YOU SUCCESSFULLY RESIST THE WEAKNESS SPELL!\n");
                     } else {
                         temp = random_number(max_increase);
                         print_message_formatted("THE SPELL HITS! YOU LOSE %i STRENGTH POINTS\n", temp);
                         player->strength -= temp;
        
                         if (player->strength <= 0) {
                             player->strength = 0;
                             print_message_formatted("YOUR STRENGTH HAS BEEN REDUCED TO ZERO. YOU COLLAPSE...\n");
                             game->game_over = 1;
                             return;
                         }
                     }
                     break;   
                 case 6:
                    print_message_formatted("THE %s CASTS CLUMSY!\n", enemy_name);
    
                     // Calculate avoidance chance based on player stats
                     avoidance_chance = (player->intelligence * 2 + player->strength + player->dexterity) / 4;
    
                     // Add a random factor
                     avoidance_chance += random_number(10) - 5;  // -5 to +5 random adjustment
    
                     // Ensure the avoidance chance is within a reasonable range (5% to 95%)
                     if (avoidance_chance < 5) avoidance_chance = 5;
                     if (avoidance_chance > 95) avoidance_chance = 95;
    
                     if (random_number(100) < avoidance_chance) {
                         print_message_formatted("YOU SUCCESSFULLY RESIST THE CLUMSY SPELL!\n");
                     } else {
                         temp = random_number(max_increase);
                         print_message_formatted("THE SPELL HITS! YOU LOSE %i DEXTERITY POINTS\n", temp);
                         player->dexterity -= temp;
        
                         if (player->dexterity <= 0) {
                             player->dexterity = 0;
                             print_message_formatted("YOUR DEXTERITY HAS BEEN REDUCED TO ZERO. YOU COLLAPSE...\n");
                             game->game_over = 1;
                             return;
                         }
                     }
                     break;
                 case 7:
                    print_message_formatted("THE %s CASTS MIND FOG!\n", enemy_name);
    
                     // Calculate avoidance chance based on player stats
                     avoidance_chance = (player->intelligence * 2 + player->strength + player->dexterity) / 4;
    
                     // Add a random factor
                     avoidance_chance += random_number(10) - 5;  // -5 to +5 random adjustment
    
                     // Ensure the avoidance chance is within a reasonable range (5% to 95%)
                     if (avoidance_chance < 5) avoidance_chance = 5;
                     if (avoidance_chance > 95) avoidance_chance = 95;
    
                     if (random_number(100) < avoidance_chance) {
                         print_message_formatted("YOU SUCCESSFULLY RESIST THE MING FOG SPELL!\n");
                     } else {
                         temp = random_number(max_increase);
                         print_message_formatted("THE SPELL HITS! YOU LOSE %i INTELLIGENCE POINTS\n", temp);
                         player->intelligence -= temp;
        
                         if (player->intelligence <= 0) {
                             player->intelligence = 0;
                             print_message_formatted("YOUR INTELLIGENCE HAS BEEN REDUCED TO ZERO. YOU COLLAPSE...\n");
                             game->game_over = 1;
                             return;
                         }
                     }
                     break;

                                             
            }
        }
        else if (enemy_attack_hits(player, enemy_dexterity)) {
            print_message_formatted("THE %s ATTACKS\n", enemy_name);
            print_message_formatted("\nOUCH! HE HIT YOU!\n");
            int damage = calculate_damage_enemy(player, enemy_strength, enemy_dexterity, (room_content-MONSTER_START)/3 +1 );
            
            player->armor_points-=damage;
            if (player->armor_type>0 && player->armor_points <= 0) {
                player->armor_points = 0;
                player->armor_type = 0;
                print_message_formatted("\nYOUR ARMOR HAS BEEN DESTROYED... GOOD LUCK!\n");
            }
            
            player->strength -= damage;
            print_message_formatted("YOU TAKE %d DAMAGE!\n", damage);
            
            if (player->strength <= 0) {
                print_message_formatted("\nYOU DIED DUE TO LACK OF STRENGTH.\n");
                game->game_over = 1;
                return;
            }
        } else {
            print_message_formatted("THE %s ATTACKS\n", enemy_name);
            print_message_formatted("\nWHAT LUCK, HE MISSED YOU!\n");
        }
    }
}

void handle_combat_victory(Player *player, GameState *game, int is_vendor, const char *enemy_name)
{
    print_message_formatted("\n%s LIES DEAD AT YOUR FEET!\n", enemy_name);
    
    if (random_number(5) == 1) {  // 20% chance of eating
        print_message_formatted("\nYOU SPEND AN HOUR EATING %s%s.\n", enemy_name, get_random_body_part());
    }

    if (is_vendor) {
        print_message_formatted("\nYOU GET ALL HIS WARES:\n");
        print_message_formatted("PLATE ARMOR\n");
        player->armor_type = 3;
        player->armor_points = 21;
        print_message_formatted("A SWORD\n");
        player->weapon_type = 3;
        print_message_formatted("A STRENGTH POTION\n");
        player->strength = min(player->strength + random_number(6), 18);
        print_message_formatted("AN INTELLIGENCE POTION\n");
        player->intelligence = min(player->intelligence + random_number(6), 18);
        print_message_formatted("A DEXTERITY POTION\n");
        player->dexterity = min(player->dexterity + random_number(6), 18);
        if (!player->lamp_flag) {
            print_message_formatted("A LAMP\n");
            player->lamp_flag = 1;
        }
    } else {
        // Check if this was the room with the Runestaff
        if (player->x == game->runestaff_location[0] &&
            player->y == game->runestaff_location[1] &&
            player->level == game->runestaff_location[2]) {
            print_message_formatted("\nGREAT ZOT! YOU'VE FOUND THE RUNESTAFF!\n");
            player->runestaff_flag = 1;
            game->runestaff_location[0] = 0;  // Mark as found
        }
    }

    int gold_found = random_number(1000);
    print_message_formatted("\nYOU NOW GET HIS HOARD OF %d GP'S!\n", gold_found);
    player->gold += gold_found;

    // Clear the room
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
}

int handle_bribe(Player *player, GameState *game, const char *enemy_name)
{
    if (player->treasure_count == 0) {
        print_message_formatted("\nALL I WANT IS YOUR LIFE!\n");
        return 0;
    }

    int treasure_index = random_number(8) - 1;
    while (!game->treasure[treasure_index]) {
        treasure_index = random_number(8) - 1;
    }

    print_message_formatted("\nI WANT %s. WILL YOU GIVE IT TO ME? ", get_treasure_name(treasure_index));
    char choice = get_user_input_yn();

    if (choice == 'Y') {
        game->treasure[treasure_index] = 0;
        player->treasure_count--;
        print_message_formatted("\nOK, JUST DON'T TELL ANYONE ELSE.\n");
        if (strcmp(enemy_name, "VENDOR") == 0) {
            game->vendor_attacked = 1;  // Vendor won't trade anymore
        }
        return 1;
    }
    return 0;
}

int handle_spell(Player *player, GameState *game, int *enemy_strength, int *enemy_intelligence, int *enemy_dexterity, const char *enemy_name)
{
    int damage;
    print_message_formatted("\nWHICH SPELL\n");
    if(player->intelligence >=14)
    {
        print_message_formatted("    (W)EB - Casts a magical web that prevents a monster from attacking\n");
        print_message_formatted("    (F)IREBALL - Casts a Fireball at the enemy\n");
    }
    if(player->intelligence>=14 && (player->race == DWARF || player->race == DROW))
    {
        print_message(          "   S(T)one Skin; improves your armor at the cost of one strength and intelligence points\n");
    }    
    if (player->intelligence >= 16)
    {
        print_message_formatted("    (D)EATHSPELL - Casts a Deathspell; be warned you may die\n");
    }
    if ((player->race == ELF || player->race == DROW || player->race == DWARF) && player->intelligence>=10)
    {
        print_message_formatted("    (H)EAL - Permanently heals you (but maxes out at 18 after combat); costs one intelligence points\n");
        print_message_formatted("    (S)PEED - Temporarily increases your dexterity\n");
        print_message_formatted("    (B)RIGHT - Temporarily increases your intelligence \n");
    }
    if ((player->race == HOBBIT ) && player->intelligence>=11)
    {
       print_message_formatted("    (M)ischief Hobbit Magic with Chaos Effect\n");
    }
    print_message_formatted("\n");
    char spell = get_user_input_custom_prompt("Which Spell:  ");
    for (;;) {
        switch (spell) {
            case 'W':
                player->strength--;
                if (player->strength <= 0) {
                    game->game_over = 1;
                    return 1;
                }
                player->web_count = random_number(8) + 1;  // Set web count to 1-9 turns
                print_message_formatted("\nTHE %s IS STUCK AND CAN'T ATTACK FOR %d TURNS!\n", enemy_name, player->web_count);
                return 0;
            case 'F':
                player->strength--;
                player->intelligence--;
                if (player->strength <= 0 || player->intelligence <= 0) {
                    game->game_over = 1;
                    return 1;
                }
                damage = random_number(7) + random_number(7);
                print_message_formatted("\nIT DOES %d POINTS WORTH OF DAMAGE.\n", damage);
                *enemy_strength -= damage;
                if (*enemy_strength <= 0) {
                    handle_combat_victory(player, game, 0, enemy_name);
                    return 1;
                }
                return 0;

            case 'D':
                print_message_formatted("\nDEATH . . . ");
                if (calculate_death_spell(player->intelligence, player->strength, player->dexterity, *enemy_intelligence, *enemy_strength, *enemy_dexterity)) {
                    print_message_formatted("YOURS!\n");
                    player->intelligence = 0;
                    game->game_over = 1;
                    return 1;
                } else {
                    print_message_formatted("HIS!\n");
                    //handle_combat_victory(player, game, 0, enemy_name)
                    *enemy_intelligence=0;
                    *enemy_strength=0;
                    *enemy_dexterity=0;
                    return 1;
                }
            case 'H':
                if ((player->race == ELF || player->race == DROW || player->race == DWARF) && player->intelligence>=10)
                {
                    cast_heal_spell(player);
                    return 0;
                }
                print_message_formatted("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
                break;
            case 'S':
                if ((player->race == ELF || player->race == DROW || player->race == DWARF) && player->intelligence>=10)
                {
                    cast_haste_spell(player);
                    return 0;
                }
                print_message_formatted("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
                break;
            case 'B':
                if ((player->race == ELF || player->race == DROW || player->race == DWARF) && player->intelligence>=10)
                {
                    cast_bright_spell(player);
                    return 0;
                }
                print_message_formatted("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
                break;
            case 'T':
                if ((player->race == DWARF || player->race == DROW) && player->intelligence>=14)
                {
                    cast_stone_skin_spell(player);
                    return 0;
                }
                print_message_formatted("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
                break;
            case 'M':  // Mischief Blast for Hobbits
                if (player->race == HOBBIT && player->intelligence >= 11)
                {
                    cast_mischief_blast(player, enemy_strength, enemy_dexterity, enemy_intelligence);
                    player->intelligence -= 1;
                    
                    if (*enemy_strength <= 0) {
                        print_message_formatted("Your chaotic magic overwhelms the enemy!  They have died from lack of strength!\n");
                        return 1;
                    }
                    if (*enemy_dexterity <= 0) {
                        print_message_formatted("Your chaotic magic overwhelms the enemy!  They have died from lack of dexterity\n");
                        return 1;
                    }
                    if (*enemy_intelligence <= 0) {
                        print_message_formatted("Your chaotic magic overwhelms the enemy!  They have died from lack of intelligence\n");
                        return 1;
                    }

                    return 0;
                }
                print_message_formatted("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
                break;

            default:
                print_message_formatted("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
        }
    }
}


void dragon_fireball_attack(Player *player, GameState *game) {
    print_message_formatted("\nThe dragon breathes a massive fireball at you!\n");
    
    // Dexterity-based avoidance check
    if (random_number(20) + player->dexterity > random_number(20) + 15) {  // Dragon has high dexterity
        print_message_formatted("You manage to dodge the fireball!\n");
        return;
    }
    
    int damage = random_number(10) + 5;  // 6 to 15 damage
    print_message_formatted("The fireball hits you for %d damage!\n", damage);
    
    // Apply armor reduction
    if (player->armor_type != 0) {
        int armor_protection = player->armor_type + random_number(3)-1;
        damage -= armor_protection;
        player->armor_points -= armor_protection;
        
        if (damage < 0) {
            player->armor_points += damage;  // Adjust for overkill protection
            damage = 0;
        }
        
        print_message_formatted("Your armor absorbs %d damage.\n", armor_protection);
        
        if (player->armor_points <= 0) {
            player->armor_points = 0;
            player->armor_type = 0;
            print_message_formatted("YOUR ARMOR HAS BEEN DESTROYED BY THE DRAGON'S FIRE!\n");
        }
    }
    
    player->strength -= damage;
    print_message_formatted("You take %d final damage from the fireball!\n", damage);
    
    if (player->strength <= 0) {
        print_message_formatted("\nYOU HAVE BEEN INCINERATED BY THE DRAGON'S FIRE!  GOOD LUCK ADVENTURER.\n");
        game->game_over = 1;
    }
}

void balrog_flame_whip_attack(Player *player, GameState *game) {

    print_message_formatted("\nThe Balrog lashes out with its flame whip!\n");
    
    // Dexterity-based avoidance check
    if (random_number(20) + player->dexterity > random_number(20) + 18) {  // Balrog has very high dexterity
        print_message_formatted("You narrowly avoid the searing flames!\n");
        return;
    }
    
    int damage = random_number(8) + 4;  // 5 to 12 damage
    print_message_formatted("The flame whip strikes you for %d damage!\n", damage);
    
    // Apply armor reduction
    if (player->armor_type != 0) {
        int armor_protection = player->armor_type + random_number(3)-1;
        damage -= (armor_protection + random_number(3)-1);
        
        player->armor_points -= armor_protection;
        if (damage < 0) {
            player->armor_points += damage;  // Adjust for overkill protection
            damage = 0;
        }
        
        print_message_formatted("Your armor absorbs %d damage.\n", armor_protection);
        
        if (player->armor_points <= 0) {
            player->armor_points = 0;
            player->armor_type = 0;
            print_message_formatted("YOUR ARMOR HAS BEEN MELTED BY THE BALROG'S FLAME WHIP!  GOOD LUCK ADVENTURER.\n");
        }
    }
    
    player->strength -= damage;
    print_message_formatted("You take %d final damage from the flame whip!\n", damage);
    
    if (player->strength <= 0) {
        print_message_formatted("\nYOU HAVE BEEN INCINERATED BY THE BALROG'S FLAME WHIP!\n");
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

int cast_heal_spell(Player *player) {
    if (player->intelligence > 9) {
        int heal_amount = random_number(5); // 1-5 Points
        player->strength += heal_amount;
        player->intelligence -=1;
        print_message_formatted("YOUR HEALTH INCREASED BY %i POINTS.\n\n", heal_amount);
        return 1;
    }
    return 0;
}

int cast_stone_skin_spell(Player *player)
{
    if(player->intelligence>=14)
    {
        if(player->armor_type<4)
        {
             player->armor_type+=1;
             player->intelligence-=1;
             player->strength-=1;
             player->armor_points=21;
             if(player->armor_type==0)
             {
                 print_message("Your armor has improved!!!");
             }
             else
             {
                 print_message("Magical armor has now appeared.");
             }
        }
        else
        {
             print_message("You can't improve your armor.");
        }
        player->armor_points*=2;
        if(player->armor_points>50)
        {
            player->armor_points=50;
        }

        return 1;
    }
    else
    {
        return 0;
    }

}

int cast_bright_spell(Player *player) {
    if (player->intelligence > 9) {
        int bright_amount = random_number(5) + 1;  // Increase intelligence by 2-6 points
        if (player->temp_intelligence == 0)
        {
            player->temp_intelligence=player->intelligence;
        }
        player->intelligence += bright_amount;
        print_message_formatted("YOUR INTELLIGENCE TEMPORARILY INCREASED BY %i POINTS.\n\n", bright_amount);
        return 1;
    }
    return 0;
}

int cast_haste_spell(Player *player) {
    if (player->intelligence > 9) {
        int haste_amount = random_number(5) + 1;  // Increase dexterity by 2-6 points
        if (player->temp_dexterity == 0)
        {
            player->temp_dexterity=player->dexterity;
        }
        player->dexterity += haste_amount;
        print_message_formatted("YOUR DEXTERITY TEMPORARILY INCREASED BY %i POINTS.\n\n", haste_amount);
        return 1;
    }
    return 0;
}

// Calculate first strike based on dexterity, intelligence, and strength
int calculate_first_strike(int player_dex, int player_int, int player_str, 
                           int enemy_dex, int enemy_int, int enemy_str) {
    // Base the calculation primarily on dexterity
    int player_score = player_dex * 2;
    int enemy_score = enemy_dex * 2;
    
    // Add some influence from intelligence and strength
    player_score += player_int / 2 + player_str / 4;
    enemy_score += enemy_int / 2 + enemy_str / 4;
    
    // Add a small random factor
    player_score += random_number(5);
    enemy_score += random_number(5);
    
    // Determine who strikes first
    if (player_score > enemy_score) {
        return 1; // Player strikes first
    } else if (player_score < enemy_score) {
        return 0; // Enemy strikes first
    } else {
        // If scores are equal, randomize
        return random_number(2);
    }
}

int calculate_death_spell(int caster_int, int caster_str, int caster_dex, 
                          int target_int, int target_str, int target_dex) {
    // Base the calculation primarily on intelligence
    int spell_power = caster_int * 3;
    int target_resistance = target_int * 2;
    
    // Add some influence from strength and dexterity
    spell_power += caster_str / 3 + caster_dex / 3;
    target_resistance += target_str / 4 + target_dex / 4;
    
    // Add a small random factor
    spell_power += random_number(10);
    target_resistance += random_number(10);
    
    // Calculate spell success chance (0 to 100)
    int success_chance = spell_power - target_resistance;
    
    // Ensure the success chance is within a reasonable range (5 to 95)
    if (success_chance < 5) success_chance = 5;
    if (success_chance > 95) success_chance = 95;
    
    // Determine if the spell succeeds
    if (random_number(100) < success_chance) {
        return 0; // Spell succeeds, enemy dies
    } else {
        return 1; // Spell fails, enemy lives
    }
}

int calculate_damage(Player *player, int enemy_strength, int enemy_dexterity) {
    // Base damage from weapon
    int base_damage = player->weapon_type;
    int weapon_bonus;  
    
    // Player's offensive bonuses
    int strength_bonus = player->strength / 9;  // Every 9 points of strength adds 1 to damage
    int dexterity_bonus = player->dexterity / 6;  // Every 6 points of dexterity adds 1 to damage
    
    // Enemy's defensive bonuses
    int enemy_strength_defense = enemy_strength / 9;  // Every 9 points of enemy strength reduces damage by 1
    int enemy_dexterity_defense = enemy_dexterity / 6;  // Every 5 points of enemy dexterity reduces damage by 1
    
    // Calculate initial damage
    int total_damage = base_damage + strength_bonus + dexterity_bonus;
    
    // Reduce damage based on enemy stats
    total_damage -= (enemy_strength_defense + enemy_dexterity_defense);
    
    // Add a random factor
    int random_factor = random_number(5) - 3;  // -2 to +2 random adjustment
    total_damage += random_factor;

    if (player->race == HOBBIT && random_number(10) == 1) {  // 10% chance
        print_message("Lucky Strike! You found a weak spot!\n");
        total_damage *= 2;  // Double the damage
    }
    
    if (player->race == HUMAN) {
        weapon_bonus = random_number(3)-1;  // 0-2 extra damage
        total_damage += weapon_bonus;
        if (weapon_bonus > 0) {
            print_message_formatted("Your weapon mastery deals %d extra damage!\n", weapon_bonus);
        }
    }
    
    if (total_damage<=0)
    {
        random_factor=random_number(3);
        if (random_factor==1)
        {
           total_damage=random_factor;
        }
        else
        {
           total_damage=0;
        }
    }

    return total_damage;
}

int calculate_damage_enemy(Player *player, int enemy_strength, int enemy_dexterity, int base_damage) {
    // Enemy's offensive bonuses
    int strength_bonus = enemy_strength / 9;  // Every 9 points of strength adds 1 to damage
    int dexterity_bonus = enemy_dexterity / 6;  // Every 5 points of dexterity adds 1 to damage
    
    // Player's defensive bonuses
    int player_strength_defense = player->strength / 9;  // Every 4 points of player strength reduces damage by 1
    int player_dexterity_defense = player->dexterity / 6;  // Every 5 points of player dexterity reduces damage by 1
    
    // Calculate initial damage
    int total_damage = base_damage + strength_bonus + dexterity_bonus;
    // Reduce damage based on player stats
    total_damage -= (player_strength_defense + player_dexterity_defense);
    // Add a random factor
    int random_factor = random_number(5) - 3;  // -2 to +2 random adjustment
    total_damage += random_factor;

    // Apply armor reduction if player has armor
    if (player->armor_type != 0) {
        total_damage -= player->armor_type;
    }
    
    if (total_damage<=0)
    {
        random_factor=random_number(3);
        if (random_factor==1)
        {
           total_damage=random_factor;
        }
        else
        {
           total_damage=0;
        }
    }
    
    // Damage can be zero or negative (will be treated as zero)
    return total_damage;
}

int enemy_attack_hits(Player *player, int enemy_dexterity) {
    // Base hit chance (out of 100)
    int hit_chance = 50;
    
    // Adjust for enemy dexterity (each point above 10 increases hit chance by 2)
    hit_chance += (enemy_dexterity - 10) * 2;
    
    // Adjust for player dexterity (each point above 10 decreases hit chance by 2)
    hit_chance -= (player->dexterity - 10) * 2;
    
    // Adjust for player's blindness
    hit_chance += 15 * player->blindness_flag;
    
    // Ensure hit chance is within 5 to 95 range
    if (hit_chance < 5) hit_chance = 5;
    if (hit_chance > 95) hit_chance = 95;
    
    // Generate a random number between 1 and 100
    int roll = random_number(100);
    
    // Return 1 if the attack hits, 0 if it misses
    return (roll <= hit_chance) ? 1 : 0;
}

int cast_mischief_blast(Player *player, int *enemy_strength, int *enemy_dexterity, int *enemy_intelligence) {
    int blast_count = random_number(4);  // 1-4 blasts
    int total_damage = 0;
    int damage;
    
    print_message_formatted("\nYou channel chaotic hobbit magic!\n");
    print_message_formatted("You unleash %d magical blasts!\n", blast_count);
    
    for (int i = 0; i < blast_count; i++) {
        damage = random_number(4);  // 1-5 damage per blast
        
        // Each blast has a special effect chance
        if (random_number(5) >= 4) {
            switch(random_number(6)) {
                case 1:
                    damage *= 2;
                    print_message_formatted("Blast %d: BOOM! Double damage! (%d damage)\n", i+1, damage);
                    break;
                case 2:
                    *enemy_dexterity -= 1;
                    print_message_formatted("Blast %d: ZAP! Enemy loses 1 dexterity and takes %d damage\n", i+1, damage);
                    break;
                case 3:
                    *enemy_intelligence -= 1;
                    print_message_formatted("Blast %d: PING! Enemy loses 1 intelligence and takes %d damage\n", i+1, damage);
                    break;
                case 4:
                    player->intelligence+=1;
                    print_message_formatted("Blast %d: Oops! You hit yourself and gained intelligence!!!\n", i+1, damage);
                    break;
                case 5:
                    player->strength+=1;
                    print_message_formatted("Blast %d: Oops! You hit yourself and gained strength!!!\n", i+1, damage);
                    break;
                case 6:
                    player->dexterity+=1;
                    print_message_formatted("Blast %d: Oops! You hit yourself and gained dexterity!!!\n", i+1, damage);
                    break;


            }
        } else {
            print_message_formatted("Blast %d: Deals %d damage\n", i+1, damage);
        }
        total_damage += damage;
    }
    
    print_message_formatted("\nTotal damage dealt: %d\n", total_damage);
    *enemy_strength -= total_damage;
    
    return total_damage;  // Return total damage dealt for any additional processing
}

