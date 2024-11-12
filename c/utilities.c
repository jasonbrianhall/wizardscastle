#include "utilities.h"
#include "combat.h"
#include "gamestate.h"
#include "player.h"
#include "wizardio.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void capitalize_first(char *str) {
    if (!str || !*str) return;
    
    // Capitalize first letter
    str[0] = toupper(str[0]);
    
    // Make rest lowercase
    for (int i = 1; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}


const char *get_random_body_part(void) {
  const char *body_parts[] = {" SANDWICH", " STEW",  " SOUP", " BURGER",
                              " ROAST",    " FILET", " TACO", " PIE"};
  int num_parts = sizeof(body_parts) / sizeof(body_parts[0]);

  return body_parts[random_number(num_parts) - 1];
}

int is_vowel(char c) {
  c = tolower(c); // Convert to lowercase for checking
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

void to_lowercase(char *dest, const char *src) {
  int i = 0;
  while (src[i]) {
    dest[i] = tolower(src[i]);
    i++;
  }
  dest[i] = '\0'; // Null terminate the string
}

const char *get_random_species(void) {
  const char *species[] = {"ELF", "HUMAN", "DWARF", "HOBBIT", "DROW"};
  int num_species = sizeof(species) / sizeof(species[0]);
  return species[random_number(num_species) - 1];
}

// Utility functions
int random_number(int max_value) { return 1 + rand() % max_value; }

// Movement and action functions
void move_player(Player *player, GameState *game, char direction) {
  int current_room =
      get_room_content(game, player->x, player->y, player->level);
  int printstatusmessage = 1;

  // If I cast web in a previous turn, webcount should be zero again
  player->web_count = 0;

  // Check if player is at the entrance and moving north
  if (current_room == 102 && direction == 'N') {
    if (player->orb_flag) {
      print_message(
          "Congratulations! You've escaped the castle with the Orb of Zot!\n");
      // Set a flag or call a function to end the game with victory
      game->game_over = 1;
      game->victory = 1;
      return;
    } else {
      print_message("You're at the entrance. Are you sure you want "
                    "to leave without the Orb of Zot? (Y/N) ");
      char choice = get_user_input_yn();
      if (choice == 'Y') {
        print_message("You leave the castle empty-handed. Game over!\n");
        // Set a flag or call a function to end the game
        game->game_over = 1;
        return;
      } else {
        print_message("You decide to stay in the castle.\n");
        return;
      }
    }
  }
  int new_x = player->x;
  int new_y = player->y;
  switch (direction) {
  case 'N':
    new_x--;
    break;
  case 'S':
    new_x++;
    break;
  case 'W':
    new_y--;
    break;
  case 'E':
    new_y++;
    break;
  case 'T': // teleport
    printstatusmessage = 0;
    break;
  default:
    print_message("Invalid direction!\n");
    return;
  }

  // Handle wrapping around the castle edges
  new_x = (new_x - 1 + CASTLE_SIZE) % CASTLE_SIZE + 1; // Wrap x between 1 and 8
  new_y = (new_y - 1 + CASTLE_SIZE) % CASTLE_SIZE + 1; // Wrap y between 1 and 8

  // Update player position
  player->x = new_x;
  player->y = new_y;

  // Print movement message
  if (printstatusmessage == 1) {
    print_message("You move %s to (%d, %d) on level %d.\n",
                  direction == 'N'   ? "North"
                  : direction == 'S' ? "South"
                  : direction == 'W' ? "West"
                                     : "East",
                  player->y, player->x, player->level);
  }
  // Check for special room events (like warp or sinkhole)
  int room_content =
      get_room_content(game, player->x, player->y, player->level);
  mark_room_discovered(game, player->x, player->y, player->level);

  if (room_content == WARP) { // Warp
    player->x = random_number(CASTLE_SIZE);
    player->y = random_number(CASTLE_SIZE);
    player->level = random_number(CASTLE_SIZE);
    print_message("You've been warped to a random location!\n");
  } else if (room_content == SINKHOLE) { // Sinkhole
    if (player->level < 8) {
      player->level++;
    } else {
      player->level = 1;
    }
    print_message("You've fallen through a sinkhole to the level below!\n");
  }

  // Print the new room description
}

void move_player_randomly(Player *player, GameState *game) {
  char directions[] = {'N', 'S', 'E', 'W'};
  char direction = directions[random_number(4) - 1];
  move_player(player, game, direction);
}

// Helper function for minimum of two integers
int get_minimum(int a, int b) {
  if (a < b)
    return a;
  return b;
}
void printStars(void) {
  print_message(
      "****************************************************************\n");
}

const char *get_weapon_name(int weapon_type) {
  const char *weapon_names[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD",
                                "EXCALIBUR"};

  if (weapon_type < 5) {
    return weapon_names[weapon_type];

  } else {
    return weapon_names[0];
  }
}

const char *get_armor_name(int armor_type) {
  const char *armor_names[] = {"NO ARMOR", "LEATHER", "CHAINMAIL", "PLATE",
                               "STONE"};

  if (armor_type < 5) {
    return armor_names[armor_type];

  } else {
    return armor_names[0];
  }
}

// Helper function to get treasure names
const char *get_treasure_name(int index) {
  static const char *treasure_names[] = {"Ruby Red", "Norn Stone", "Pale Pearl",
                                         "Opal Eye", "Green Gem",  "Blue Flame",
                                         "Palantir", "Silmaril"};
  return treasure_names[index];
}

// New helper function to get abbreviated room descriptions
void get_room_description(int room_content, char *desc) {
  char full_desc[10];
  switch (room_content) {
  case EMPTY_ROOM:
    strncpy(full_desc, "EMPTY   \0", 9);
    break;
  case ENTRANCE:
    strncpy(full_desc, "ENTRANCE\0", 9);
    break;
  case STAIRS_UP:
    strncpy(full_desc, "STAIRS U\0", 9);
    break;
  case STAIRS_DOWN:
    strncpy(full_desc, "STAIRS D\0", 9);
    break;
  case POOL:
    strncpy(full_desc, "POOL    \0", 9);
    break;
  case CHEST:
    strncpy(full_desc, "CHEST   \0", 9);
    break;
  case GOLD:
    strncpy(full_desc, "GOLD    \0", 9);
    break;
  case FLARES:
    strncpy(full_desc, "FLARES  \0", 9);
    break;
  case WARP:
    strncpy(full_desc, "WARP    \0", 9);
    break;
  case SINKHOLE:
    strncpy(full_desc, "SINKHOLE\0", 9);
    break;
  case CRYSTAL_ORB:
    strncpy(full_desc, "CRYSTAL \0", 9);
    break;
  case BOOK:
    strncpy(full_desc, "BOOK    \0", 9);
    break;
  case KOBOLD:
    strncpy(full_desc, "KOBOLD  \0", 9);
    break;
  case ORC:
    strncpy(full_desc, "ORC     \0", 9);
    break;
  case WOLF:
    strncpy(full_desc, "WOLF    \0", 9);
    break;
  case GOBLIN:
    strncpy(full_desc, "GOBLIN  \0", 9);
    break;
  case OGRE:
    strncpy(full_desc, "OGRE    \0", 9);
    break;
  case TROLL:
    strncpy(full_desc, "TROLL   \0", 9);
    break;
  case BEAR:
    strncpy(full_desc, "BEAR    \0", 9);
    break;
  case MINOTAUR:
    strncpy(full_desc, "MINOTAUR\0", 9);
    break;
  case GARGOYLE:
    strncpy(full_desc, "GARGOYLE\0", 9);
    break;
  case CHIMERA:
    strncpy(full_desc, "CHIMERA \0", 9);
    break;
  case BALROG:
    strncpy(full_desc, "BALROG  \0", 9);
    break;
  case DRAGON:
    strncpy(full_desc, "DRAGON  \0", 9);
    break;
  case MIMIC:
    strncpy(full_desc, "MIMIC   \0", 9);
    break;
  case VENDOR:
    strncpy(full_desc, "VENDOR  \0", 9);
    break;
  case RUBY_RED:
    strncpy(full_desc, "RUBY RED\0", 9);
    break;
  case NORN_STONE:
    strncpy(full_desc, "NORN STN\0", 9);
    break;
  case PALE_PEARL:
    strncpy(full_desc, "PALE PRL\0", 9);
    break;
  case OPAL_EYE:
    strncpy(full_desc, "OPAL EYE\0", 9);
    break;
  case GREEN_GEM:
    strncpy(full_desc, "GREEN GM\0", 9);
    break;
  case BLUE_FLAME:
    strncpy(full_desc, "BLUE FLM\0", 9);
    break;
  case PALANTIR:
    strncpy(full_desc, "PALANTIR\0", 9);
    break;
  case SILMARIL:
    strncpy(full_desc, "SILMARIL\0", 9);
    break;
  default:
    strncpy(full_desc, "   ??   \0", 9);
    break;
  }
  strncpy(desc, full_desc, 10);
}

char get_room_symbol(int room_content) {
  switch (room_content) {
  case EMPTY_ROOM:
    return '.'; // Empty room
  case ENTRANCE:
    return 'E'; // Entrance
  case STAIRS_UP:
    return 'U'; // Stairs going up
  case STAIRS_DOWN:
    return 'D'; // Stairs going down
  case POOL:
    return 'P'; // Pool
  case CHEST:
    return 'C'; // Chest
  case GOLD:
    return 'G'; // Gold
  case FLARES:
    return 'F'; // Flares
  case WARP:
    return 'W'; // Warp
  case SINKHOLE:
    return 'S'; // Sinkhole
  case CRYSTAL_ORB:
    return 'O'; // Crystal Orb
  case BOOK:
    return 'B'; // Book
  case VENDOR:
    return 'V'; // Vendor
  default:
    // Replace the range case with a check in default
    if (room_content >= MONSTER_START && room_content <= MONSTER_END) {
      return 'M'; // Monsters (all types)
    }
    if (room_content >= TREASURE_START && room_content <= TREASURE_END) {
      return 'T'; // Treasures
    }
    return '?'; // Unknown
  }
}

void use_lamp(Player *player, GameState *game) {
  if (!player->lamp_flag) {
    print_message("You don't have a lamp!\n");
    return;
  }

  if (player->blindness_flag == 1) {
    print_message("You are blind, you can't use a lamp!\n");
    return;
  }
  print_message("Which direction do you want to shine the lamp? (N/S/E/W) ");
  char direction = get_user_input();
  int dx = 0, dy = 0;

  switch (direction) {
  case 'N':
    dx = -1;
    break;
  case 'S':
    dx = 1;
    break;
  case 'W':
    dy = -1;
    break;
  case 'E':
    dy = 1;
    break;
  default:
    print_message("Invalid direction!\n");
    return;
  }

  int x = WRAP_COORDINATE(player->x + dx);
  int y = WRAP_COORDINATE(player->y + dy);

  mark_room_discovered(game, x, y, player->level);
  int room_content = get_room_content(game, x, y, player->level);
  char room_desc[10];
  get_room_description(room_content, room_desc);

  print_message("\nThe lamp reveals: ");
  print_message_formatted("%s\n", room_desc);
}

void use_flare(Player *player, GameState *game) {
  if (player->flares <= 0) {
    print_message("You don't have any flares!\n");
    return;
  }
  if (player->blindness_flag == 1) {
    print_message("You are blind, you can't use flares!\n");
    return;
  }

  player->flares--;
  print_message(
      "\nYou light a flare. It illuminates the surrounding rooms:\n\n");

  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      int x = WRAP_COORDINATE(player->x + dx);
      int y = WRAP_COORDINATE(player->y + dy);

      mark_room_discovered(game, x, y, player->level);
      int room_content = get_room_content(game, x, y, player->level);
      char room_desc[9];
      get_room_description(room_content, room_desc);

      if (dx == 0 && dy == 0) {
        print_message("  [You]   ");
      } else {
        print_message_formatted(" %s ", room_desc);
      }
    }
    print_message("\n");
  }
}

void open_chest(Player *player, GameState *game) {
  print_message("\nYou open the chest and ");

  int event = random_number(7), damage, gold, flares, book_type;
  switch (event) {
  case 1:
    print_message("Kaboom! It explodes!!\n");
    damage = random_number(6);
    player->strength -= damage;
    print_message("You take %d damage.\n", damage);
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);

    if (player->strength <= 0) {
      print_message("\nThe explosion killed you. You died from lack of "
                    "strength. This is the end for you adventurer.\n");
      game->game_over = 1;
    }
    break;

  case 2:
    print_message("Oh no! It's a mimic!\n");
    print_message("The chest sprouts teeth and tentacles!\n");

    if (player->dexterity > 8 + random_number(4) && random_number(4) != 1) {
      // Player might notice it's a mimic if they're dexterous
      print_message("You jump back just in time as the mimic lunges!\n");

      // Move player in a random direction
      char directions[] = {'N', 'S', 'E', 'W'};
      char direction = directions[random_number(4) - 1];
      set_room_content(game, player->x, player->y, player->level, MIMIC);
      move_player(player, game, direction);
    } else {
      // Calculate initial surprise attack damage
      damage = random_number(6) + 2; // 3-8 base damage

      // Apply armor reduction if player has armor
      if (player->armor_type != 0) {
        int armor_protection =
            player->armor_type +
            random_number(2); // Armor type plus small random bonus
        print_message("Your armor absorbs some of the mimic's attack!\n");

        // Reduce damage based on armor
        damage -= armor_protection;

        // Apply armor damage
        player->armor_points -= armor_protection;

        if (damage < 0) {
          damage = 0;
        }

        if (player->armor_points <= 0) {
          player->armor_points = 0;
          player->armor_type = 0;
          print_message(
              "Your armor has been destroyed by the mimic's powerful bite!\n");
        } else {
          print_message("Your armor took %d points of damage!\n",
                        armor_protection);
        }
      }

      if (damage > 0) {
        print_message("The mimic's surprise attack hits you for %d damage!\n",
                      damage);
        player->strength -= damage;

        if (player->strength <= 0) {
          print_message(
              "\nThe mimic devours you whole! Your adventure ends here.\n");
          game->game_over = 1;
        } else {
          set_room_content(game, player->x, player->y, player->level, MIMIC);
          fight_monster(player, game, 1); // Initiate combat immediately
        }
      } else {
        print_message(
            "Your armor completely protects you from the surprise attack!\n");
        set_room_content(game, player->x, player->y, player->level, MIMIC);
        fight_monster(player, game, 1); // Initiate combat
      }
    }
    break;

  case 3:
    print_message("Gas!! You stagger from the room!\n");
    game->turn_count += 20; // Equivalent to T = T + 20 in BASIC
    // Move player in a random direction
    char directions[] = {'N', 'S', 'E', 'W'};
    char direction = directions[random_number(4) - 1];
    // I set it here because it has to be before I move the player
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
    move_player(player, game, direction);
    break;

  case 4:
  case 5:
    gold = random_number(500) + random_number(500);
    print_message("find %d gold pieces!\n", gold);
    player->gold += gold;
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
    break;

  case 6:
    flares = random_number(6);
    print_message("find %d flares!\n", flares);
    player->flares += flares;
    set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
    break;
  case 7:
      // 1 in 7 chance of finding a book
      book_type = random_number(3);
      switch(book_type) {
        case 1:
            print_message("you find a dusty book of ancient lore. Reading it increases your intelligence by 1!\n");
            player->intelligence = get_minimum(player->intelligence + 1, MAX_INTELLIGENCE);
            break;
        case 2:
            if(player->stickybook_flag==0)
            {
                 print_message("you find a strange book. As you pick it up, it magically sticks to your hand!\n");
                 player->stickybook_flag = 1;
            }
            else
            {
                 print_message("you find a strange book. As you pick it up, it tries to stick to your hand but then falls to the ground!\n");
            }
            break;
        case 3:
            print_message("you find a sinister-looking book. Reading it makes your head spin!\n");
            if(player->intelligence>1)
            {
                 player->intelligence--;
            }
            break;
      break;
    }
  }
}
void drink_from_pool(Player *player, GameState *game) {
  print_message("\nYou take a drink and ");

  int effect = random_number(8);
  switch (effect) {
  case 1:
    player->strength =
        get_minimum(MAX_STRENGTH, player->strength + random_number(3));
    print_message("feel stronger.\n");
    break;
  case 2:
    player->strength -= random_number(3);
    print_message("feel weaker.\n");
    if (player->strength <= 0) {
      print_message("\nYou died due to lack of strength.\n");
      game->game_over = 1;
    }
    break;
  case 3:
    player->intelligence =
        get_minimum(MAX_INTELLIGENCE, player->intelligence + random_number(3));
    print_message("feel smarter.\n");
    break;
  case 4:
    player->intelligence -= random_number(3);
    print_message("feel dumber.\n");
    if (player->intelligence <= 0) {
      print_message("\nYou died due to lack of intelligence.\n");
      game->game_over = 1;
    }
    break;
  case 5:
    player->dexterity =
        get_minimum(MAX_DEXTERITY, player->dexterity + random_number(3));
    print_message("feel nimbler.\n");
    break;
  case 6:
    player->dexterity -= random_number(3);
    print_message("feel clumsier.\n");
    if (player->dexterity <= 0) {
      print_message("\nYou died due to lack of dexterity.\n");
      game->game_over = 1;
    }
    break;
  case 7: {
    int new_race;
    do {
      new_race = random_number(5);
    } while (new_race == player->race);
    player->race = new_race;
    print_message_formatted("become a %s.\n", get_race_name(player->race));
  } break;
  case 8:
    if (player->sex == MALE) {
      player->sex = FEMALE;
      print_message_formatted("become a female %s.\n",
                              get_race_name(player->race));
    } else {
      player->sex = MALE;
      print_message_formatted("become a male %s.\n",
                              get_race_name(player->race));
    }

    break;
  }

  // The pool remains in the room, so we don't remove it
}

void teleport(Player *player, GameState *game) {
  if (!player->runestaff_flag) {
    print_message("\n** You can't teleport without a runestaff!\n");
    return;
  }

  int new_x, new_y, new_level;

  print_message("\nEnter X-coordinate (1-%d): ", CASTLE_SIZE);
  new_x = get_user_input_number();
  if (new_x < 1 || new_x > CASTLE_SIZE) {
    print_message("Invalid coordinate. Teleportation failed.\n");
    return;
  }

  print_message("Enter Y-coordinate (1-%d): ", CASTLE_SIZE);
  new_y = get_user_input_number();
  if (new_y < 1 || new_y > CASTLE_SIZE) {
    print_message("Invalid coordinate. Teleportation failed.\n");
    return;
  }

  print_message("Enter Z-coordinate (level 1-%d): ", CASTLE_SIZE);
  new_level = get_user_input_number();
  if (new_level < 1 || new_level > CASTLE_SIZE) {
    print_message("Invalid level. Teleportation failed.\n");
    return;
  }

  // X and Y are switched
  player->x = new_y;
  player->y = new_x;
  player->level = new_level;

  print_message("\nYou have teleported to (%d, %d) on level %d.\n", player->y,
                player->x, player->level);

  // Check if the player teleported to the Orb of Zot
  if (player->x == game->orb_location[0] &&
      player->y == game->orb_location[1] &&
      player->level == game->orb_location[2]) {
    print_message("\nGreat unmitigated zot!\n");
    print_message("\nYou just found ***The Orb of Zot***!\n");
    print_message("\nThe runestaff has disappeared!\n");
    player->runestaff_flag = 0;
    player->orb_flag = 1;
    game->orb_location[0] = 0; // Mark as found
  }
  move_player(player, game, 'T');
}

void gaze_into_orb(Player *player, GameState *game) {
  if (get_room_content(game, player->x, player->y, player->level) !=
      CRYSTAL_ORB) {
    print_message("\n** It's hard to gate without an orb!\n");
    return;
  }
  if (player->blindness_flag)
  {
        print_message("\nBlind ");
        print_message_formatted("%s can not gaze.\n", get_race_name(player->race));
        return;
  }
  print_message("\nYou gaze into the crystal orb and see ");

  // Special rare effects (0.5% each)
  int rare_vision = random_number(1000);
  switch (rare_vision) {
  case 1:
    print_message("\nthe orb suddenly flares with an ancient power. A great "
                  "lidless eye wreathed in flame briefly fixes its gaze upon "
                  "you before fading away...\n");
    return;
  case 2:
    print_message("\na small figure with hairy feet sneaking past, clutching "
                  "something precious...\n");
    return;
  case 3:
    print_message("\an ancient scroll covered in angular black letters. The "
                  "writing fills you with unease...\n");
    return;
  case 4:
    print_message("\na grey-robed figure facing down a creature of shadow and "
                  "flame upon a narrow bridge...\n");
    return;
  case 5:
    print_message("\na golden ring spinning, falling into flames...\n");
    return;
  }

  int vision = random_number(6);
  switch (vision) {
  case 1:
    print_message("yourself in a bloody heap!\n");
    player->strength -= random_number(2);
    if (player->strength <= 0) {
      print_message("\nYou died due to lack of strength.\n");
      game->game_over = 1;
    }
    break;
  case 2: {
    const char *monster_name = get_monster_name(
        MONSTER_START + random_number(MONSTER_END - MONSTER_START + 1) - 1);
    char lowercase_name[100]; // Adjust size based on max monster name length
    to_lowercase(lowercase_name, monster_name);
    print_message("yourself drinking from a pool and becoming %s %s!\n",
                  is_vowel(lowercase_name[0]) ? "an" : "a", lowercase_name);
    break;
  }
  case 3: {
    const char *monster_name = get_monster_name(
        MONSTER_START + random_number(MONSTER_END - MONSTER_START + 1) - 1);
    char lowercase_name[100]; // Adjust size based on max monster name length
    to_lowercase(lowercase_name, monster_name);
    print_message("%s %s gazing back at you!\n",
                  is_vowel(lowercase_name[0]) ? "an" : "a", lowercase_name);
    break;
  }
  case 4: {
    int x = random_number(CASTLE_SIZE);
    int y = random_number(CASTLE_SIZE);
    int z = random_number(CASTLE_SIZE);
    int content = get_room_content(game, x, y, z);
    char room_desc[100]; // Adjust size as needed
    get_room_description(content, room_desc);
    print_message("%s at (%d,%d) Level %d.\n", room_desc, x, y, z);
  } break;
  case 5: {
    int x, y, z;
    if (random_number(8) < 4) {
      x = game->orb_location[1];
      y = game->orb_location[0];
      z = game->orb_location[2];
    } else {
      x = random_number(CASTLE_SIZE);
      y = random_number(CASTLE_SIZE);
      z = random_number(CASTLE_SIZE);
    }
    print_message("***The Orb of Zot*** at (%d,%d) ", x, y);
    print_message("Level %d!\n", z);
  } break;
  case 6:
    print_message("a soap opera rerun!\n");
    break;
  }
}

void open_book(Player *player, GameState *game) {
  int effect = random_number(7);

  print_message("You open the book and ");
  if (player->blindness_flag) {
    print_message_formatted("Blind %s can't read books\n",
                            get_race_name(player->race));
    return;
  }

  int rare_book = random_number(200);
  if (rare_book == 1) {
    int rare_effect = random_number(5);
    print_message("You open the book and discover ");
    switch (rare_effect) {
    case 1:
      print_message("find an ancient red leather-bound book with strange "
                    "runes. The pages detail the history of rings of power!  "
                    "Your intelligence increases.\n");
      player->intelligence += 5; // Small bonus for ancient knowledge
      break;
    case 2:
      print_message("find 'There and Back Again', a dusty manuscript written "
                    "in a small, neat hand.  You read the entire manuscript "
                    "and feel more determined to find the Orb of Zot.\n");
      player->dexterity += 3; // Hobbits are naturally dexterous
      break;
    case 3:
      print_message(
          "find a tome written in elegant elvish script, containing ancient "
          "songs of power.  You sing it and you feel stronger.\n");
      player->strength += 3; // Songs of power grant some strength
      break;
    case 4:
      print_message(
          "find a book of ancient dwarvish lore, detailing the secrets of the "
          "great halls.  The knowledge is now seered in you.\n");
      player->intelligence += 3; // Knowledge of the ancient kingdoms
      break;
    case 5:
      print_message("find a black tome with sinister angular script. You "
                    "quickly close it!\n");
      // No penalty, but no bonus either - wisdom is knowing some books should
      // stay closed
      break;
    }
    player->dexterity = get_minimum(MAX_DEXTERITY, player->dexterity);
    player->intelligence = get_minimum(MAX_INTELLIGENCE, player->intelligence);
    player->strength = get_minimum(MAX_STRENGTH, player->strength);

    return;
  }

  switch (effect) {
  case 1:
    print_message("flash! Oh no! You are now a blind %s.\n",
                  player->race == HUMAN ? "human" : "creature");
    player->blindness_flag = 1;
    break;
  case 2:
    print_message("it'S another volume of Zot's poetry! - Yech!!\n");
    break;
  case 3:
    print_message_formatted("it's an old copy of play%s!\n",
                            get_random_species());
    break;
  case 4:
    print_message("it's a manual of dexterity!\n");
    player->dexterity = (player->dexterity += 18) > MAX_DEXTERITY
                            ? MAX_DEXTERITY
                            : player->dexterity;

    break;
  case 5:
    print_message("it's a manual of strength!\n");
    player->strength = (player->strength += 18) > MAX_STRENGTH
                           ? MAX_STRENGTH
                           : player->strength;
    break;
  case 6:
    
    if (player->stickybook_flag==0)
    {
         print_message("the book sticks to your hand -\n");
         print_message("Now you are unable to draw your weapon!\n");
         player->stickybook_flag = 1;
    }
    else
    {
             print_message("the book attempts to stick to your hand but falls to the ground instead.\n");
    }
    break;
  case 7:
    print_message("it's a manual of intelligence!\n");
    player->intelligence = (player->intelligence += 18) > MAX_INTELLIGENCE
                               ? MAX_INTELLIGENCE
                               : player->intelligence;

    break;
  }

  // Remove the book from the room
  set_room_content(game, player->x, player->y, player->level,
                   EMPTY_ROOM); // Empty room
}

void discover_adjacent_rooms(GameState *game, Player *player) {
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      int x = WRAP_COORDINATE(player->x + dx);
      int y = WRAP_COORDINATE(player->y + dy);
      mark_room_discovered(game, x, y, player->level);
    }
  }
}

void display_map(GameState *game, Player *player) {
  if (player->blindness_flag == 0) {
    print_message("\n=== MAP OF LEVEL ");
    print_message("%d", player->level);
    print_message(" ===\n\n");

    // Print top border with column coordinates
    for (int x = 1; x <= CASTLE_SIZE; x++) {
      if (x < 10) {
        print_message("       %d ", x);
      } else {
        print_message("      %d ", x);
      }
    }
    if (CASTLE_SIZE < 10) {
      print_message("\n +");
    } else {
      print_message("\n  +");
    }
    for (int x = 1; x <= CASTLE_SIZE; x++) {
      print_message("--------+");
    }
    print_message("\n");

    for (int x = 1; x <= CASTLE_SIZE; x++) {
      // Print row coordinate
      if (x < 10 && CASTLE_SIZE >= 10) {
        print_message(" %d", x);
      } else {
        print_message("%d", x);
      }
      for (int y = 1; y <= CASTLE_SIZE; y++) {
        print_message("|");
        if (x == player->x && y == player->y) {
          print_message("  [YOU] ");
        } else if (is_room_discovered(game, x, y, player->level)) {
          int room_content = get_room_content(game, x, y, player->level);
          char room_str[10] = "        \0";
          get_room_description(room_content, room_str);
          print_message(room_str);
        } else {
          print_message("????????");
        }
      }
      print_message("|\n");

      // Print horizontal border between rows
      if (x < CASTLE_SIZE) {
        if (CASTLE_SIZE >= 10) {
          print_message(" ");
        }
        print_message(" +");
        for (int x = 1; x <= CASTLE_SIZE; x++) {
          print_message("--------+");
        }
        print_message("\n");
      }
    }

    // Print bottom border
    if (CASTLE_SIZE >= 10) {
      print_message(" ");
    }
    print_message(" +");
    for (int x = 1; x <= CASTLE_SIZE; x++) {
      print_message("--------+");
    }
    print_message("\n");

#ifdef MSDOS
    print_message("PRESS ENTER TO CONTINUE...");
    while (getchar() != '\n')
      ; // Wait for Enter key
#endif

    // Map is too large for the default font for MS-DOS (Same Information is
    // available in help)
    print_message("\nLEGEND:\n");
    print_message("[YOU]    = Your location   EMPTY    = Empty room     "
                  "ENTRANCE = Entrance\n");
    print_message("POOL     = Magic Pool      CHEST    = Treasure Chest\n");
    print_message("GOLD     = Gold Pieces     FLARES   = Flares\n");
    print_message("WARP     = Warp/Orb        SINKHOLE = Sinkhole\n");
    print_message("CRYSTAL  = Crystal Orb     BOOK     = Magic Book\n");
    print_message("MONSTER  = Monster Name    VENDOR   = Vendor\n");
    print_message("TREASURE = Treasure Name   ???????? = Undiscovered\n");
    print_message("STAIRS UP= Stairs U        STAIRS D = Stairs Down\n");
  } else {
    print_message("\nBlind %s can't see maps.\n", get_race_name(player->race));
  }
}

/*
char* strip(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    const char* start = str;
    while (*start && (*start == ' ' || *start == '\t')) {
        start++;
    }
    if (*start == '\0') {
        return strdup("");
    }
    const char* end = str + strlen(str) - 1;
    while (end > start && (*end == ' ' || *end == '\t')) {
        end--;
    }
    size_t len = end - start + 1;
    char* result = (char*)malloc(len + 1);  // Problem: Dynamic allocation
    if (result == NULL) {
        return NULL;
    }
    strncpy(result, start, len);
    result[len] = '\0';
    return result;
} */

void capitalize_sentences(char *str) {
  if (str == NULL || *str == '\0') {
    return;
  }
  bool capitalize_next = true;

  for (int i = 0; str[i] != '\0'; i++) {
    if (isalpha((unsigned char)str[i])) {
      if (capitalize_next) {
        str[i] = toupper((unsigned char)str[i]);
        capitalize_next = false;
      } else {
        str[i] = tolower((unsigned char)str[i]);
      }
    } else if (str[i] == '.' || str[i] == '?' || str[i] == '!') {
      capitalize_next = true;
    } else if (!isspace((unsigned char)str[i])) {
      capitalize_next = false;
    }
  }
}
