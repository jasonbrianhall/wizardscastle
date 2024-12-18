#include "wizards-castle.h"
#include "combat.h"
#include "gamestate.h"
#include "player.h"
#include "save_load.h"
#include "utilities.h"
#include "vendor.h"
#include "wizardio.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_introduction(void) {
    printStars();
    print_message("\n                *  The Wizard's Castle  *\n\n");
    printStars();
    print_message("\n");
    print_message("Many cycles ago, in the Kingdom of ");
    print_message("N'dic, the gnomic\n"
                           "wizard ");
    print_message("Zot forged his great ");
    print_message("*Orb of Power*. ");
    print_message(
        "He soon\n"
        "vanished, leaving behind his vast subterranean castle\n"
        "filled with esurient monsters, fabulous treasures, and\n"
        "the incredible ");
    print_message("*Orb of Zot*.");
    print_message(
        " From that time hence, many\n"
        "a bold youth has ventured into the Wizard's Castle. As\n"
        "of now, *NONE* has ever emerged victoriously! Beware!!\n\n");
}

bool main_game_loop(Player *player, GameState *game) {
  char user_command[100];
  int game_over = 0;
  int currentgold;
  const char *room_contents[] = {
      "AN EMPTY ROOM",  "THE ENTRANCE", "STAIRS GOING UP", "STAIRS GOING DOWN",
      "A POOL",         "A CHEST",      "GOLD PIECES",     "FLARES",
      "A WARP",         "A SINKHOLE",   "A CRYSTAL ORB",   "A BOOK",
      "A KOBOLD",       "AN ORC",       "A WOLF",          "A GOBLIN",
      "AN OGRE",        "A TROLL",      "A BEAR",          "A MINOTAUR",
      "A GARGOYLE",     "A CHIMERA",    "A BALROG",        "A DRAGON",
      "A MIMIC",        "A VENDOR",     "A RED RUBY",      "THE NORN STONE",
      "THE PALE_PEARL", "THE OPAL EYE", "A GREEN GEM",     "THE BLUE FLAME",
      "THE PALANTIR",   "THE SILMARIL"};

  while (!game_over) {
    game->turn_count++;
    mark_room_discovered(game, player->x, player->y, player->level);

    // Handle curses
    if (player->runestaff_flag == 0 && player->orb_flag == 0) {
      // Lethargy curse (similar to line 1960 in BASIC)
      if (!game->treasure[RUBY_RED_INDEX] &&
          random_number(100) <= 5) { // 5% chance if no Ruby Red
        game->turn_count++;
        print_message("\nYou are affected by lethargy. You lose a turn.\n");
      }

      // Leech curse (similar to line 1965 in BASIC)
      if (!game->treasure[PALE_PEARL_INDEX] &&
          random_number(100) <= 5) { // 5% chance if no Pale Pearl
        int gold_lost = random_number(5);
        currentgold = player->gold;
        player->gold =
            (player->gold > gold_lost) ? player->gold - gold_lost : 0;
        print_message("\nA leech attacks you! You lose %d gold pieces.\n", currentgold - player->gold);
      }

      // Forgetfulness curse (similar to lines 1975-2015 in BASIC)
      if (!game->treasure[GREEN_GEM_INDEX] &&
          random_number(100) <= 5) { // 5% chance if no Green Gem
        //int old_x = player->x, old_y = player->y, old_z = player->level;
        player->x = random_number(CASTLE_SIZE);
        player->y = random_number(CASTLE_SIZE);
        player->level = random_number(CASTLE_SIZE);
        print_message("\nYou suddenly forget where you are!\n");
        print_message("You find yourself at (%d,%d) on level %d.\n",
                                player->x, player->y, player->level);
        mark_room_discovered(game, player->x, player->y, player->level);
        //index = CALCULATE_ROOM_INDEX(player->level, player->x, player->y);
        //game->discovered_rooms[index] = 1;
       
        // If player was in an empty room, mark old room as unexplored
        /*if (get_room_content(game, old_x, old_y, old_z) == EMPTY_ROOM) {
          int index = CALCULATE_ROOM_INDEX(old_z, old_x, old_y);
          game->discovered_rooms[index] = 0;
        }*/
      }
    }

    // Display random events (similar to lines 2010-2060 in BASIC)
    if (random_number(5) == 1) {
      print_message("\nYou ");
      int event_type = random_number(7) + player->blindness_flag;
      if (event_type > 7)
        event_type = 4;

      switch (event_type) {
      case 1:
        print_message("see a bat fly by!\n");
        break;
      case 2:
        print_message("hear footsteps!\n");
        break;
      case 3:
        print_message("sneezed!\n");
        break;
      case 4:
        print_message("stepped on a frog!\n");
        break;
      case 5:
        print_message("smell something frying!\n");
        break;
      case 6:
        print_message("feel like you are being watched!\n");
        break;
      case 7:
        print_message("Hear faint rustling noises!\n");
        break;
      }
    }

    // Handle blindness cure (similar to lines 2065-2075 in BASIC)
    if (player->blindness_flag == 1 && game->treasure[OPAL_EYE_INDEX] == 1) {
      print_message("\nThe Opal Eye cures your blindness!\n");
      player->blindness_flag = 0;
    }

    // Handle sticky book cure (similar to lines 2080-2090 in BASIC)
    if (player->stickybook_flag == 1 && game->treasure[BLUE_FLAME_INDEX] == 1) {
      print_message("\nThe Blue Flame dissolves the book!\n");
      player->stickybook_flag = 0;
    }

    print_message("\n");
    // print_status(player, game);

    int room_content =
        get_room_content(game, player->x, player->y, player->level);
    if (room_content == ENTRANCE) { // The Entrance
      print_message("Ok, ");
      print_message("%s, you are now entering the castle!\n",
                              get_race_name_flc(player->race));
    } else if (room_content >= EMPTY_ROOM && room_content <= TREASURE_END) {
      print_message_formatted("Here you find %s.\n",
                              room_contents[room_content - EMPTY_ROOM]);
      if (room_content == GOLD) {
        int gold_found =
            random_number(1000); // Random amount between 1 and 1000
        player->gold += gold_found;
        print_message(
            "%d gold pieces have been added to your inventory!\n", gold_found);
        set_room_content(game, player->x, player->y, player->level,
                         EMPTY_ROOM); // Empty the room
      } else if (room_content == FLARES) {
        int flares_found = random_number(5); // Random amount between 1 and 5

        player->flares += flares_found;
        print_message(
            "%d flares have been added to your inventory!\n", flares_found);
        set_room_content(game, player->x, player->y, player->level,
                         EMPTY_ROOM); // Empty the room
      }
      // Monsters
      else if (room_content >= MONSTER_START && room_content <= MONSTER_END) {
        fight_monster_normalize(player, game);
      }
      // Vendors
      else if (room_content == VENDOR) {
        handle_vendor(player, game);
      }
      // Treasure
      else if (room_content >= TREASURE_START && room_content <= TREASURE_END) {
        handle_treasure(player, game, room_content);
      }

    } else {
      print_message("Here you find an unknown room.\n");
    }
    game_over = check_game_over(player, game);
    if (!game_over) {
      strncpy(user_command, get_user_input_main(), sizeof(user_command) - 1);
      user_command[sizeof(user_command) - 1] = '\0'; // Ensure null-termination
      switch (user_command[0]) {
      case '\0':
        print_message("\n\nPlease enter a command.\n\n");
        break;

      case 'N':
      case 'S':
      case 'E':
      case 'W':
        move_player(player, game, user_command[0]);
        break;
      case 'U':
        if (room_content == STAIRS_UP) { // Stairs going up
          player->level++;
          if (player->level > CASTLE_SIZE) {
            player->level = 1;
          }
          print_message("You climb up the stairs.\n");
        } else {
          print_message("There are not stairs going up from here\n");
        }
        break;
      case 'D':
        if (strncmp(user_command, "DR", 2) == 0) {
          if (room_content == POOL) {
            drink_from_pool(player, game);
          } else {
            print_message("** If you want to drink, find a pool!\n");
          }
        } else if (room_content == STAIRS_DOWN) { // Stairs going down
          player->level--;
          if (player->level < 1) {
            player->level = CASTLE_SIZE;
          }
          print_message("You descend the stairs.\n");
        } else {
          print_message("There are no stairs coming down from here!\n");
        }
        break;
      case 'M':
        display_map(game, player);
        break;
      case 'F':
        use_flare(player, game);
        break;
      case 'L':
        use_lamp(player, game);
        break;
      case 'O':
        if (room_content == CHEST) { // Chest
          open_chest(player, game);
        } else if (room_content == BOOK) { // Book
          open_book(player, game);
        } else {
          print_message("There's nothing here to open!\n");
        }
        break;
      case 'G':
        if (room_content == CRYSTAL_ORB) { // Crystal orb
          gaze_into_orb(player, game);
        } else {
          print_message(
              "There's no crystal orb in here to gaze into!\n");
        }
        break;
      case 'T':
        if (player->runestaff_flag) {
          teleport(player, game);
        } else {
          print_message("You can't teleport without the Runestaff!\n");
        }
        break;
      case 'Q':
        if (strncmp(user_command, "QS", 2) == 0) {
          if (save_game("quicksav.wcs", player, game)) {
            print_message("Quick save was successful.\n");
          } else {
            print_message("Quick save failed.\n");
          }
        } else if (strncmp(user_command, "QL", 2) == 0) {
          if (load_game("quicksav.wcs", player, game)) {
            print_message("Quick load was successful\n");
          } else {
            print_message("Quick load failed.\n");
          }
        } else {
          print_message("Do you really want to quit now? (Y/N) ");
          if (get_user_input_yn() == 'Y') {
            game_over = 1;
          } else {
            print_message("Ok, continue on brave adventurer!\n");
          }
        }
        break;
      case 'Z':
        print_status(player, game);
        break;
      case 'H':
        print_help();
        break;
      default:
        print_message("Invalid command. Type 'H' for help.\n");
      }
    }
    if (!game_over) {
      game_over = check_game_over(player, game);
    }
  }

  end_game(player, game);
  // Ask if the player wants to play again
#ifndef __ANDROID__
  print_message("\nAre you foolish enough to want to play again? ");
  char play_again = get_user_input_yn();
  if (play_again == 'Y') {
    print_message("\nSome adventurers never learn!\n\n");
    print_message("Please be patient while the castle is restocked.\n\n");
    return 1;
  } else {
    print_message("\nGood bye, and good luck in your travels!\n");
    return 0;
  }
#else
  // Infinite loop if Android
  print_message("\n\nPlease be patient while the castle is restocked.\n\n");
  return 1;
#endif
}

void print_help() {
  print_message(
      "\n*** WIZARD'S CASTLE COMMAND AND INFORMATION SUMMARY ***\n\n");
  print_message("THE FOLLOWING COMMANDS ARE AVAILABLE:\n\n");
  print_message("H/ELP     - Display this help information\n");
  print_message("N/ORTH    - Move north\n");
  print_message("S/OUTH    - Move south\n");
  print_message("E/AST     - Move east\n");
  print_message("W/EST     - Move west\n");
  print_message("U/P       - Go up stairs\n");
  print_message("D/OWN     - Go down stairs\n");
  print_message("DR/INK    - Drink from a pool\n");
  print_message("M/AP      - Display map of current level\n");
  print_message("F/LARE    - Light a flare\n");
  print_message("L/AMP     - Use lamp to look into adjacent room\n");
  print_message("O/PEN     - Open a chest or book\n");
  print_message("G/AZE     - Gaze into a crystal orb\n");
  print_message(
      "T/ELEPORT - Teleport to a new location (requires Runestaff)\n");
  print_message("Q/UIT     - End the game\n");
  print_message("Z/tatus   - Player Status (South was already used)\n");
  print_message("QS/ave    - Quick Save (saves a file named quicksav.wcs)\n");
  print_message("QL/oad    - Quick Load\n\n");

#ifdef MSDOS
  print_message("Press enter to continue...");
  while (getchar() != '\n')
    ; // Wait for Enter key
#endif

  print_message("The contents of the room are as follows:\n\n");
  print_message(
      "EMPTY    = EMPTY ROOM      BOOK     = BOOK            Chest = CHEST\n");
  print_message("STAIRS D = STAIRS DOWN     ENTRANCE = ENTRANCE/EXIT   FLARES "
                "= FLARES\n");
  print_message("GOLD     = GOLD PIECES     MONSTER  = MONSTER NAME    ORB = "
                "CRYSTAL ORB\n");
  print_message("POOL     = MAGIC POOL      SINKHOLE = SINKHOLE        "
                "TREASURE = TREASURE\n");
  print_message("STAIRS U = STAIRS UP       VENDOR   = VENDOR          WARP = "
                "WARP/ORB\n\n");

  print_message("The benefits of having treasure are:\n\n");
  print_message("RUBY RED    - Avoid Lethargy     PALE PEARL  - Avoid Leech\n");
  print_message(
      "GREEN GEM   - Avoid Forgetting   OPAL EYE    - Cures Blindess\n");
  print_message("BLUE FLAME  - Dissolves Books    NORN STONE  - Pretty\n");
  print_message("PALANTIR    - Pretty             SILMARIL    - Pretty\n\n");
}
