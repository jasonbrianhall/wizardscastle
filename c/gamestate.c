
#include "gamestate.h"
#include "utilities.h"
#include "wizardio.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Game ending functions
int check_game_over(Player *player, GameState *game) {
  // Check if player has died
  if (player->strength <= 0 || player->intelligence <= 0 ||
      player->dexterity <= 0) {
    return 1;
  }

  // Check if player has won (e.g., found the Orb of Zot and exited)
  /*if (player->orb_flag && player->x == 1 && player->y == 4 && player->level ==
  1) { return 1;
  }*/

  if (game->game_over) {
    return 1;
  }
  // Game is not over
  return 0;
}

void end_game(Player *player, GameState *game) {
    printStars();
    
    const char *race_names[] = {
        "hobbit", "elf", "human", "dwarf", "drow"
    };
    const char *armor_types[] = {
        "No Armor", "Leather", "Chainmail", "Plate", "Stone"
    };
    const char *weapon_types[] = {
        "No Weapon", "Dagger", "Mace", "Sword", "Excalibur"
    };

    if (player->strength <= 0 || player->intelligence <= 0 || player->dexterity <= 0) {
        print_message("A noble effort, oh formerly living %s!\n\n", race_names[player->race - 1]);
        
        if (player->strength <= 0) {
            print_message("You died due to lack of strength.\n\n");
        } else if (player->intelligence <= 0) {
            print_message("You died due to lack of intelligence.\n\n");
        } else {
            print_message("You died due to lack of dexterity.\n\n");
        }
        print_message("At the time you died, you had:\n");
    } else if (game->victory) {
        print_message("Congratulations!\n\n");
        print_message("You left the castle with the Orb of Zot.\n\n");
        print_message("An incredibly glorious victory!!\n\n");
        print_message("In addition, you got out with the following:\n");
    } else {
        print_message("You left the castle without the Orb of Zot.\n\n");
        print_message("A less than awe-inspiring defeat.\n\n");
        print_message("When you left the castle, you had:\n");
    }

    // List treasures
    for (int i = 0; i < TREASURE_COUNT; i++) {
        if (game->treasure[i]) {
            print_message_formatted("%s\n", get_treasure_name(i));
        }
    }

    // Print equipment
    print_message("%s and %s", weapon_types[player->weapon_type], 
                  armor_types[player->armor_type]);
    
    if (player->lamp_flag) {
        print_message(" and a lamp");
    }
    print_message("\n");

    // Print flares and gold
    print_message("You also had %d flares and %d gold pieces\n",
                 player->flares, player->gold);

    // Print Runestaff status
    if (player->runestaff_flag) {
        print_message("and the Runestaff\n");
    }

    // Print turn count
    print_message("\nand it took you %d turns!\n", game->turn_count);
    
    printStars();
}

// Map and room functions
void generate_castle(GameState *game) {
  int x, y, z, q, q1, level;

  // Seed the random number generator
  srand(time(NULL));
  for (int i = 0; i < TREASURE_END - TREASURE_START + 1; i++) {
    game->treasure[i] = 0;
  }
  game->game_over = 0;
  game->victory = 0;
  game->vendor_attacked = 0;
  game->turn_count = 0;

  // Seed the random number generator
  srand(time(NULL));

  // Set the entrance
  x = 1;
  y = 4;
  level = 1;
  game->location_map[CALCULATE_ROOM_INDEX(level, x, y)] = 2; // Entrance

  // Initialize all rooms to empty (101)
  for (q = 0; q < MAP_SIZE; q++) {
    game->location_map[q] = EMPTY_ROOM;
    game->discovered_rooms[q] = 0; // 0 means undiscovered
  }

  int entrance_index = CALCULATE_ROOM_INDEX(level, x, y);
  game->location_map[entrance_index] = ENTRANCE;
  game->discovered_rooms[entrance_index] = 1; // 1 means discovered

  // Place stairs
  for (z = 1; z <= CASTLE_SIZE; z++) {
    for (int i = 1; i <= 2; i++) {
      do {
        x = random_number(CASTLE_SIZE);
        y = random_number(CASTLE_SIZE);
      } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);

      // Place stairs down on current level
      game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] =
          STAIRS_UP; // Stairs down

      // Place corresponding stairs up on the level below (or on level 1 if
      // we're on level 8)
      int next_level = (z % CASTLE_SIZE + 1);
      game->location_map[CALCULATE_ROOM_INDEX(next_level, x, y)] =
          STAIRS_DOWN; // Stairs up
    }
  }

  // Place other elements
  for (z = 1; z <= CASTLE_SIZE; z++) {
    // Place monsters, treasures, etc.
    for (q = MONSTER_START; q <= MONSTER_END - 1;
         q++) { // Don't put Mimics (-1)
      do {
        x = random_number(CASTLE_SIZE);
        y = random_number(CASTLE_SIZE);
      } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
      game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
    }

    // Place pools, chests, and gold
    for (q1 = 1; q1 <= 3; q1++) {
      for (q = START_ITEMS; q <= END_ITEMS; q++) {
        do {
          x = random_number(CASTLE_SIZE);
          y = random_number(CASTLE_SIZE);
        } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] !=
                 EMPTY_ROOM);
        game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
      }

      // Place vendor
      do {
        x = random_number(CASTLE_SIZE);
        y = random_number(CASTLE_SIZE);
      } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
      game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = VENDOR;
    }
  }

  // Place special items (crystal orb, ruby red, etc.)
  for (q = TREASURE_START; q <= TREASURE_END; q++) {
    do {
      z = random_number(CASTLE_SIZE);
      x = random_number(CASTLE_SIZE);
      y = random_number(CASTLE_SIZE);
    } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
  }

  // Place treasures (126-133)
  for (q = TREASURE_START; q <= TREASURE_END; q++) {
    do {
      z = random_number(CASTLE_SIZE);
      x = random_number(CASTLE_SIZE);
      y = random_number(CASTLE_SIZE);
    } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
    game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = q;
  }

  // Place the Runestaff
  do {
    z = random_number(CASTLE_SIZE);
    x = random_number(CASTLE_SIZE);
    y = random_number(CASTLE_SIZE);
  } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
  game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] =
      MONSTER_START + random_number(MONSTER_END - MONSTER_START) - 1;
  game->runestaff_location[0] = x;
  game->runestaff_location[1] = y;
  game->runestaff_location[2] = z;

  // Place the Orb of Zot
  do {
    z = random_number(CASTLE_SIZE);
    x = random_number(CASTLE_SIZE);
    y = random_number(CASTLE_SIZE);
  } while (game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] != EMPTY_ROOM);
  game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = WARP;
  game->orb_location[0] = x;
  game->orb_location[1] = y;
  game->orb_location[2] = z;

  // Set the entrance
  x = 1;
  y = 4;
  z = 1;
  game->location_map[CALCULATE_ROOM_INDEX(z, x, y)] = ENTRANCE;
}

int get_room_content(GameState *game, int x, int y, int level) {
  int index = CALCULATE_ROOM_INDEX(level, x, y);
  if (index >= 0 && index < MAP_SIZE) {
    return game->location_map[index];
  }
  print_message("Invalid Room %d\n", index);
  return -1; // Invalid room
}

void set_room_content(GameState *game, int x, int y, int level, int content) {
  int index = CALCULATE_ROOM_INDEX(level, x, y);
  if (index >= 0 && index < MAP_SIZE) {
    game->location_map[index] = content;
  } else {
    // Handle error: invalid room coordinates
    print_message(
        "Error: Attempted to set invalid room content at (%d,%d) level %d\n", x,
        y, level);
  }
}

void mark_room_discovered(GameState *game, int x, int y, int level) {
  int index = CALCULATE_ROOM_INDEX(level, x, y);
  game->discovered_rooms[index] = 1;
}

int is_room_discovered(GameState *game, int x, int y, int level) {
  // print_message("is room discovered %i %i %i %i", level, x, y
  // CALCULATE_ROOM_INDEX(level, x, y));
  int index = CALCULATE_ROOM_INDEX(level, x, y);

  // print_message("Discovered %i, %i\n\n", CALCULATE_ROOM_INDEX(level, x, y),
  // game->discovered_rooms[index]);
  return game->discovered_rooms[index];
}

void handle_treasure(Player *player, GameState *game, int room_content) {
  int treasure_index = room_content - TREASURE_START;
  const char *treasure_name = get_treasure_name(treasure_index);

  print_message_formatted("\nYou found %s!\n\n", treasure_name);

  game->treasure[treasure_index]++;
  player->treasure_count = 0;
  for (int i = 0; i < TREASURE_END - TREASURE_START + 1; i++) {
    if (game->treasure[i] >= 1) {
      player->treasure_count += game->treasure[i];
    }
  }

  print_message("You now have %d treasures.\n\n",
                          player->treasure_count);

  // Apply special effects of treasures
  switch (treasure_index) {
  case RUBY_RED_INDEX: // Ruby Red
    print_message("The Ruby Red will help you avoid lethargy.\n\n");
    break;
  case NORN_STONE_INDEX: // Norn Stone
    print_message(
        "The Norn Stone glows with an otherworldly light.\n\n");

    break;
  case PALE_PEARL_INDEX: // Pale Pearl
    print_message(
        "The Pale Pearl will protect you from Leeches.\n\n");
    break;
  case OPAL_EYE_INDEX: // Opal Eye
    print_message("The Opal Eye will cure your blindness.\n");
    if (player->blindness_flag) {
      player->blindness_flag = 0;
      print_message("Your blindness is cured!\n");
    }
    print_message("\n");
    break;
  case GREEN_GEM_INDEX: // Green Gem
    print_message(
        "The Green Gem will help prevent you from forgetting.\n\n");
    break;
  case BLUE_FLAME_INDEX: // Blue Flame
    print_message("The blue flames dissolves books.\n\n");
    if (player->stickybook_flag) {
      player->stickybook_flag = 0;
      print_message("The sticky book dissolves!\n\n");
    }
    break;
  case PALANTIR_INDEX: // Palantir
  case SILMARIL_INDEX: // Silmaril
    print_message("Its beauty is beyond compare.\n\n");
    break;
  }

  // Remove the treasure from the room
  set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
}
