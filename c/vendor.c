#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "vendor.h"
#include "utilities.h"
#include "combat.h"
#include "wizardio.h"

void handle_vendor(Player *player, GameState *game)
{
    print_message_formatted("\nYou've encountered a vendor!\n");
    
    // Check if the player has attacked the vendor before
    if (game->vendor_attacked) {
        fight_monster(player, game);
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

void trade_with_vendor(Player *player, GameState *game)
{
    // Offer to buy treasures
    for (int i = 0; i < TREASURE_COUNT; i++) {
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
               if (random_number(8)==1 && player->weapon_type<4)
               {
                   print_message("I'll give you an extremely rare offer; sell %s for Excalibur? (Y/N) ", get_treasure_name(i), offer);
                   if (get_user_input_yn() == 'Y') {
                       game->treasure[i] = 0;
                       player->treasure_count--;
                       player->weapon_type=4;
                   }        
               }
               else
                   if (random_number(8)==1 && player->armor_type<4)
                   {
                       print_message("I'll give you an extremely rare offer; sell %s for Stone Armor? (Y/N) ", get_treasure_name(i), offer);
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
        print_message_formatted("\n\nWhat would you like to buy?\n");
        print_message("    1. Improve Strength (1000 GP)\n");
        print_message("    2. Improve Intelligence (1000 GP)\n");
        print_message("    3. Improve Dexterity (1000 GP)\n");
        print_message("    4. Armor (1250-2000 GP)\n");
        print_message("    5. Weapon (1250-2000 GP)\n");
        print_message("    6. Lamp (1000 GP)\n");
        print_message("    7. Flares (10 GP)\n");
        print_message("    8. Nothing more\n\n");

        char purchase_choice = get_user_input();
        switch(purchase_choice) {
            case '1':
                if(player->gold>=1000)
		{
                    if (player->strength < 18)
                    {
                        player->strength = min(player->strength + random_number(6), 18);
                        player->gold -= 1000;
                    }
                    else
                    {
                        print_message_formatted("\nYOU ARE ALREADY HERCULES\n");
                    }
                }
                else
                {
                    print_message_formatted("\nYOU ARE TOO POOR TO BUY A STRENGTH POTION\n");
                }
                break;
            case '2':
                if(player->gold>=1000)
		{
                    if (player->intelligence < 18)
                    {
                        player->intelligence = min(player->strength + random_number(6), 18);
                        player->gold -= 1000;
                    }
                    else
                    {
                        print_message_formatted("\nYOU ARE ALREADY A GENIUS ... GENIUS\n");
                    }
                }
                else
                {
                    print_message_formatted("\nYOU ARE TOO POOR TO BUY A STRENGTH POTION\n");
                }
                break;
            case '3':
                if(player->gold>=1000)
		{
                    if (player->dexterity < 18)
                    {
                        player->dexterity = min(player->strength + random_number(6), 18);
                        player->gold -= 1000;
                    }
                    else
                    {
                        print_message_formatted("\nYOU ARE ALREADY NIMBLE ... GENIUS\n");
                    }
                }
                else
                {
                    print_message_formatted("\nYOU ARE TOO POOR TO BUY A STRENGTH POTION\n");
                }
                break;
            case '4':
                if (player->gold<1250)
                {
                    print_message_formatted("\nYOU DO NOT HAVE ENOUGH GOLD TO BUY ARMOR!\n");
                }
                else
                {
                    buy_armor(player);
                }
                break;
            case '5':
                if (player->gold<1250)
                {
                    print_message_formatted("\nYOU DO NOT HAVE ENOUGH GOLD TO BUY WEAPONS!\n");
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
                    print_message_formatted("\nYou already have a lamp!\n");
                    continue;
                } else {
                    print_message_formatted("\nYou don't have enough gold for a lamp.\n");
                    continue;
                }
                break;
            case '7':
                buy_flares(player);
                break;
            case '8':
                print_message_formatted("\nOK FINE, DON'T TRADE\n");
                return;
            default:
                print_message_formatted("Invalid choice.\n");
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

    // Call fight_monster to handle the combat
    fight_monster(player, game);
}

void buy_flares(Player *player)
{
    int max_flares = player->gold / 10;
    char message[100];
    snprintf(message, sizeof(message), "How many flares do you want to buy? (0-%d): ", max_flares);
    print_message_formatted(message);

    int flares_to_buy = get_user_input_number();

    if (flares_to_buy < 0 || flares_to_buy > max_flares) {
        print_message_formatted("Invalid number of flares.\n");
        return;
    }

    player->flares += flares_to_buy;
    player->gold -= flares_to_buy * 10;

    snprintf(message, sizeof(message), "You bought %d flares for %d GP.\n", flares_to_buy, flares_to_buy * 10);
    print_message(message);
}

void buy_armor(Player *player)
{
    print_message("\n\nArmor Options:\n");
    print_message("    1. Leather (1250 GP)\n");
    print_message("    2. Chainmail (1500 GP)\n");
    print_message("    3. Plate (2000 GP)\n");
    print_message("    4. Nothing\n\n");

    char armor_choice = get_user_input();
    switch(armor_choice) {
        case '1':
            if (player->gold >= 1250) {
                if (player->armor_type < 1)
                {
                    player->armor_type = 1;
                    player->armor_points = 7;
                    player->gold -= 1250;
                    print_message_formatted("\nArmor purchased successfully!\n");
                }
                else
                {

                    print_message_formatted("\nYOUR ARMOR IS ALREADY SUPERIOR BUT I WILL REPAIR YOUR ARMOR!!!   ON THE HOUSE\n\n");
                    player->armor_points = player->armor_type*7;
                }
            } else {
                print_message("Not enough gold for Leather Armor.\n");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                if(player->armor_type < 2)
                {
                    player->armor_type = 2;
                    player->armor_points = 14;
                    player->gold -= 1500;
                    print_message("\nArmor purchased successfully!\n");

                }
                else
                {
                    print_message("\nYOUR ARMOR IS ALREADY SUPERIOR BUT I WILL REPAIR YOUR ARMOR!!!   ON THE HOUSE\n\n");
                    player->armor_points = player->armor_type*7;
                }
            } else {
                print_message_formatted("Not enough gold for Chainmail.\n");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                if (player->armor_type<3)
                {
                    player->armor_type = 3;
                    player->armor_points = 21;
                    player->gold -= 2000;
                    print_message_formatted("Armor purchased successfully!\n");

                }
                else
                {
                    print_message_formatted("YOUR ARMOR IS ALREADY SUPERIOR BUT I WILL REPAIR YOUR ARMOR!!!   ON THE HOUSE\n\n");
                    player->armor_points = player->armor_type*2;
                    if(player->armor_points>50)
                    {                   
                         player->armor_points=50;
                    }
                }
                
            } else {
                print_message("Not enough gold for Plate Armor.\n");
                return;
            }
            break;
        case '4':
            return;
        default:
            print_message_formatted("Invalid choice. No armor purchased.\n");
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
                    print_message_formatted("Weapon purchased successfully!\n");
                }
                else
                {
                    print_message_formatted("YOUR WEAPON IS ALREADY SUPERIOR!\n");
                }
            } else {
                print_message_formatted("Not enough gold for a Dagger.\n");
                return;
            }
            break;
        case '2':
            if (player->gold >= 1500) {
                if (player->weapon_type <2)
                {
                    player->weapon_type = 2;
                    player->gold -= 1500;
                    print_message_formatted("Weapon purchased successfully!\n");

                }
                else
                {
                    print_message_formatted("YOUR WEAPON IS ALREADY SUPERIOR!\n");
                }                
            } else {
                print_message_formatted("Not enough gold for a Mace.\n");
                return;
            }
            break;
        case '3':
            if (player->gold >= 2000) {
                if (player->weapon_type < 3)
                {
                    player->weapon_type = 3;
                    player->gold -= 2000;
                    print_message_formatted("Weapon purchased successfully!\n");
                }
                else
                {
                    print_message_formatted("YOUR WEAPON IS ALREADY SUPERIOR!\n");                
                }
                
            } else {
                print_message_formatted("Not enough gold for a Sword.\n");
                return;
            }
            break;
        case '4':
            return;
        default:
            print_message_formatted("Invalid choice. No weapon purchased.\n");
            return;
    }
    
}
