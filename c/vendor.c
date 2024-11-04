#include "vendor.h"
#include "combat.h"
#include "player.h"
#include "utilities.h"
#include "wizardio.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void handle_vendor(Player *player, GameState *game) {
  print_message("\nYou've encountered a vendor!\n");

  // Check if the player has attacked the vendor before
  if (game->vendor_attacked) {
    fight_monster_normalize(player, game);
    return;
  }

  char choice;
  do {
    print_message(
        "\nDo you want to (T)rade, (A)ttack, or (I)gnore the vendor?\n");
    choice = get_user_input_custom_prompt("?  ");

    switch (choice) {
    case 'T':
      trade_with_vendor(player, game);
      return;
    case 'A':
      attack_vendor(player, game);
      return;
    case 'I':
      print_message("You ignore the vendor and move on.\n");
      return;
    default:
      print_message("Invalid choice. Please choose T, A, or I.\n");
    }
  } while (1);
}

// To do; make option choices more dynamic;  Like only offer stuff they can
// afford and put the numbers in order
void trade_with_vendor(Player *player, GameState *game) {
  // Offer to buy treasures
  int i = 0;
  // int optionchoice=7;
  for (i = 0; i < TREASURE_COUNT; i++) {
    if (game->treasure[i]) {
      int offer = random_number(1500) * (i + 1);
      print_message("Do you want to sell %s for %d GP? (Y/N) ",
                    get_treasure_name(i), offer);

      if (get_user_input_yn() == 'Y') {
        player->gold += offer;
        game->treasure[i]--;
        player->treasure_count--;
        print_message("Sold!\n");
      } else {
        if (random_number(8) == 1 && player->weapon_type < EXCALIBUR) {
          print_message("I'll give you an extremely rare offer; trade %s for "
                        "Excalibur? (Y/N) ",
                        get_treasure_name(i));
          if (get_user_input_yn() == 'Y') {
            game->treasure[i]--;
            player->treasure_count--;
            player->weapon_type = EXCALIBUR;
          }
        } else if (random_number(8) == 1 && player->armor_type < STONE) {
          print_message("I'll give you an extremely rare offer; trade %s for "
                        "Stone Armor? (Y/N) ",
                        get_treasure_name(i));
          if (get_user_input_yn() == 'Y') {
            game->treasure[i]--;
            player->treasure_count--;
            player->armor_type = STONE;
            player->armor_points = MAX_ARMOR_POINTS;
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

    int purchase_choice = get_user_input_number();
    switch (purchase_choice) {
    case 1:
      if (player->gold >= 500) {
        if (player->strength < MAX_STRENGTH) {
          print_message("\n%s, your strength has been greatly enhanced.\n",
                        player->sex == MALE ? "My liege" : "My lady");
          player->strength = get_minimum(player->strength + random_number(3) +
                                             random_number(3),
                                         MAX_STRENGTH);
          player->gold -= 500;
        } else {
          print_message("\n%s, your strength rivals that of the mightiest "
                        "heroes of legend.\n",
                        player->sex == MALE ? "My liege" : "My lady");
        }
      } else {
        print_message("\n%s you are too poor to buy an intelligence potion.\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
      break;
    case 2:
      if (player->gold >= 500) {
        if (player->intelligence < MAX_INTELLIGENCE) {
          print_message("\n%s, your intellect has been greatly enhanced.\n",
                        player->sex == MALE ? "My liege" : "My lady");
          player->intelligence = get_minimum(
              player->intelligence + random_number(3) + random_number(3),
              MAX_INTELLIGENCE);
          player->gold -= 500;
        } else {
          print_message("\n%s, your intellect surpasses even the greatest "
                        "minds of the realm.\n",
                        player->sex == MALE ? "My liege" : "My lady");
        }
      } else {
        print_message("\n%s, you are too poor to buy an intelligence potion.\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
      break;
    case 3:
      if (player->gold >= 500) {
        if (player->dexterity < MAX_DEXTERITY) {
          print_message("\n%s, your agility has been greatly enhanced.\n",
                        player->sex == MALE ? "My liege" : "My lady");
          player->dexterity = get_minimum(player->dexterity + random_number(3) +
                                              random_number(3),
                                          MAX_DEXTERITY);
          player->gold -= 500;
        } else {
          print_message(
              "\n%s, your agility rivals the swiftest rogues of the realm.\n",
              player->sex == MALE ? "My liege" : "My lady");
        }
      } else {
        print_message("\n%s, you are too poor to buy a dexterity potion.\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
      break;
    case 4:
      if (player->gold < 100) {
        print_message("\n%s, you are too poor to buy or repair armor.\n",
                      player->sex == MALE ? "My liege" : "My lady");
      } else {
        buy_armor(player);
      }
      break;
    case 5:
      if (player->gold < 1250) {
        print_message("\n%s, you do not have enough money to buy weapons!\n",
                      player->sex == MALE ? "My liege" : "My lady");
      } else {
        buy_weapon(player);
      }
      break;
    case 6:
      if (!player->lamp_flag && player->gold >= 1000) {
        player->lamp_flag = 1;
        player->gold -= 1000;
        print_message("You bought a lamp!\n");
      } else if (player->lamp_flag) {
        print_message("\n%s, you already have a lamp!\n",
                      player->sex == MALE ? "My liege" : "My lady");
        continue;
      } else {
        print_message("\n%s, you don't have enough gold for a lamp.\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
      break;
    case 7:
      buy_flares(player);
      break;
    case 8:
      if (player->blindness_flag && player->gold >= 1000) {
        player->blindness_flag = 0;
        player->gold -= 1000;
        print_message("Your blindness has been cured!\n");
      } else if (!player->blindness_flag) {
        print_message("You're not blind. No need for this service.\n");
      } else {
        print_message("%s, you don't have enough gold to cure blindness.\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
      break;

    case 9:
      if (player->stickybook_flag && player->gold >= 1000) {
        player->stickybook_flag = 0;
        player->gold -= 1000;
        print_message("The sticky book has been removed from your hand!\n");
      } else if (!player->stickybook_flag) {
        print_message(
            "%s, you don't have a sticky book. No need for this service.\n",
            player->sex == MALE ? "My liege" : "My lady");
      } else {
        print_message(
            "%s, you don't have enough gold to remove the sticky book.\n",
            player->sex == MALE ? "My liege" : "My lady");
      }
      break;
    case 0:
      print_message("\nOk, fine, don't trade.\n");
      return;

    default:
      print_message("*** %s, invalid choice. ***\n",
                    player->sex == MALE ? "My liege" : "My lady");
      continue;
    }
  }

  print_message("You don't have enough gold for any more purchases.\n");
}

void attack_vendor(Player *player, GameState *game) {
  print_message("You dare to strike the vendor!\n");
  game->vendor_attacked = 1;
  fight_monster_normalize(player, game);
}

void buy_flares(Player *player) {
  int max_flares = player->gold / 10;
  print_message("How many flares do you want to buy? (0-%d): ", max_flares);

  int flares_to_buy = get_user_input_number();

  if (flares_to_buy < 0 || flares_to_buy > max_flares) {
    print_message("Invalid number of flares.\n");
    return;
  }

  player->flares += flares_to_buy;
  player->gold -= flares_to_buy * 10;

  print_message("You bought %d flares for %d GP.\n", flares_to_buy,
                flares_to_buy * 10);
}

void buy_armor(Player *player) {
  print_message("\n\nArmor Options:\n");
  print_message("    1. Leather (1250 GP)\n");
  print_message("    2. Chainmail (1500 GP)\n");
  print_message("    3. Plate (2000 GP)\n");
  print_message("    4. Repair Armor (100 GP)\n");
  print_message("    5. Nothing\n\n");

  print_message("Current armor is %s with armor points of %d\n\n",
                get_armor_name(player->armor_type), player->armor_points);

  char armor_choice = get_user_input();
  switch (armor_choice) {
  case '1':
    if (player->gold >= 1250) {
      if (player->armor_type < 1) {
        player->armor_type = LEATHER;
        player->armor_points = MAX_ARMOR_POINTS;
        player->gold -= 1250;
        print_message("\nLeather armor purchased successfully!\n");
      }
    } else {
      print_message("%s, not enough gold for Leather Armor.\n",
                    player->sex == MALE ? "My liege" : "My lady");
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
      print_message("%s, not enough gold for Chainmail.\n",
                    player->sex == MALE ? "My liege" : "My lady");
      return;
    }
    break;
  case '3':
    if (player->gold >= 2000) {
      player->armor_type = PLATE;
      player->armor_points = MAX_ARMOR_POINTS;
      player->gold -= 2000;
      print_message("Plate armor purchased successfully!\n");
    } else {
      print_message("%s, not enough gold for Plate Armor.\n",
                    player->sex == MALE ? "My liege" : "My lady");
      return;
    }
    break;
  case '4':
    if (player->gold >= 100 && player->armor_points < MAX_ARMOR_POINTS) {
      player->armor_points += random_number(3) + random_number(3); // 2 to 6
      if (player->armor_points >= MAX_ARMOR_POINTS) {
        player->armor_points = MAX_ARMOR_POINTS;
      }
      player->gold -= 100;
      print_message("Armor repaired.\n");
    } else {
      if (player->armor_points >= MAX_ARMOR_POINTS) {
        print_message("%s, your armor does not need repaired\n",
                      player->sex == MALE ? "My liege" : "My lady");

      } else {
        print_message("%s, not enough gold to repair armor.\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
      return;
    }
    break;
  case '5':
    return;
  default:
    print_message("%s, invalid choice. No armor purchased.\n",
                  player->sex == MALE ? "My liege" : "My lady");
    return;
  }
}

void buy_weapon(Player *player) {
  print_message("\nWeapon Options:\n");
  print_message("1. Dagger (1250 GP)\n");
  print_message("2. Mace (1500 GP)\n");
  print_message("3. Sword (2000 GP)\n");
  print_message("4. Nothing\n");

  char weapon_choice = get_user_input();
  switch (weapon_choice) {
  case '1':
    if (player->gold >= 1250) {
      if (player->weapon_type < DAGGER) {
        player->weapon_type = DAGGER;
        player->gold -= 1250;
        print_message("Weapon purchased successfully!\n");
      } else {
        print_message("%s, your weapon is already superior!\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
    } else {
      print_message("%s, not enough gold for a dagger.\n",
                    player->sex == MALE ? "My liege" : "My lady");
      return;
    }
    break;
  case '2':
    if (player->gold >= 1500) {
      if (player->weapon_type < MACE) {
        player->weapon_type = MACE;
        player->gold -= 1500;
        print_message("Weapon purchased successfully!\n");

      } else {
        print_message("%s, your weapon is already superior!\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }
    } else {
      print_message("%s, not enough gold for a mace.\n",
                    player->sex == MALE ? "My liege" : "My lady");
      return;
    }
    break;
  case '3':
    if (player->gold >= 2000) {
      if (player->weapon_type < SWORD) {
        player->weapon_type = SWORD;
        player->gold -= 2000;
        print_message("Weapon purchased successfully!\n");
      } else {
        print_message("%s, your weapon is already superior!\n",
                      player->sex == MALE ? "My liege" : "My lady");
      }

    } else {
      print_message("%s, , you do not have enough gold for a sword.\n",
                    player->sex == MALE ? "My liege" : "My lady");
      return;
    }
    break;
  case '4':
    return;
  default:
    print_message("%s, invalid choice. No weapon purchased.\n",
                  player->sex == MALE ? "My liege" : "My lady");
    return;
  }
}
