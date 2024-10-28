#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "vendor.h"
#include "utilities.h"
#include "combat.h"
#include "player.h"
#include "wizardio.h"

void handle_vendor(Player *player, GameState *game)
{
    print_message_formatted("\nYou've encountered a vendor!\n");
    
    // Check if the player has attacked the vendor before
    if (game->vendor_attacked) {
        fight_monster_normalize(player, game);
        return;
    }

    char choice;
    do {
        print_message("\nDo you want to (T)rade, (A)ttack, or (I)gnore the vendor?\n");
        choice = get_user_input_custom_prompt("?  ");

        switch(choice) {
            case 'T':
                trade_with_vendor(player, game);
                return;
            case 'A':
                attack_vendor(player, game);
                return;
            case 'I':
                print_message_formatted("You ignore the vendor and move on.\n");
                return;
            default:
                print_message("Invalid choice. Please choose T, A, or I.\n");
        }
    } while (1);
}

// To do; make option choices more dynamic;  Like only offer stuff they can afford and put the numbers in order
void trade_with_vendor(Player *player, GameState *game)
{
    // Offer to buy treasures
    int i=0;
    //int optionchoice=7;
    for (i = 0; i < TREASURE_COUNT; i++) {
        if (game->treasure[i]) {
            int offer = random_number(1500) * (i + 1);
            print_message("Do you want to sell %s for %d GP? (Y/N) ", get_treasure_name(i), offer);
            
            if (get_user_input_yn() == 'Y') {
                player->gold += offer;
                game->treasure[i] = 0;
                player->treasure_count--;
                print_message_formatted("Sold!\n");
            }
            else
            { 
               if (random_number(8)==1 && player->weapon_type<EXCALIBUR)
               {
                   print_message("I'll give you an extremely rare offer; trade %s for Excalibur? (Y/N) ", get_treasure_name(i), offer);
                   if (get_user_input_yn() == 'Y') {
                       game->treasure[i] = 0;
                       player->treasure_count--;
                       player->weapon_type=4;
                   }        
               }
               else
                   if (random_number(8)==1 && player->armor_type<STONE)
                   {
                       print_message("I'll give you an extremely rare offer; trade %s for Stone Armor? (Y/N) ", get_treasure_name(i), offer);
                       if (get_user_input_yn() == 'Y') {
                           game->treasure[i] = 0;
                           player->treasure_count--;
                           player->armor_type=4;
                           player->armor_points=42;
                       }        
                   }          
            }
        }
    }

    // Offer to sell items if player has enough gold
    while (player->gold >= 10) {
        print_message("\n\nThese are the services I offer:\n");
        print_message("    1. Improve Strength (500 GP)\n");
        print_message("    2. Improve Intelligence (500 GP)\n");
        print_message("    3. Improve Dexterity (500 GP)\n");
        print_message("    4. Armor (1250-2000 GP); repair cost at 100\n");
        print_message("    5. Weapon (1250-2000 GP)\n");
        print_message("    6. Lamp (1000 GP)\n");
        print_message("    7. Flares (10 GP)\n");
        print_message("    8. Cure Blindness (1000 GP)\n");
        print_message("    9. Remove Sticky Book (1000 GP)\n");
        print_message("    0. Nothing\n\n");
        print_message("You have %d GP to spend\n\n", player->gold);
        

        char purchase_choice = get_user_input();
        switch(purchase_choice) {
            case '1':
                if(player->gold>=500)
		{
                    if (player->strength < MAX_STRENGTH)
                    {
                        print_message_formatted("\n%s, I have increased your strength\n", player->sex == MALE ? "SIR" : "MA'AM");
                        player->strength = get_minimum(player->strength + random_number(6), MAX_STRENGTH);
                        player->gold -= 500;
                    }
                    else
                    {
                        print_message_formatted("\n%s, YOU ARE ALREADY HERCULES\n", player->sex == MALE ? "SIR" : "MA'AM");
                    }
                }
                else
                {
                    print_message_formatted("\n%s YOU ARE TOO POOR TO BUY A STRENGTH POTION\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
                break;
            case '2':
                if(player->gold>=500)
		{
                    if (player->intelligence < MAX_INTELLIGENCE)
                    {
                        print_message_formatted("\n%s, I have increased your intelligence\n", player->sex == MALE ? "SIR" : "MA'AM");
                        player->intelligence = get_minimum(player->intelligence + random_number(6), MAX_INTELLIGENCE);
                        player->gold -= 500;
                    }
                    else
                    {
                        print_message_formatted("\n%s, YOU ARE ALREADY A GENIUS ... GENIUS\n", player->sex == MALE ? "SIR" : "MA'AM");
                    }
                }
                else
                {
                    print_message_formatted("\n%s YOU ARE TOO POOR TO BUY A STRENGTH POTION\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
                break;
            case '3':
                if(player->gold>=500)
		{
                    if (player->dexterity < MAX_DEXTERITY)
                    {
                        print_message_formatted("\n%s, I have made you more nimble\n", player->sex == MALE ? "SIR" : "MA'AM");
                        player->dexterity = get_minimum(player->dexterity + random_number(6), MAX_DEXTERITY);
                        player->gold -= 500;
                    }
                    else
                    {
                        print_message_formatted("\n%s, YOU ARE ALREADY NIMBLE ... GENIUS\n", player->sex == MALE ? "SIR" : "MA'AM");
                    }
                }
                else
                {
                    print_message_formatted("\n%s, YOU ARE TOO POOR TO BUY A STRENGTH POTION\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
                break;
            case '4':
                if (player->gold<100)
                {
                    print_message_formatted("\n%s, YOU DO NOT HAVE ENOUGH GOLD TO BUY ARMOR!\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
                else
                {
                    buy_armor(player);
                }
                break;
            case '5':
                if (player->gold<1250)
                {
                    print_message_formatted("\n%s, YOU DO NOT HAVE ENOUGH GOLD TO BUY WEAPONS!\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
                else
                {
                    buy_weapon(player);
                }
                break;
            case '6':
                if (!player->lamp_flag && player->gold >= 1000) {
                    player->lamp_flag = 1;
                    player->gold -= 1000;
                    print_message_formatted("You bought a lamp!\n");
                } else if (player->lamp_flag) {
                    print_message_formatted("\n%s, You already have a lamp!\n", player->sex == MALE ? "SIR" : "MA'AM");
                    continue;
                } else {
                    print_message_formatted("\n%s, You don't have enough gold for a lamp.\n", player->sex == MALE ? "SIR" : "MA'AM");
                    continue;
                }
                break;
            case '7':
                buy_flares(player);
                break;
            case '8':
                if (player->blindness_flag && player->gold >= 1000) {
                    player->blindness_flag = 0;
                    player->gold -= 1000;
                    print_message("Your blindness has been cured!\n");
                } else if (!player->blindness_flag) {
                    print_message("You're not blind. No need for this service.\n");
                } else {
                    print_message_formatted("%s, You don't have enough gold to cure blindness.\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
                break;
            
            case '9':
                if (player->stickybook_flag && player->gold >= 1000) {
                    player->stickybook_flag = 0;
                    player->gold -= 1000;
                    print_message_formatted("The sticky book has been removed from your hand!\n");
                } else if (!player->stickybook_flag) {
                    print_message_formatted("%s, You don't have a sticky book. No need for this service.\n", player->sex == MALE ? "SIR" : "MA'AM");
                } else {
                    print_message_formatted("%s, You don't have enough gold to remove the sticky book.\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
                break;
            case '0':
                print_message_formatted("\nOK FINE, DON'T TRADE\n");
                return;

            default:
                print_message_formatted("*** %s, Invalid choice. ***\n", player->sex == MALE ? "SIR" : "MA'AM");
                continue;
        }

        //print_message_formatted("Purchase successful!\n");
    }

    print_message_formatted("You don't have enough gold for any more purchases.\n");
}

void attack_vendor(Player *player, GameState *game)
{
    print_message_formatted("You attack the vendor!\n");
    game->vendor_attacked = 1;
    fight_monster_normalize(player, game);
}

void buy_flares(Player *player)
{
    int max_flares = player->gold / 10;
    print_message_formatted("How many flares do you want to buy? (0-%d): ", max_flares);

    int flares_to_buy = get_user_input_number();

    if (flares_to_buy < 0 || flares_to_buy > max_flares) {
        print_message_formatted("Invalid number of flares.\n");
        return;
    }

    player->flares += flares_to_buy;
    player->gold -= flares_to_buy * 10;

    print_message("You bought %d flares for %d GP.\n", flares_to_buy, flares_to_buy * 10);
}

void buy_armor(Player *player)
{
    print_message("\n\nArmor Options:\n");
    print_message("    1. Leather (1250 GP)\n");
    print_message("    2. Chainmail (1500 GP)\n");
    print_message("    3. Plate (2000 GP)\n");
    print_message("    4. Repair Armor (100 GP)\n");
    print_message("    5. Nothing\n\n");

    print_message_formatted("Current armor is %s with armor points of %d\n\n", get_armor_name(player->armor_type), player->armor_points);

    char armor_choice = get_user_input();
    switch(armor_choice) {
        case '1':
            if (player->gold >= 1250) {
                if (player->armor_type < 1)
                {
                    player->armor_type = LEATHER;
                    player->armor_points = MAX_ARMOR_POINTS;
                    player->gold -= 1250;
                    print_message_formatted("\nLeather armor purchased successfully!\n");
                }
            } else {
                print_message("%s, not enough gold for Leather Armor.\n", player->sex == MALE ? "Sir" : "Ma'am");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                player->armor_type = CHAINMAIL;
                player->armor_points = MAX_ARMOR_POINTS;
                player->gold -= 1500;
                print_message("\nChainmail armor purchased successfully!\n");
            } else {
                print_message("%s, not enough gold for Chainmail.\n", player->sex == MALE ? "Sir" : "Ma'am");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                player->armor_type = PLATE;
                player->armor_points = MAX_ARMOR_POINTS;
                player->gold -= 2000;
                print_message_formatted("Plate armor purchased successfully!\n");
            } else {
                print_message("%s, not enough gold for Plate Armor.\n", player->sex == MALE ? "Sir" : "Ma'am");
                return;
            }
            break;
        case '4':
            if (player->gold >= 100 && player->armor_points<MAX_ARMOR_POINTS) {
                player->armor_points += random_number(3) + random_number(3); // 2 to 6
                if(player->armor_points>=MAX_ARMOR_POINTS) {
                    player->armor_points=MAX_ARMOR_POINTS;
                }   
                player->gold -= 100;
                print_message_formatted("Armor repaired.\n");
            } else {
                if(player->armor_points>=MAX_ARMOR_POINTS)
                {
                    print_message("%s, your armor does not need repaired\n", player->sex == MALE ? "Sir" : "Ma'am");
                
                }
                else {
                    print_message("%s, not enough gold for Plate Armor.\n", player->sex == MALE ? "Sir" : "Ma'am");
                }
                return;
            }
            break;
        case '5':
            return;
        default:
            print_message_formatted("%s, invalid choice. No armor purchased.\n", player->sex == MALE ? "Sir" : "Ma'am");
            return;
    }
}

void buy_weapon(Player *player)
{
    print_message("\nWeapon Options:\n");
    print_message("1. Dagger (1250 GP)\n");
    print_message("2. Mace (1500 GP)\n");
    print_message("3. Sword (2000 GP)\n");
    print_message("4. Nothing\n");

    char weapon_choice = get_user_input();
    switch(weapon_choice) {
        case '1':
            if (player->gold >= 1250) {
                if (player->weapon_type < 1)
                {
                    player->weapon_type = 1;
                    player->gold -= 1250;
                    print_message("Weapon purchased successfully!\n");
                }
                else
                {
                    print_message_formatted("%s, YOUR WEAPON IS ALREADY SUPERIOR!\n", player->sex == MALE ? "SIR" : "MA'AM");
                }
            } else {
                print_message_formatted("%s, Not enough gold for a Dagger.\n", player->sex == MALE ? "SIR" : "MA'AM");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                if (player->weapon_type <2)
                {
                    player->weapon_type = 2;
                    player->gold -= 1500;
                    print_message("Weapon purchased successfully!\n");

                }
                else
                {
                    print_message_formatted("%s, YOUR WEAPON IS ALREADY SUPERIOR!\n", player->sex == MALE ? "SIR" : "MA'AM");
                }                
            } else {
                print_message_formatted("%s, Not enough gold for a Mace.\n", player->sex == MALE ? "SIR" : "MA'AM");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                if (player->weapon_type < 3)
                {
                    player->weapon_type = 3;
                    player->gold -= 2000;
                    print_message("Weapon purchased successfully!\n");
                }
                else
                {
                    print_message_formatted("%s, YOUR WEAPON IS ALREADY SUPERIOR!\n", player->sex == MALE ? "SIR" : "MA'AM");                
                }
                
            } else {
                print_message_formatted("%s, Not enough gold for a Sword.\n", player->sex == MALE ? "SIR" : "MA'AM");
                return;
            }
            break;
        case '4':
            return;
        default:
            print_message_formatted("%s, Invalid choice. No weapon purchased.\n", player->sex == MALE ? "SIR" : "MA'AM");
            return;
    }
    
}
