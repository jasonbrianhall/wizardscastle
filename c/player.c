#include "player.h"
#include "combat.h"
#include "utilities.h"
#include "wizardio.h"
#include "wizards-castle.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize_player(Player *player) {
  // Set initial attributes
  player->race = HOBBIT;
  player->sex = FEMALE;
  player->strength = 2;
  player->intelligence = 2;
  player->dexterity = 2;
  player->gold = 60;
  player->flares = 0;
  player->armor_type = 0;
  player->armor_points = 0;
  player->weapon_type = 0;
  player->lamp_flag = 0;

  // Set initial position
  player->x = 1;
  player->y = 4;
  player->level = 1;

  // Set initial flags
  player->runestaff_flag = 0;
  player->orb_flag = 0;

  // Additional variables from BASIC that might be needed
  player->book_flag = 0;
  player->blindness_flag = 0;
  player->stickybook_flag = 0;
  player->has_runestaff = 0;
  player->has_orb = 0;
  player->treasure_count = 0;
  player->web_count = 0;
}

// Player creation and attribute functions
void choose_race(Player *player) {
  const char *races[] = {"Hobbit", "Elf", "Human", "Dwarf", "Drow"};
  char user_input;
  int valid_choice = 0;

  player->strength = 2;
  player->intelligence = 8;
  player->dexterity = 14;

  do {
    print_message("All right, bold one.\n");
    print_message("You may be an (E)lf, (D)warf, (M)an, (H)obbit, or D(R)ow "
                  "(Dark Elf).\n\n");
    user_input = get_user_input_custom_prompt("Your choice:  ");

    player->race = 0;

    switch (user_input) {
    case 'H':
      player->race = 1; // Hobbit
      break;
    case 'E':
      player->race = 2; // Elf
      break;
    case 'M':
      player->race = 3; // Man/Human
      break;
    case 'D':
      player->race = 4; // Dwarf
      break;
    case 'R':
      player->race = 5; // Dark Elf
      break;

    default:
      // Handle invalid input if needed
      break;
    }

    if (player->race > 0) {
      player->strength += 2 * player->race;
      player->dexterity -= 2 * player->race;
      if (player->race == 2 || player->race == 5) { // Elf gets extra points
        player->intelligence += 4;
      }
      if (player->race == DROW) // Give Dark Elves 4 extra dexterity
      {
        player->dexterity += 4;
      }
      print_message("You have chosen to be a ");
      print_message("%s\n", races[player->race - 1]);
      valid_choice = 1;
    } else {
      print_message("\n** That was incorrect. Please type E, D, M, OR H.\n");
    }
  } while (!valid_choice);
}

void choose_sex(Player *player) {
  char user_input;
  int valid_choice = 0;
  const char *race_names[] = {"Hobbit", "Elf", "Human", "Dwarf", "Drow"};

  do {
    print_message("Which sex do you prefer?\n\n");
    user_input = get_user_input_custom_prompt("Your choice:  ");

    switch (user_input) {
    case 'M':
      player->sex = 1;
      valid_choice = 1;
      break;
    case 'F':
      player->sex = 0;
      valid_choice = 1;
      break;
    default: {
      print_message("** Cute ");
      print_message("%s, real cute. Try ",
                              race_names[player->race - 1]);
      print_message("M or F.\n");
    } break;
    }
  } while (!valid_choice);

  print_message("\n");
}

void allocate_attributes(Player *player) {
  int other_points;
  int points_to_add;
  const char *race_names[] = {"Hobbit", "Elf", "Human", "Dwarf", "Drow"};

  // Set other_points based on race
  if (player->race == 1) { // Hobbit
    other_points = 4;
  } else { // Elf, Human, Dwarf
    other_points = 8;
  }

  print_message("Ok, ");
  print_message("%s, YOU HAVE THE FOLLOWING ATTRIBUTES:\n",
                          race_names[player->race - 1]);
  print_message("Strength = %d    Intelligence = %d    Dexterity = %d\n",
                player->strength, player->intelligence, player->dexterity);
  print_message("AND %d OTHER POINTS TO ALLOCATE AS YOU WISH.\n\n",
                          other_points);

  // Allocate points to Strength
  while (other_points > 0) {
    print_message("How many points do you wish to add to your strength (you have %d points)?\n\n", other_points);
    points_to_add = get_user_input_number();

    if (points_to_add >= 0 && points_to_add <= other_points) {
      player->strength += points_to_add;
      other_points -= points_to_add;
      break;
    } else {
      print_message("\n** Invalid input.  You have %d points to allocate.\n\n", other_points);
    }
  }

  if (player->strength > MAX_STRENGTH) {
    other_points += player->strength - MAX_STRENGTH;
    player->strength = MAX_STRENGTH;
  }

  // Allocate points to Intelligence
  while (other_points > 0) {
    print_message("How many points do you wish to allocate to your intelligence (you have %d points)?\n\n", other_points);
    points_to_add = get_user_input_number();
    if (points_to_add >= 0 && points_to_add <= other_points) {
      player->intelligence += points_to_add;
      other_points -= points_to_add;
      break;
    } else {
      print_message(
          "\n** Invalid choice. You have %d points to allocate.\n\n",
          other_points);
    }
  }

  if (player->intelligence > MAX_INTELLIGENCE) {
    other_points += player->intelligence - MAX_INTELLIGENCE;
    player->intelligence = MAX_INTELLIGENCE;
  }

  // Allocate remaining points to Dexterity
  if (other_points > 0) {
    print_message("Allocating remaining %d points to dexterity.\n",
                            other_points);
    player->dexterity += other_points;
    if (player->dexterity >= MAX_DEXTERITY) {
      player->dexterity = MAX_DEXTERITY;
    }
  }

  print_message("\nYour attributes are now:\n");
  print_message("Strength = %d    Intelligence = %d    Dexterity = %d\n",
                player->strength, player->intelligence, player->dexterity);
}

// Item and treasure functions
void buy_equipment(Player *player) {
  char user_input;
  const char *race_names[] = {"Hobbit", "Elf", "Human", "Dwarf", "Drow"};
  const char *armor_types[] = {"No Armor", "Leather", "Chainmail", "Plate"};
  const char *weapon_types[] = {"No Weapon", "Dagger", "Mace", "Sword"};
  int cost, exittheloop;

  // Buy Armor
  print_message("\nOK, %s", (race_names[player->race - 1]));

  print_message(", you have %d gold pieces (GP'S).\n\n",
                          player->gold);

  print_message("These are the types of armor you can buy:\n");
  print_message("(P)late<30> (C)hainmail<20> (L)eather<10> (N)othing<0>\n\n");

  player->armor_points = 0;
  do {
    user_input = get_user_input_custom_prompt("Your choice:  ");
    exittheloop = 1;
    switch (user_input) {
    case 'P':
      player->armor_type = PLATE;
      cost = 30;
      player->armor_points = MAX_ARMOR_POINTS;
      break;
    case 'C':
      player->armor_type = CHAINMAIL;
      cost = 20;
      player->armor_points = MAX_ARMOR_POINTS;
      break;
    case 'L':
      player->armor_type = LEATHER;
      cost = 10;
      player->armor_points = MAX_ARMOR_POINTS;
      break;
    case 'N':
      player->armor_type = 0;
      cost = 0;
      player->armor_points = 0;
      break;
    default:
      print_message("\n** Are you a ");
      print_message("%s or a fool?  Try again.\n\n",
                              race_names[player->race - 1]);
      exittheloop = 0;
      continue;
    }
  } while (exittheloop == 0);

  player->gold -= cost;

  // Buy Weapon
  print_message("\nOk, %s, you have %d GP's left.\n\n",
                          race_names[player->race - 1], player->gold);
  print_message("These are the type of weapons you can buy:\n");
  print_message("(S)word<30> (M)ace<20> (D)agger<10> (N)othing<0>\n\n");

  do {
    user_input = get_user_input_custom_prompt("Your choice:  ");
    exittheloop = 1;
    switch (user_input) {
    case 'S':
      if (player->gold < 30) {
        print_message(
            "** Your dungeon express card - you left home without it!\n\n");
        continue;
      }
      player->weapon_type = SWORD;
      cost = 30;
      break;
    case 'M':
      if (player->gold < 20) {
        print_message("** Sorry %s, I'm afraid I don't give credit!!\n\n",
                      player->sex == MALE ? "sir" : "ma'am");
        continue;
      }
      player->weapon_type = MACE;
      cost = 20;
      break;
    case 'D':
      player->weapon_type = DAGGER;
      cost = 10;
      break;
    case 'N':
      player->weapon_type = NOTHING;
      cost = 0;
      break;
    default:
      print_message("** Poor ");
      print_message("%s ", race_names[player->race - 1]);
      print_message(", try again, your choice must be S, M, D, or N.\n\n");
      exittheloop = 0;
    }
  } while (exittheloop == 0);

  player->gold -= cost;

  print_message("\nYou now have %s armor and a %s.\n",
                          armor_types[player->armor_type],
                          weapon_types[player->weapon_type]);
  print_message("You have %d gold pieces left.\n", player->gold);
}

void buy_lamp_and_flares(Player *player) {
  char user_input_yn;
  const char *race_names[] = {"Hobbit", "Elf", "Human", "Dwarf", "Drow"};

  // Try to buy a lamp
  if (player->gold >= 20 && !player->lamp_flag) {
    print_message("\nDo you want to buy a lamp for 20 GP'S?\n");
    do {
      print_message("Your choice (Y/N):  ");
      user_input_yn = get_user_input_yn();
      if (user_input_yn == 'Y') {
        player->lamp_flag = 1;
        player->gold -= 20;
        print_message(
            "\nOk, lamp purchased. It's guaranteed to outlive you!\n");
        break;
      } else if (user_input_yn == 'N') {
        break;
      } else {
        print_message("** %s, Please answer Yes or No.\n",
                      player->sex == MALE ? "Sir" : "Ma'am");
      }
    } while (1);
  }

  // Try to buy flares
  if (player->gold >= 1) {
    print_message("\nOk, %s, you have %d gold pieces left.\n",
                            race_names[player->race - 1], player->gold);
    print_message("Flares cost 1 GP each. How many do you want? ");

    int flares_to_buy;
    do {
      flares_to_buy = get_user_input_number();

      // Check if the input is "0" to explicitly buy no flares
      if (flares_to_buy == 0) {
        print_message("You chose not to buy any flares.\n");
        return;
      } else if (flares_to_buy < 0) {
        print_message("** %s, PLEASE ENTER A POSITIVE NUMBER.\n",
                      player->sex == MALE ? "Sir" : "Ma'am");
      } else if (flares_to_buy > player->gold) {
        print_message(
            "** You can only afford %d. Please enter a lower number.\n",
            player->gold);
      } else {
        player->flares += flares_to_buy;
        player->gold -= flares_to_buy;
        print_message("\nOk, you now have %d flares and %d gold pieces left.\n",
                      player->flares, player->gold);
        return;
      }
      print_message("How many flares do you want? (or enter 0 to buy none):  ");
    } while (1);
  }
}

const char *get_race_name(int race) {
  switch (race) {
  case 1:
    return "HOBBIT";
  case 2:
    return "ELF";
  case 3:
    return "HUMAN";
  case 4:
    return "DWARF";
  case 5:
    return "DARK ELF";
  default:
    return "UNKNOWN";
  }
}

const char *get_race_name_flc(int race) {
  switch (race) {
  case 1:
    return "Hobbit";
  case 2:
    return "Elf";
  case 3:
    return "Human";
  case 4:
    return "Dwarf";
  case 5:
    return "Dark Elf";
  default:
    return "UNKNOWN";
  }
}


void print_status(Player *player, GameState *game) {
  // char message[256];  // Buffer for formatting messages
  int treasurecount = 0, i;

  print_message("\n=== PLAYER STATUS ===\n");
  // Print player race and attributes
  print_message("Race: %s", get_race_name_flc(player->race));
  print_message("   Sex: ");
  if (player->sex == FEMALE) {
    print_message("Female");
  } else {
    print_message("Male");
  }
  print_message("\n");
  print_message("Strength: %d  Intelligence: %d  Dexterity: %d\n",
                player->strength, player->intelligence, player->dexterity);

  // Print player position
  print_message("Location: Level %d, Room (%d, %d)\n", player->level, player->x,
                player->y);

  // Print player inventory
  print_message("Gold Pieces: %d  Flares: %d\n", player->gold, player->flares);

  // Print armor and weapon
  const char *armor_types[] = {"No Armor", "Leather", "Chainmail", "Plate",
                               "Stone"};
  const char *weapon_types[] = {"No Weapon", "Dagger", "Mace", "Sword",
                                "Excalibur"};
  print_message("Armor: %s  Weapon: %s\n", armor_types[player->armor_type],
                weapon_types[player->weapon_type]);
  // Print special items

  print_message("Special Items: ");
  if (player->lamp_flag)
    print_message("Lamp ");
  if (player->runestaff_flag)
    print_message("Runestaff ");
  if (player->orb_flag)
    print_message("Orb of Zot ");
  print_message("\n");

  // Print curses or blessings
  print_message("Status Effects: ");
  if (player->blindness_flag)
    print_message("Blind ");
  if (player->stickybook_flag)
    print_message("Sticky Book ");
  print_message("\n");

  print_message("Armor points: %d\n", player->armor_points);

  // Print number of treasures
  for (i = 0; i < TREASURE_END - TREASURE_START + 1; i++) {
    if (game->treasure[i] >= 1) {
      treasurecount++;
    }
  }

  print_message("Treasures Found: %d\n", treasurecount);

  for (int i = 0; i < TREASURE_END - TREASURE_START + 1; i++) {
    if (game->treasure[i] >= 1) {
      print_message("     ");
      print_message_formatted(get_treasure_name(i));
      if (game->treasure[i] > 1) {
        print_message(" * %d", game->treasure[i]);
      }
      print_message("\n");
    }
  }
  print_message("Turn count: %d\n", game->turn_count);
  print_message("======================\n\n");
}

void fight_monster_normalize(Player *player, GameState *game) {
  player->temp_blindness_flag = 0;
  fight_monster(player, game);
  if (!game->game_over) {
    if (player->temp_intelligence > 0 &&
        player->intelligence > player->temp_intelligence) {
      player->intelligence = player->temp_intelligence;
      player->temp_intelligence = 0;
    }
    if (player->temp_dexterity > 0 &&
        player->dexterity > player->temp_dexterity) {
      player->dexterity = player->temp_dexterity;
      player->temp_dexterity = 0;
    }
    if (player->strength > MAX_STRENGTH) {
      player->strength = MAX_STRENGTH;
    }
    if (player->temp_blindness_flag == 1) {
      print_message("Your vision has returned.\n");
      player->blindness_flag = 0;
    }
  }
}
