#include "combat.h"
#include "gamestate.h"
#include "utilities.h"
#include "wizardio.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void fight_monster(Player *player, GameState *game) {
  int room_content =
      get_room_content(game, player->x, player->y, player->level);
  int is_vendor = (room_content == VENDOR);
  int enemy_strength, enemy_dexterity, enemy_intelligence,
      muted = 0, spellcasted, temp, base_chance, success_chance, random_factor,
      avoidance_chance;
  int can_bribe = 1;
  const char *enemy_name =
      is_vendor ? "VENDOR" : get_monster_name(room_content);
  int firststrike, chance;
  char choice;
  int max_increase;
  player->web_count = 0;
  player->temp_strength = 0;
  player->temp_intelligence = 0;
  player->temp_dexterity = 0;
  if (game->game_over) // No use fighting a monster if you are dead.
  {
    return;
  }

  if (can_escape(player, is_vendor, enemy_name)) {
    return;
  }

  // Set enemy stats based on room content
  if (is_vendor) {
    enemy_strength = 15;
    enemy_dexterity = 15;
    enemy_intelligence = 15;
  } else if (room_content == MIMIC) {
    enemy_strength = 8;
    enemy_dexterity = 12;
    enemy_intelligence = 12;
  } else {
    enemy_strength = (room_content - MONSTER_START + 1) * 2;
    enemy_dexterity = room_content - MONSTER_START + 8;
    enemy_intelligence = room_content - MONSTER_START + 8;
    if (enemy_strength <= 10) {
      enemy_strength = 10;
    }
  }

  firststrike = calculate_first_strike(player->dexterity, player->intelligence,
                                       player->strength, enemy_dexterity,
                                       enemy_intelligence, enemy_strength);
  if (room_content == MIMIC) {
    firststrike = 1;
  } else if (firststrike == 0) {
    print_message("Enemy gets first strike\n\n");
  }
  int rally_used = 0;
  while (1) {
    if (firststrike == 1) {
      print_message("\nYou're facing a ");
      print_message_formatted("%s!\n\n", enemy_name);
      print_message("You may (A)ttack or (R)etreat.\n");
      if (can_bribe) {
        print_message("You can also attempt a (B)ribe.\n");
      }
      if (can_cast_magic(player, muted)) {
        print_message("You can also (C)ast a spell.\n");
      }
      if (player->race == HOBBIT) {
        print_message("You can also (T)hrow a stone.\n");
      }
      if ((player->race == ELF || player->race == DROW) &&
          player->intelligence >= 8 && player->stickybook_flag == 0) {
        print_message("You can also (S)hoot a magical bow with extremely high "
                      "accuracy.\n");
      }

      if (player->race == HUMAN && rally_used == 0) {
        print_message("You can also Ra(L)ly.\n");
      }

      print_message("\n");
      print_message("Your strength is %d, your dexterity is %d, and "
                    "your intelligence is %d.\n",
                    player->strength, player->dexterity, player->intelligence);

      print_message("The enemies strength is %d, dexterity is %d, "
                    "and intelligence is %d.\n\n",
                    enemy_strength, enemy_dexterity, enemy_intelligence);

      choice = get_user_input_custom_prompt("Fight command:  ");

      switch (choice) {
      case 'A':
        if (player->weapon_type == 0) {
          print_message_formatted("\n** Pounding on %s won't hurt it!\n",
                                  enemy_name);
        } else if (player->stickybook_flag) {
          print_message(
              "\n** You can't bludgeon it to death with a mere book!\n");
        } else if (random_number(20) + player->dexterity <=
                   random_number(20) + enemy_dexterity +
                       (3 * player->blindness_flag)) {
          print_message("\nYou missed, too bad!\n");
        } else {
          temp = calculate_damage(player, enemy_strength, enemy_dexterity);

          // 20% chance of a critical hit as a Dwarf.
          chance = player->blindness_flag == 1 ? 9 : 5;

          if (player->race == DWARF &&
              random_number(chance) == 1) { // 20% chance
            int extra_damage = random_number(player->strength / 3);
            temp += extra_damage;
            print_message("You land a mighty blow on the ");
            print_message_formatted(
                "%s, dealing %d extra damage!  Total damage was %d.\n",
                enemy_name, extra_damage, temp);
          } else {
            print_message("\nYou hit the evil ");
            print_message_formatted("%s AND DID %i DAMAGE!\n", enemy_name,
                                    temp);
          }
          enemy_strength -= temp;
          if ((room_content == GARGOYLE || room_content == DRAGON) &&
              random_number(8) == 1) {
            if (player->weapon_type < EXCALIBUR ||
                (player->weapon_type >= EXCALIBUR && random_number(8) == 1)) {
              print_message_formatted("\nOh no! Your %s broke!\n",
                                      get_weapon_name(player->weapon_type));
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
        temp = 0;
        if (player->race == HOBBIT) {
          // Hobbits are sneaky and more able to escape.
          temp = player->dexterity / 6;
        }
        if (random_number(20) + player->dexterity + temp >
            random_number(20) + enemy_dexterity) {
          print_message("\nYou have successfully evaded your foe!\n");
          if (player->race == HOBBIT) {
            print_message("Hobbitsons are sneaky.\n");
          }
          move_player_randomly(player, game);
          return;
        } else {
          print_message("\nYour retreat has failed!\n");
        }
        break;
      case 'B':
        if (!can_bribe) {
          print_message("\n** Choose one of the options presented.\n");
          continue;
        }
        if (handle_bribe(player, game, enemy_name)) {
          return;
        }
        can_bribe = 0;
        break;
      case 'C':
        if (can_cast_magic(player, muted)) {
          // Player can cast a spell
          if (handle_spell(player, game, &enemy_strength, &enemy_intelligence,
                           &enemy_dexterity, enemy_name)) {
            if (game->game_over) {
              return;
            }
            if (enemy_strength <= 0) {
              handle_combat_victory(player, game, is_vendor, enemy_name);
              return;
            }
          }
        } else {
          // Player cannot cast a spell
          print_message("\n** You cannot cast a spell at this moment!\n");
          continue; // Skip enemy's turn if player couldn't cast a spell
        }
        break;
      case 'S': // Shoot magical bows
        if ((player->race == ELF || player->race == DROW) &&
            player->intelligence >= 8 && player->stickybook_flag == 0) {
          chance = player->blindness_flag == 1 ? 4 : 5;

          if (random_number(chance) >= 4 || random_number(chance) >= 4 ||
              random_number(24 - get_minimum(23, player->dexterity)) ==
                  1) // 3/5 twice plus dexterity based chance (100% chance if
                     // dexterity is 23)
          {
            chance = player->blindness_flag == 1 ? 12 : 8;

            if (player->race == DROW &&
                random_number(chance) == 1) { // 12.5% chance
              temp = random_number(4) + 1;    // 2-5 damage
              print_message("Dark Elf Precision! Your strike finds a vital "
                            "spot!  You hit the enemy for %d damage!\n",
                            temp);
            } else if (player->race == ELF) {
              temp = random_number(3) + 1; // 2-4 damage
              print_message("With elven grace, your arrow flies true! You hit "
                            "the enemy for %d damage!\n",
                            temp);
            } else {
              temp = random_number(3); // 1-3 damage
              print_message("You pull out your magical bow and hit "
                            "the enemy for %d damage!\n",
                            temp);
            }
            enemy_strength -= temp;
            if (enemy_strength <= 0) {
              handle_combat_victory(player, game, is_vendor, enemy_name);
              return;
            }
          } else {
            if (player->race == ELF) {
              print_message("Despite your elven skill, your arrow misses its "
                            "mark. Even the best archers have off days.\n");
            } else {
              print_message("You pull out your magic bow and... you miss the "
                            "enemy. Your aim wasn't true.\n");
            }
          }
          break;
        }
        if (player->stickybook_flag == 0) {
          print_message("You don't have any magical arrows.\n");
        } else {
          if (player->race == ELF || player->race == DROW) {
            print_message("Elf, you have book stuck to your hand.  How are you "
                          "going to draw your bow?\n");
          } else {
            print_message("Not sure how you are going to fire magical arrows "
                          "with a book stuck to your hand.\n");
          }
        }
        continue;
      case 'T':
        if (player->race == HOBBIT && player->stickybook_flag == 0) {
          chance = player->blindness_flag == 1 ? 9 : 6;
          if (random_number(chance) == 1 || random_number(chance) == 1 ||
              random_number(24 - get_minimum(23, player->dexterity)) ==
                  1) { // Two 20% chances plus dexterity-based chance
            temp = random_number(3); // 1-3 damage
            print_message(
                "You throw a stone and hit the enemy for %d damage!\n", temp);
            enemy_strength -= temp;
            if (enemy_strength <= 0) {
              handle_combat_victory(player, game, is_vendor, enemy_name);
              return;
            }
          } else {
            print_message("Your thrown stone misses the enemy.\n");
          }
        } else {
          if (player->race == HOBBIT) {
            print_message("You have a book stuck to your hand.  How are you "
                          "going to throw stones?\n");
          } else {
            print_message(
                "Do you look like a hobbit?  You can't cast stones.\n");
          }
        }
        continue;
      case 'L': // Rally
        if (player->race == HUMAN && rally_used == 0) {
          int rally_bonus = random_number(2) + 1;
          if (player->temp_strength == 0) {
            player->temp_strength = player->strength;
          }
          if (player->temp_intelligence == 0) {
            player->temp_intelligence = player->intelligence;
          }
          if (player->temp_dexterity == 0) {
            player->temp_dexterity = player->dexterity;
          }
          player->strength += rally_bonus;
          player->dexterity += rally_bonus;
          player->intelligence += rally_bonus;
          print_message(
              "You rally your spirit, gaining %d to all stats temporarily!\n",
              rally_bonus);
          rally_used = 1;
          break;
        } else if (player->race != HUMAN) {
          print_message("Only humans can use Rally.\n");
        } else {
          print_message("You've already used Rally this battle.\n");
        }
        continue;
      default:
        print_message("\n** Choose one of the options presented.\n");
        continue;
      }
    } else {
      firststrike = 1;
      choice = '\0'; // Enemy has first strike
    }

    // Enemy's turn
    if (player->web_count > 0) {
      player->web_count--;
      if (player->web_count == 0) {
        print_message("\nThe web spell has shattered!\n");
      } else {
        // Determine if the monster breaks free
        if (random_number(20) < 5) { // Example: 25% chance to break free
          print_message_formatted("\nThe %s struggles and breaks free from the "
                                  "web but cannot attack until next turn!\n",
                                  enemy_name);
          player->web_count = 0;
        } else {
          print_message_formatted(
              "\nThe %s is ensnared by the web and cannot attack!\n",
              enemy_name);
          continue; // Skip the enemy's attack
        }
      }
    } else if (room_content == DRAGON &&
               random_number(3) == 1) { // 1 in 3 chance for fireball
      print_message_formatted("\nTHE %s ATTACKS!\n", enemy_name);
      dragon_fireball_attack(player, game, enemy_strength, enemy_dexterity,
                             enemy_intelligence);
      if (game->game_over) {
        return;
      }
    } else if (room_content == TROLL &&
               random_number(4) == 1) { // 25% chance for crushing attack
      troll_crushing_attack(player, game, enemy_strength, enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == WOLF && random_number(4) == 1) { // 25% chance
      wolf_frenzy_attack(player, enemy_strength, enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == GARGOYLE &&
               random_number(4) == 1) { // 25% chance
      gargoyle_stone_form_attack(player, game, enemy_strength, enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == BEAR && random_number(4) == 1) { // 25% chance
      bear_maul_attack(player, game, enemy_strength, enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == OGRE && random_number(4) == 1) { // 25% chance
      ogre_rage_attack(player, game, enemy_strength, &enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == ORC && random_number(4) == 1) { // 25% chance
      orc_battle_fury_attack(player, game, enemy_strength, enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == GOBLIN && random_number(4) == 1) { // 25% chance
      goblin_dirty_tricks_attack(player, game, enemy_strength, enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == MIMIC && random_number(4) == 1) { // 25% chance
      mimic_special_attack(player, game, enemy_strength, &enemy_dexterity,
                           &enemy_intelligence);
      ;
      if (game->game_over) {
        return;
      }
    } else if (room_content == CHIMERA &&
               random_number(3) == 1) { // 33% chance for special attack
      chimera_attack(player, game, enemy_strength, enemy_dexterity,
                     enemy_intelligence);
      if (game->game_over) {
        return;
      }
    } else if (room_content == MINOTAUR &&
               random_number(4) == 1) { // 25% chance for charge attack
      minotaur_charge_attack(player, game, enemy_strength, &enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if (room_content == BALROG && random_number(5) == 1) {
      balrog_flame_whip_attack(player, game, enemy_strength, enemy_dexterity);
      if (game->game_over) {
        return;
      }
    } else if ((room_content == KOBOLD && random_number(4) == 1) ||
               (room_content == DRAGON && random_number(3) == 1) ||
               (room_content == TROLL &&
                random_number(2) ==
                    1) || // Trolls have 50% chance to regenerate if they don't
                          // use their special attack (making them pretty
                          // annoying to kill)
               (room_content == BALROG && random_number(5) == 1) ||
               (room_content == GOBLIN && random_number(5) == 1) ||
               (room_content == MIMIC && random_number(4) == 1))

    {
      switch (room_content) {
      case KOBOLD:
        max_increase = 3;
        spellcasted = random_number(7); // KOBOLDS CAN'T CAST DARKNESS
        break;
      case DRAGON:
        spellcasted = random_number(9);
        max_increase = 5;
        break;
      case GOBLIN:
        spellcasted = random_number(9);
        max_increase = 2;
        break;
      case BALROG:
        spellcasted = random_number(2) + 7; // Firebolt or Darkness
        max_increase = 2;
        break;

      case TROLL:
        max_increase = 5; // Trolls have strong regeneration
        spellcasted = 1;  // Trolls can only heal, not cast other spells
        break;
      case MIMIC:
        max_increase =
            3; // Mimics get smaller stat increases than dragons/balrogs
        spellcasted = random_number(9); // Full spell range like most monsters
      }
      switch (spellcasted) {
      case 1:
        temp = random_number(max_increase);
        switch (room_content) {
        case MIMIC:
          print_message("The mimic absorbs nearby material and gained %d "
                        "strength points\n",
                        temp);
          break;
        default:
          print_message_formatted(
              "The %s casts heal and gained %i strength points\n", enemy_name,
              temp);
          break;
        }
        enemy_strength += temp;
        break;
      case 2:
        temp = random_number(max_increase);
        switch (room_content) {
        case MIMIC:
          print_message(
              "The mimic liqueifies it form and gained %d dexterity points\n",
              temp);
          break;
        default:
          print_message_formatted(
              "THE %s CASTS HASTE AND GAINED %i DEXTERITY POINTS\n", enemy_name,
              temp);
          break;
        }

        enemy_dexterity += temp;
        break;
      case 3:
        temp = random_number(max_increase);
        switch (room_content) {
        case MIMIC:
          print_message("The mimic studies your movements with uncanny "
                        "precision, gaining %d intelligence points!\n",
                        temp);
          break;
        default:
          print_message_formatted(
              "THE %s CASTS BRIGHT AND GAINED %i INTELLIGENCE POINTS\n",
              enemy_name, temp);
          break;
        }

        enemy_intelligence += temp;
        break;
      case 4:
        switch (room_content) {
        case MIMIC:
          print_message("The mimic secretes a magical dampening substgance; ");
          break;
        default:
          print_message_formatted("THE %s CASTS SILENCE; ", enemy_name);
          break;
        }

        base_chance = 50 + (player->intelligence - enemy_intelligence) * 5;

        // Add a random factor (-10 to +10)
        random_factor = (rand() % 21) - 10;

        // Calculate final success chance
        success_chance = base_chance + random_factor;

        // Ensure the success chance is within a reasonable range (5 to 95)
        if (success_chance < 5)
          success_chance = 5;
        if (success_chance > 95)
          success_chance = 95;

        // Determine if the spell succeeds
        if (muted) {
          print_message("The spell hits but you are already muted so "
                        "it has no effect!\n");
        } else if (random_number(100) < success_chance) {
          print_message("You've been muted. You are now unable to "
                        "cast spells until the end of combat.\n");
          muted = 1;
        } else {
          print_message(
              "The spell failed. You successfully resisted the spell.\n");
          // Spell fails, can still cast; already checked if muted.
        }
        break;
      case 5:
        switch (room_content) {
        case MIMIC:
          print_message("The Mimic sprays digestive enzymes at you!\n");
          break;
        default:
          print_message_formatted("THE %s CASTS WEAKNESS!\n", enemy_name);
          break;
        }

        // Calculate avoidance chance based on player stats
        avoidance_chance =
            (player->intelligence * 2 + player->strength + player->dexterity) /
            4;

        // Add a random factor
        avoidance_chance += random_number(10) - 5; // -5 to +5 random adjustment

        // Ensure the avoidance chance is within a reasonable range (5% to 95%)
        if (avoidance_chance < 5)
          avoidance_chance = 5;
        if (avoidance_chance > 95)
          avoidance_chance = 95;

        if (random_number(100) < avoidance_chance) {
          switch (room_content) {
          case MIMIC:
            print_message("You successfully resist the digestive enzymes!\n");
            break;
          default:
            print_message("You successfully resist the weakness spell!\n");
            break;
          }

        } else {
          temp = random_number(max_increase);
          switch (room_content) {
          case MIMIC:
            print_message("The enzymes burn! You lose %d strength points!\n",
                          temp);
            break;
          default:
            print_message("The spell hits! You lose %d strength points!\n",
                          temp);
            break;
          }

          player->strength -= temp;

          if (player->strength <= 0) {
            player->strength = 0;
            print_message(
                "Your strength has been reduced to zero.  You collapse...\n");
            game->game_over = 1;
            return;
          }
        }
        break;
      case 6:
        switch (room_content) {
        case MIMIC:
          print_message("The mimic exudes a sticky substance!\n");
          break;
        default:
          print_message_formatted("The %s casts ", enemy_name);
          print_message("Clumsy\n");
          break;
        }

        // Calculate avoidance chance based on player stats
        avoidance_chance =
            (player->intelligence * 2 + player->strength + player->dexterity) /
            4;

        // Add a random factor
        avoidance_chance += random_number(10) - 5; // -5 to +5 random adjustment

        // Ensure the avoidance chance is within a reasonable range (5% to 95%)
        if (avoidance_chance < 5)
          avoidance_chance = 5;
        if (avoidance_chance > 95)
          avoidance_chance = 95;

        if (random_number(100) < avoidance_chance) {
          switch (room_content) {
          case MIMIC:
            print_message("You avoid getting stuck in the secretion!\n");
            break;
          default:
            print_message("You resist the Clumsy spell with ease!\n");
            break;
          }
        } else {
          temp = random_number(max_increase);
          switch (room_content) {
          case MIMIC:
            print_message("You are sticky! You lose %d dexterity points.\n",
                          temp);
            break;
          default:
            print_message(
                "The spell strikes you! You lose %d dexterity points.\n", temp);
            break;
          }

          player->dexterity -= temp;

          if (player->dexterity <= 0) {
            player->dexterity = 0;
            print_message(
                "Your dexterity has been sapped to nothing. You collapse...\n");
            game->game_over = 1;
            return;
          }
        }
        break;
      case 7:
        switch (room_content) {
        case MIMIC:
          print_message("The mimic rapidly shifts between confusing forms!\n");
          break;
        default:
          print_message_formatted(
              "The %s casts Mind Fog, attemping to cloud your thoughts!\n",
              enemy_name);
          break;
        }

        // Calculate avoidance chance based on player stats
        avoidance_chance =
            (player->intelligence * 2 + player->strength + player->dexterity) /
            4;

        // Add a random factor
        avoidance_chance += random_number(10) - 5; // -5 to +5 random adjustment

        // Ensure the avoidance chance is within a reasonable range (5% to 95%)
        if (avoidance_chance < 5)
          avoidance_chance = 5;
        if (avoidance_chance > 95)
          avoidance_chance = 95;

        if (random_number(100) < avoidance_chance) {
          switch (room_content) {
          case MIMIC:
            print_message(
                "The mimic rapidly shifts between confusing forms!\n");
            break;
          default:
            print_message("You successfully resist the Mind Fog spell!\n");
            break;
          }

        } else {
          temp = random_number(max_increase);
          switch (room_content) {
          case MIMIC:
            print_message("The shifting forms disorient you! You lose %d "
                          "intelligence points.\n",
                          temp);
            break;
          default:
            print_message(
                "The spell strikes you! You lose %d intelligence points.\n",
                temp);
            break;
          }

          player->intelligence -= temp;

          if (player->intelligence <= 0) {
            player->intelligence = 0;
            switch (room_content) {
            case MIMIC:
              print_message(
                  "Your mind has been overwhelmed. You collapse...\n");
              break;
            default:
              print_message("Your intellect has been drained to nothing. You "
                            "collapse...\n");
              break;
            }

            game->game_over = 1;
            return;
          }
        }
        break;
      case 8:
        switch (room_content) {
        case MIMIC:
          print_message("The mimic sprays a cloud of dark ink!\n");
          break;
        default:
          print_message("The %s casts a veil of darkness upon you!\n",
                        enemy_name);
          break;
        }

        // Calculate avoidance chance based on player stats
        avoidance_chance =
            (player->intelligence * 2 + player->strength + player->dexterity) /
            4;

        // Add a random factor
        avoidance_chance += random_number(10) - 5; // -5 to +5 random adjustment

        // Ensure the avoidance chance is within a reasonable range (5% to 95%)
        if (avoidance_chance < 5)
          avoidance_chance = 5;
        if (avoidance_chance > 95)
          avoidance_chance = 95;

        if (random_number(100) < avoidance_chance) {
          switch (room_content) {
          case MIMIC:
            print_message("You dodge away from the ink cloud!\n");
            break;
          default:
            print_message("You stand firm and resist the darkness!\n");
            break;
          }

        } else {
          if (player->blindness_flag == 0) {
            switch (room_content) {
            case MIMIC:
              print_message("The ink gets in your eyes! You are now blind!\n");
              break;
            default:
              print_message("The spell strikes true! You are now blind!");
              break;
            }

            if (game->treasure[OPAL_EYE_INDEX]) {
              print_message("The Opal Eye immediately cures your blindness!\n");
              player->temp_blindness_flag = 0;
              player->blindness_flag = 0;
            } else {
              player->temp_blindness_flag = 1;
              player->blindness_flag = 1;
            }
          } else {
            switch (room_content) {
            case MIMIC:
              print_message(
                  "The ink gets in your eyes! You were already blind!  You "
                  "wipe your eyes and continue fighting!\n");
              break;
            default:
              print_message("The spell hits, but you are already blind, so it "
                            "has no effect!\n");
              break;
            }
          }
        }
        break;
      case 9:
        switch (room_content) {
        case MIMIC:
          print_message("The mimic sprays corrosive acid!\n");
          break;
        case BALROG:
          print_message("The Balrog opens its fiery mouth and a firebolt "
                        "shoots out of it.\n");
          break;

        default:
          print_message("The %s conjures a blazing firebolt!\n", enemy_name);
          break;
        }

        // Calculate avoidance chance based on player stats
        avoidance_chance =
            (player->intelligence * 2 + player->strength + player->dexterity) /
            4;

        // Add a random factor
        avoidance_chance += random_number(10) - 5; // -5 to +5 random adjustment

        // Ensure the avoidance chance is within a reasonable range (5% to 95%)
        if (avoidance_chance < 5)
          avoidance_chance = 5;
        if (avoidance_chance > 95)
          avoidance_chance = 95;

        if (random_number(100) < avoidance_chance) {
          switch (room_content) {
          case MIMIC:
            print_message("You dodge the acid spray!\n");
            break;
          case BALROG:
            print_message("Sensing the firebolt, you quickly dodge it.\n");
            break;
          default:
            print_message("You nimbly avoid the %ss firebolt!\n", enemy_name);
            break;
          }
        } else {
          temp = random_number(max_increase);
          print_message("The firebolt hits you\n");
          if (temp - player->armor_type < 0) {
            temp = 0;
          }
          player->armor_points -= temp;
          if (player->armor_points <= 0) {
            player->strength += player->armor_points;
            print_message("You took %i damage.\n", player->armor_points * -1);
            player->armor_points = 0;
            switch (room_content) {
            case MIMIC:
              print_message(
                  "The acid melted your armor.  Good luck adventurer!\n");
              break;
            default:
              print_message("Your armor was destroyed, good luck adventurer.");
              break;
            }

            player->armor_type = 0;
          } else {
            print_message(
                "Your armor absorbed the blow but you lost %d armor points.\n",
                temp);
          }

          if (player->strength <= 0) {
            player->strength = 0;
            switch (room_content) {
            case MIMIC:
              print_message("The acid has dissolved you.  You have died and "
                            "not even a skeleton to show for it.\n");
              break;
            default:
              print_message(
                  "The firebolt incinerates you.  You have died ...\n");
              break;
            }
            game->game_over = 1;
            return;
          }
        }
        break;
      }
    } else if (enemy_attack_hits(player, enemy_dexterity)) {
      switch (room_content) {
      case KOBOLD:
        print_message("The Kobold lunges at you with surprising speed, its "
                      "tiny claws slashing viciously!\n");
        break;
      case ORC:
        print_message("The Orc charges at you with a brutal roar, swinging its "
                      "weapon viciously!\n");
        break;
      case WOLF:
        print_message("The Wolf snarls and lunges at you, its sharp fangs "
                      "snapping fiercely!\n");
        break;
      case GOBLIN:
        print_message("The Goblin cackles wickedly and darts forward, its "
                      "crude weapon aimed at you with malicious intent!\n");
        break;
      case OGRE:
        print_message("The Ogre bellows with rage, swinging its massive club "
                      "towards you with bone-crushing force!\n");
        break;
      case TROLL:
        print_message("The Ogre bellows with rage, swinging its massive club "
                      "towards you with bone-crushing force!\n");
        break;
      case BEAR:
        print_message(
            "The Bear growls menacingly, rearing up on its hind legs before "
            "crashing down with powerful claws swiping towards you!\n");
        break;
      case MINOTAUR:
        print_message(
            "The Minotaur swings its massive axe with deadly precision!\n");
        break;
      case GARGOYLE:
        print_message(
            "The Gargoyle screeches with an ear-piercing cry, its stone claws "
            "slashing through the air with deadly intent!\n");
        break;
      case CHIMERA:
        print_message("The Chimera roars with its three heads, each one "
                      "striking at you with deadly precision!\n");
        break;
      case BALROG:
        print_message(
            "The Balrog's massive fist crashes down with immense force!\n");
        break;
      case VENDOR:
        print_message("The vendor, enraged, pulls out a dagger and lunges at "
                      "you with unexpected skill!\n");
        break;
      case DRAGON:
        print_message("The dragon lunges at you with a ferocious attack!\n");
        break;
      case MIMIC:
        print_message("The mimic lunges at you with its dagger like teeth!\n");
        break;
      default:
        print_message("The creature lunges at you with a ferocious attack!\n");
        break;
      }
      print_message("\nOuch! He hits you!\n");
      int damage =
          calculate_damage_enemy(player, enemy_strength, enemy_dexterity,
                                 (room_content - MONSTER_START) / 3 + 1);

      player->armor_points -= damage;
      if (player->armor_type > 0 && player->armor_points <= 0) {
        player->armor_points = 0;
        player->armor_type = 0;
        print_message("\nYour armor has been shattered... good luck surviving "
                      "the onslaught!\n");
      }

      player->strength -= damage;
      print_message("You take %d damage!\n", damage);

      if (player->strength <= 0) {
        print_message("\nYour strength has failed you... You collapse and "
                      "succumb to your wounds.\n");

        game->game_over = 1;
        return;
      }
    } else {
      print_message_formatted("THE %s ATTACKS\n", enemy_name);
      print_message("\nWhat luck, he missed you!\n");
    }
    // Catch all for enemy deaths for one offs
    if (enemy_intelligence <= 0) {
      print_message_formatted("The %s", enemy_name);
      print_message(
          " collapses, succumbing to its own lack of intelligence.\n");
      return;
    }
    if (enemy_strength <= 0) {
      print_message("The %s", enemy_name);
      print_message(" has succumbed to its own weakness and perishes.\n");

      return;
    }
    if (enemy_dexterity <= 0) {
      print_message_formatted("The %s", enemy_name);
      print_message(
          " falters and collapses, its lack of dexterity its undoing.\n");

      return;
    }
  }
}

void handle_combat_victory(Player *player, GameState *game, int is_vendor,
                           const char *enemy_name) {
  print_message_formatted("\n%s LIES DEAD AT YOUR FEET!\n", enemy_name);
  int room_content =
      get_room_content(game, player->x, player->y, player->level);

  if (random_number(5) == 1) { // 20% chance of eating
    print_message_formatted("\nYOU SPEND AN HOUR EATING %s%s.\n", enemy_name,
                            get_random_body_part());
  }

  if (is_vendor) {
    print_message("\nYou get all his wares:\n");
    print_message("Plate armor\n");

    if (player->armor_type < PLATE) {
      player->armor_type = PLATE;
      player->armor_points = MAX_ARMOR_POINTS;
    } else {
      print_message("You discard the leather armor.\n");
    }

    print_message("A sword\n");
    if (player->weapon_type <
        SWORD) // A Dwarf might want to keep their mace but no logic for that.
    {
      player->weapon_type = SWORD;
    } else {
      print_message("You discard the sword.\n");
    }

    print_message("A strength potion\n");
    player->strength =
        get_minimum(player->strength + random_number(6), MAX_STRENGTH);
    print_message("An intelligence potion\n");
    player->intelligence =
        get_minimum(player->intelligence + random_number(6), MAX_INTELLIGENCE);
    print_message("A dexterity potion\n");
    player->dexterity =
        get_minimum(player->dexterity + random_number(6), MAX_DEXTERITY);
    if (!player->lamp_flag) {
      print_message("A lamp\n");
      player->lamp_flag = 1;
    }
  } else {
    // Check if this was the room with the Runestaff
    if (player->x == game->runestaff_location[0] &&
        player->y == game->runestaff_location[1] &&
        player->level == game->runestaff_location[2]) {
      print_message("\nGreat zot! You've found the runestaff!\n");
      player->runestaff_flag = 1;
      game->runestaff_location[0] = 0; // Mark as found
    }
  }

  int gold_found =
      random_number(250) + random_number(250) + random_number(250) +
      random_number(250); // 4 to 1000 but higher chance of being a good number.
  if (room_content == DRAGON) {
    gold_found += random_number(250) + random_number(250) + random_number(250) +
                  random_number(250); // Dragons hoard gold
  } else if (room_content == KOBOLD) {
    gold_found += random_number(250); // Kobolds have a litte more chance to
                                      // have money since they are small dragons
  } else if (room_content == BALROG) {
    gold_found +=
        random_number(250) + random_number(250) +
        random_number(
            250); // Balrogs are so hard, they should have more treasure
  }

  print_message("\nYou now get his hoard of %d GP's!\n", gold_found);
  player->gold += gold_found;

  if (random_number(5) == 1 || room_content == DRAGON || is_vendor) {
    int flares_found = random_number(6);
    print_message("\nYou also found his hoard of %d flares!\n", flares_found);
    player->flares += flares_found;
  }

  if ((random_number(20) == 1 || room_content == DRAGON) && !is_vendor) {
    print_message_formatted(
        "\nThe %s was also hoarding a lamp.  It is now yours.\n", enemy_name);
    player->lamp_flag = 1;
  }

  if (room_content == DRAGON) {
    print_message("The dragon was also hoarding a strength potion, an "
                  "intelligence potion and a dexterity potion.\n\n");
    player->strength =
        get_minimum(player->strength + random_number(6), MAX_STRENGTH);
    player->intelligence =
        get_minimum(player->intelligence + random_number(6), MAX_INTELLIGENCE);
    player->dexterity =
        get_minimum(player->dexterity + random_number(6), MAX_DEXTERITY);
  } else {
    if (random_number(8) == 1) {
      print_message_formatted("The %s was also hoarding a strength potion.\n",
                              enemy_name);
      player->strength =
          get_minimum(player->strength + random_number(6), MAX_STRENGTH);
    }
    if (random_number(8) == 1) {
      print_message_formatted(
          "The %s was also hoarding a intelligence potion.\n", enemy_name);
      player->intelligence = get_minimum(
          player->intelligence + random_number(6), MAX_INTELLIGENCE);
    }
    if (random_number(8) == 1) {
      print_message_formatted("The %s was also hoarding a dexterity potion.\n",
                              enemy_name);
      player->dexterity =
          get_minimum(player->dexterity + random_number(6), MAX_DEXTERITY);
    }
  }
  if (should_drop_weapon(room_content)) {
    print_message_formatted("In the %s's den, you find ", enemy_name);

    switch (get_monster_weapon_drop(room_content)) {
    case EXCALIBUR:
      print_message("Excalibur.\n");
      if (player->weapon_type < EXCALIBUR) // A Dwarf might want to keep their
                                           // mace but no logic for that.
      {
        player->weapon_type = EXCALIBUR;
      } else {
        print_message("You discard Excalibur.\n");
      }
      break;
    case SWORD:
      print_message("a sword.\n");
      if (player->weapon_type < SWORD) {
        player->weapon_type = SWORD;
      } else {
        print_message("You discard the sword.\n");
      }
      break;
    case MACE:
      print_message("a mace.\n");
      if (player->weapon_type < MACE) {
        player->weapon_type = MACE;
      } else {
        print_message("You discard the mace.\n");
      }
      break;

    case DAGGER:
      print_message("a dagger.\n");
      if (player->weapon_type < DAGGER) {
        player->weapon_type = DAGGER;
      } else {
        print_message("You discard the dagger.\n");
      }
      break;
    }
  }
  if (should_drop_armor(room_content)) {
    print_message_formatted("In the %s's den, you find ", enemy_name);
    player->armor_points = MAX_ARMOR_POINTS;

    switch (get_monster_armor_drop(room_content)) {
    case STONE:
      print_message("Stone armor.\n");
      if (player->armor_type <
          STONE) // A Dwarf might want to keep their mace but no logic for that.
      {
        player->armor_type = STONE;
      } else {
        print_message("You discard the stone armor.\n");
      }
      break;
    case PLATE:
      print_message("plate armor.\n");
      if (player->armor_type < PLATE) {
        player->armor_type = PLATE;
      } else {
        print_message("You discard the plate armor.\n");
      }
      break;
    case CHAINMAIL:
      print_message("chainmail.\n");
      if (player->armor_type < CHAINMAIL) {
        player->armor_type = CHAINMAIL;
      } else {
        print_message("You discard the chainmail armor.\n");
      }
      break;

    case LEATHER:
      print_message("leather armor.\n");
      if (player->armor_type < LEATHER) {
        player->armor_type = LEATHER;
      } else {
        print_message("You discard the leather armor.\n");
      }
      break;
    }
  }

  // Clear the room
  set_room_content(game, player->x, player->y, player->level, EMPTY_ROOM);
}

int handle_bribe(Player *player, GameState *game, const char *enemy_name) {
  if (player->treasure_count == 0) {
    print_message("\nAll I want is your life!\n");
    return 0;
  }

  int treasure_index = random_number(TREASURE_COUNT) - 1;
  while (!game->treasure[treasure_index]) {
    treasure_index = random_number(TREASURE_COUNT) - 1;
  }

  print_message_formatted("\nI WANT %s. WILL YOU GIVE IT TO ME? ",
                          get_treasure_name(treasure_index));
  char choice = get_user_input_yn();

  if (choice == 'Y') {
    game->treasure[treasure_index] = 0;
    player->treasure_count--;
    print_message("\nOk, just don't tell anyone else.\n");
    if (strcmp(enemy_name, "VENDOR") == 0) {
      game->vendor_attacked = 0; // Vendor will now trade
    }
    return 1;
  }
  return 0;
}

int handle_spell(Player *player, GameState *game, int *enemy_strength,
                 int *enemy_intelligence, int *enemy_dexterity,
                 const char *enemy_name) {
  int damage;
  print_message("\nWhich spell\n");
  if (player->intelligence >= 14) {
    print_message("    (W)EB - Casts a magical web that prevents a monster "
                  "from attacking (guaranteed to hit every time)\n");
    print_message("    (F)IREBALL - Casts a Fireball at the enemy (guaranteed "
                  "to hit every time)\n");
  }
  if (player->intelligence >= 14 &&
      (player->race == DWARF || player->race == DROW)) {
    print_message("   S(T)ONE SKIN; improves your armor at the cost of one "
                  "strength and intelligence points\n");
  }
  if (player->intelligence >= 16) {
    print_message(
        "    (D)EATHSPELL - Casts a Deathspell; be warned you may die\n");
  }
  if ((player->race == ELF || player->race == DROW || player->race == DWARF) &&
      player->intelligence >= 10) {
    print_message("    (H)EAL - Permanently heals you (maximum %d strength "
                  "after combat); costs one intelligence point\n",
                  MAX_STRENGTH);
    print_message("    (S)PEED - Temporarily increases your dexterity\n");
    print_message("    (B)RIGHT - Temporarily increases your intelligence \n");
  }
  if ((player->race == HOBBIT) && player->intelligence >= 11) {
    print_message("    (M)ischief Hobbit Magic with Chaos Effect\n");
  }
  if ((player->race == HUMAN) && player->intelligence >= 12) {
    print_message("    (P)aladin Strike - Channel divine energy into a "
                  "powerful magical attack\n\n");
  }

  print_message("\n");
  for (;;) {
    char spell = get_user_input_custom_prompt("Which Spell:  ");
    switch (spell) {
    case 'W':
      if (player->intelligence >= 14) {
        if (player->strength > 10) {
          player->strength--;
        }
        if (player->strength <= 0) {
          game->game_over = 1;
          return 1;
        }
        player->web_count = random_number(9); // Set web count to 1-9 turns
        print_message_formatted(
            "\nTHE %s IS STUCK AND CAN'T ATTACK FOR %d TURNS!\n", enemy_name,
            player->web_count);
        return 0;
      } else {
        print_message("WEB . . . INVALID CHOICE\n");
      }
      break;
    case 'F':
      if (player->intelligence >= 14) {
        if (player->strength > 10) {
          player->strength--;
        }
        player->intelligence--;
        if (player->strength <= 0 || player->intelligence <= 0) {
          game->game_over = 1;
          return 1;
        }
        damage = random_number(7) + random_number(7);
        print_message("\nIt does %d points worth of damage\n", damage);
        *enemy_strength -= damage;
        if (*enemy_strength <= 0) {
          handle_combat_victory(player, game, 0, enemy_name);
          return 1;
        }
        return 0;
      } else {
        print_message("FIREBALL . . . INVALID CHOICE\n");
      }
      break;
    case 'D':
      if (player->intelligence >= 16) {
        print_message("\nDeath . . . ");
        if (calculate_death_spell(player->intelligence, player->strength,
                                  player->dexterity, *enemy_intelligence,
                                  *enemy_strength, *enemy_dexterity)) {
          print_message("Yours!\n");
          player->intelligence = 0;
          game->game_over = 1;
          return 1;
        } else {
          print_message("His!\n");
          // handle_combat_victory(player, game, 0, enemy_name)
          *enemy_intelligence = 0;
          *enemy_strength = 0;
          *enemy_dexterity = 0;
          return 1;
        }
      } else {
        print_message("Death . . . Invalid choice\n");
      }
      break;
    case 'H':
      if ((player->race == ELF || player->race == DROW ||
           player->race == DWARF) &&
          player->intelligence >= 10) {
        cast_heal_spell(player);
        return 0;
      }
      print_message("\n** TRY ONE OF THE OPTIONS GIVEN.\n");
      break;
    case 'S':
      if ((player->race == ELF || player->race == DROW ||
           player->race == DWARF) &&
          player->intelligence >= 10) {
        cast_haste_spell(player);
        return 0;
      }
      print_message("\n** Try one of the options given.\n");
      break;
    case 'B':
      if ((player->race == ELF || player->race == DROW ||
           player->race == DWARF) &&
          player->intelligence >= 10) {
        cast_bright_spell(player);
        return 0;
      }
      print_message("\n** Try one of the options given.\n");
      break;
    case 'T':
      if ((player->race == DWARF || player->race == DROW) &&
          player->intelligence >= 14) {
        cast_stone_skin_spell(player);
        return 0;
      }
      print_message("\n** Try one of the options given.\n");
      break;
    case 'M': // Mischief Blast for Hobbits
      if (player->race == HOBBIT && player->intelligence >= 11) {
        cast_mischief_blast(player, enemy_strength, enemy_dexterity,
                            enemy_intelligence);
        player->intelligence -= 1;

        if (*enemy_strength <= 0) {
          print_message("Your chaotic magic overwhelms the enemy!  "
                        "They have died from lack of strength!\n");
          return 1;
        }
        if (*enemy_dexterity <= 0) {
          print_message("Your chaotic magic overwhelms the enemy!  "
                        "They have died from lack of dexterity\n");
          return 1;
        }
        if (*enemy_intelligence <= 0) {
          print_message("Your chaotic magic overwhelms the enemy!  "
                        "They have died from lack of intelligence\n");
          return 1;
        }

        return 0;
      }
      print_message("\n** Try one of the options given.\n");
      break;
    case 'P': // Paladin Strike for Humans
      if (player->race == HUMAN && player->intelligence >= 12) {
        damage = random_number(6) + random_number(6); // 2-12 base damage

        // Bonus damage based on strength
        damage +=
            player->strength / 4; // Additional 0-4 damage based on strength

        print_message("\nDivine energy surges through your weapon!\n");

        if (random_number(6) == 6) { // Critical hit chance
          damage *= 2;
          print_message(
              "Critical Strike! The divine power courses through you!\n");
        }

        print_message("Your Paladin Strike deals %d damage!\n", damage);
        *enemy_strength -= damage;
        player->intelligence -= 1;

        if (*enemy_strength <= 0) {
          print_message("The divine power vanquishes your foe!\n");
          return 1;
        }
        return 0;
      }
      print_message("\n** Try one of the options given.\n");
      break;
    default:
      print_message("\n** Try one of the options given.\n");
    }
  }
}

void dragon_fireball_attack(Player *player, GameState *game, int enemy_strength,
                            int enemy_dexterity, int enemy_intelligence) {
  print_message("\nThe dragon breathes a massive fireball at you!\n");

  // Calculate hit chance using enemy stats
  int hit_chance = 50;
  hit_chance += (enemy_dexterity - 10) * 2; // Dexterity bonus
  hit_chance +=
      (enemy_intelligence - 10); // Intelligence bonus for magical attack
  hit_chance -= (player->dexterity - 10) * 2; // Player's dodge chance
  hit_chance += 15 * player->blindness_flag;  // Blindness penalty

  // Ensure hit chance is within reasonable bounds
  if (hit_chance < 10)
    hit_chance = 10; // Dragons are more accurate with fireballs
  if (hit_chance > 90)
    hit_chance = 90; // Always a chance to dodge

  // Avoidance check
  if (random_number(100) > hit_chance) {
    print_message("You manage to dodge the scorching flames!\n");
    return;
  }

  // Base damage calculation
  int damage = random_number(5) + 1; // 2-6 base damage

  // Add dragon's strength and intelligence bonuses
  damage += enemy_strength / 9;     // Strength bonus like normal attacks
  damage += enemy_intelligence / 6; // Intelligence bonus for magical attack

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  print_message("The fireball hits you for %d initial damage!\n", damage);

  // Apply armor reduction if player has armor
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type + random_number(3) - 1;

    if (player->armor_type == STONE) {
      damage -= (armor_protection * 3);
      player->armor_points -= (armor_protection);

    } else {
      damage -= (armor_protection * 2);
      player->armor_points -=
          (armor_protection * 2); // Double armor damage from fire
    }

    if (damage < 0) {
      damage = 0;
      if (player->armor_type == STONE) {
        print_message("Your armor protects you; absorbing the fireball!\n");
      } else {
        print_message("Your armor protects you, absorbing damage but "
                      "taking extra wear!\n");
      }

    } else {
      print_message("Your armor partially protects you, absorbing %d "
                    "damage but taking extra wear!\n",
                    armor_protection);
    }

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message("Your armor has been melted by the Dragon's fire!\n");
    }
  }

  player->strength -= damage;
  print_message("You take %d final damage from the fireball!\n", damage);

  if (player->strength <= 0) {
    print_message("\nYou have been incinerated by the Dragon's fire! "
                  "Your adventure ends here.\n");
    game->game_over = 1;
  }
}

void balrog_flame_whip_attack(Player *player, GameState *game,
                              int enemy_strength, int enemy_dexterity) {
  print_message("\nThe Balrog lashes out with its flame whip!\n");

  // Dexterity-based avoidance check using enemy stats
  int hit_chance = 50;
  hit_chance += (enemy_dexterity - 10) * 2;   // Dexterity bonus
  hit_chance += (enemy_strength - 10);        // Strength bonus for whip control
  hit_chance -= (player->dexterity - 10) * 2; // Player's dodge chance
  hit_chance += 15 * player->blindness_flag;  // Blindness penalty

  // Ensure hit chance is within reasonable bounds
  if (hit_chance < 10)
    hit_chance = 10; // Balrog minimum accuracy
  if (hit_chance > 90)
    hit_chance = 90; // Always a chance to dodge

  if (random_number(100) > hit_chance) {
    print_message("You narrowly avoid the searing flames!\n");
    return;
  }

  // Base damage calculation
  int damage = random_number(5) + 1; // 1-6 base damage

  // Add Balrog's strength bonus
  damage += enemy_strength / 9; // Strength bonus like normal attacks

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  print_message("The flame whip strikes you for %d initial damage!\n", damage);

  // Apply armor reduction if player has armor
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type + random_number(3) - 1;
    damage -= armor_protection;
    if (player->armor_type == STONE) {
      player->armor_points -= (armor_protection); // STONE ARMOR IS STRONG
    } else {
      player->armor_points -=
          (armor_protection * 2); // Double armor damage from fire
    }
    if (damage < 0) {
      player->armor_points += damage; // Adjust for overkill protection
      damage = 0;
    }

    print_message("Your armor absorbs %d damage but is severely damaged!\n",
                  armor_protection);

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message("Your armor has been melted by the Balrog's flame whip!\n");
    }
  }

  player->strength -= damage;
  print_message("You take %d final damage from the flame whip!\n", damage);

  if (player->strength <= 0) {
    print_message("\nYou have been incinerated by the Balrog's flame whip!\n");
    game->game_over = 1;
  }
}

const char *get_monster_name(int room_content) {
  switch (room_content) {
  case KOBOLD:
    return "KOBOLD";
  case ORC:
    return "ORC";
  case WOLF:
    return "WOLF";
  case GOBLIN:
    return "GOBLIN";
  case OGRE:
    return "OGRE";
  case TROLL:
    return "TROLL";
  case BEAR:
    return "BEAR";
  case MINOTAUR:
    return "MINOTAUR";
  case GARGOYLE:
    return "GARGOYLE";
  case CHIMERA:
    return "CHIMERA";
  case BALROG:
    return "BALROG";
  case DRAGON:
    return "DRAGON";
  case MIMIC:
    return "MIMIC";
  default:
    return "UNKNOWN MONSTER";
  }
}

int cast_heal_spell(Player *player) {
  if (player->intelligence > 9) {
    int heal_amount = random_number(5); // 1-5 Points
    player->strength += heal_amount;
    player->intelligence -= 1;
    print_message("Your health increased by %d points.\n\n", heal_amount);
    return 1;
  }
  return 0;
}

int cast_stone_skin_spell(Player *player) {
  if (player->intelligence >= 14) {
    if (player->armor_type < STONE) {
      player->armor_type += 1;
      player->intelligence -= 1;
      player->strength -= 1;
      player->armor_points = MAX_ARMOR_POINTS;
      if (player->armor_type == 0) {
        print_message("Your armor has improved!!!");
      } else {
        print_message("Magical armor has now appeared.");
      }
    } else {
      print_message(
          "You can't improve your armor but armor points have been restored.");
    }
    player->armor_points = (player->armor_points += 10) > MAX_ARMOR_POINTS
                               ? MAX_ARMOR_POINTS
                               : player->armor_points;
    return 1;
  } else {
    return 0;
  }
}

int cast_bright_spell(Player *player) {
  if (player->intelligence > 9) {
    int bright_amount =
        random_number(5) + 1; // Increase intelligence by 2-6 points
    if (player->temp_intelligence == 0) {
      player->temp_intelligence = player->intelligence;
    }
    player->intelligence += bright_amount;
    if (player->intelligence >= MAX_INTELLIGENCE) {
      player->intelligence = MAX_INTELLIGENCE;
      print_message_formatted(
          "Your intelligence temporarily increased to %d points.\n\n",
          MAX_INTELLIGENCE);
    } else {
      print_message_formatted(
          "Your intelligence temporarily increased by %d points.\n\n",
          bright_amount);
    }
    return 1;
  }
  return 0;
}

int cast_haste_spell(Player *player) {
  if (player->intelligence > 9) {
    int haste_amount = random_number(5) + 1; // Increase dexterity by 2-6 points
    if (player->temp_dexterity == 0) {
      player->temp_dexterity = player->dexterity;
    }
    player->dexterity += haste_amount;
    if (player->dexterity >= MAX_DEXTERITY) {
      print_message_formatted("Your dexterity temporarily increasd to %d.\n\n",
                              MAX_DEXTERITY);
    } else {
      print_message_formatted(
          "Your dexterity temporarily increased by %d points.\n\n",
          haste_amount);
    }
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
  if (success_chance < 5)
    success_chance = 5;
  if (success_chance > 95)
    success_chance = 95;

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
  int strength_bonus =
      player->strength / 9; // Every 9 points of strength adds 1 to damage
  int dexterity_bonus =
      player->dexterity / 6; // Every 6 points of dexterity adds 1 to damage

  // Enemy's defensive bonuses
  int enemy_strength_defense =
      enemy_strength /
      9; // Every 9 points of enemy strength reduces damage by 1
  int enemy_dexterity_defense =
      enemy_dexterity /
      6; // Every 5 points of enemy dexterity reduces damage by 1

  // Calculate initial damage
  int total_damage = base_damage + strength_bonus + dexterity_bonus;

  // Reduce damage based on enemy stats
  total_damage -= (enemy_strength_defense + enemy_dexterity_defense);

  // Add a random factor
  int random_factor = random_number(5) - 3; // -2 to +2 random adjustment
  total_damage += random_factor;

  // Critical hits
  switch (player->race) {
  case HOBBIT:
    if (random_number(10) == 1 && player->weapon_type > NOTHING) { // 10% chance
      print_message("Lucky Strike! You found a weak spot!\n");
      total_damage *= 2; // Double the damage
    } else if (player->weapon_type == DAGGER) {
      if (random_number(20) == 20) { // Natural 20 (5% chance)
        print_message(
            "Critical Strike! Your small size lets you strike a vital spot!\n");
        total_damage *= 2;
      }
      // Additional dagger bonus for Hobbits
      weapon_bonus = random_number(4); // d4 extra damage with daggers
      total_damage += weapon_bonus;
    }
    break;
  case HUMAN:
    if (random_number(4) == 1) {
      weapon_bonus = random_number(3) - 1; // 0-2 extra damage
      total_damage += weapon_bonus;
      if (weapon_bonus > 0) {
        print_message("Your weapon mastery deals %d extra damage!\n",
                      weapon_bonus);
      }
    } else if (player->weapon_type >= SWORD) {
      if (random_number(20) >= 15) {     // 30% chance
        weapon_bonus = random_number(6); // d6 extra damage
        total_damage += weapon_bonus;
        print_message("Your sword mastery deals %d extra damage!\n",
                      weapon_bonus);
      }
    }
    break;
  case ELF:
    if (player->weapon_type == SWORD) {
      weapon_bonus = random_number(4); // d4 extra damage
      total_damage += weapon_bonus;
      if (weapon_bonus > 0) {
        print_message("Your elven grace adds %d damage!\n", weapon_bonus);
      }
    } else if (player->weapon_type == EXCALIBUR) {
      weapon_bonus = random_number(8); // d8 extra damage with Excalibur
      total_damage += weapon_bonus;
      if (weapon_bonus > 0) {
        print_message(
            "Your elven magic enhances Excalibur for %d extra damage!\n",
            weapon_bonus);
      }
    } else if (random_number(20) >= 15) {
      weapon_bonus = random_number(2); // 1-2 extra damage
      total_damage += weapon_bonus;
      print_message("Your weapon mastery deals %d extra damage!\n",
                    weapon_bonus);
    }
    break;
  case DROW:
    // Drow are deadly with daggers and swords
    if (player->weapon_type == DAGGER || player->weapon_type == SWORD) {
      if (random_number(6) == 6) {       // ~17% chance
        weapon_bonus = random_number(4); // d4 extra damage
        total_damage += weapon_bonus;
        if (weapon_bonus > 0) {
          print_message("Your Drow combat training deals %d extra damage!\n",
                        weapon_bonus);
        }
      }
    } else if (random_number(20) >= 15) {
      weapon_bonus = random_number(2); // 1-2 extra damage
      total_damage += weapon_bonus;
      print_message("Your weapon mastery deals %d extra damage!\n",
                    weapon_bonus);
    }
    break;
  case DWARF:
    // Dwarves excel with maces and heavy weapons
    if (player->weapon_type == MACE) {
      weapon_bonus = random_number(4) + player->strength / 9;
      total_damage += weapon_bonus;
      if (weapon_bonus > 0) {
        print_message(
            "Using your might mace, your dwarven strength adds %d damage!\n",
            weapon_bonus);
      }
    } else if (random_number(20) >= 15) {
      weapon_bonus = random_number(2); // 1-2 extra damage
      total_damage += weapon_bonus;
      print_message("Your dwarven weapon mastery deals %d extra damage!\n",
                    weapon_bonus);
    }
    break;
  }

  if (total_damage <= 0) {
    random_factor = random_number(3);
    if (random_factor == 1) {
      total_damage = random_factor;
    } else {
      total_damage = 0;
    }
  }

  return total_damage;
}

int calculate_damage_enemy(Player *player, int enemy_strength,
                           int enemy_dexterity, int base_damage) {
  // Enemy's offensive bonuses
  int strength_bonus =
      enemy_strength / 9; // Every 9 points of strength adds 1 to damage
  int dexterity_bonus =
      enemy_dexterity / 6; // Every 5 points of dexterity adds 1 to damage

  // Player's defensive bonuses
  int player_strength_defense =
      player->strength /
      9; // Every 4 points of player strength reduces damage by 1
  int player_dexterity_defense =
      player->dexterity /
      6; // Every 5 points of player dexterity reduces damage by 1

  // Calculate initial damage
  int total_damage = base_damage + strength_bonus + dexterity_bonus;
  // Reduce damage based on player stats
  total_damage -= (player_strength_defense + player_dexterity_defense);
  // Add a random factor
  int random_factor = random_number(5) - 3; // -2 to +2 random adjustment
  total_damage += random_factor;

  // Apply armor reduction if player has armor
  if (player->armor_type != 0) {
    total_damage -= player->armor_type;
  }

  if (total_damage <= 0) {
    random_factor = random_number(3);
    if (random_factor == 1) {
      total_damage = random_factor;
    } else {
      total_damage = 0;
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
  if (hit_chance < 5)
    hit_chance = 5;
  if (hit_chance > 95)
    hit_chance = 95;

  // Generate a random number between 1 and 100
  int roll = random_number(100);

  // Return 1 if the attack hits, 0 if it misses
  return (roll <= hit_chance) ? 1 : 0;
}

int cast_mischief_blast(Player *player, int *enemy_strength,
                        int *enemy_dexterity, int *enemy_intelligence) {
  int blast_count = random_number(4); // 1-4 blasts
  int total_damage = 0;
  int damage;
  int temp;
  print_message("\nYou channel chaotic hobbit magic!\n");
  print_message("You unleash %d magical blasts!\n", blast_count);

  for (int i = 0; i < blast_count; i++) {
    damage = random_number(4); // 1-5 damage per blast

    // Each blast has a special effect chance
    if (random_number(5) >= 4) {
      switch (random_number(6)) {
      case 1:
        damage *= 2;
        print_message("Blast %d: BOOM! Double damage! (%d damage)\n", i + 1,
                      damage);
        break;
      case 2:
        temp = random_number(3);
        *enemy_dexterity -= temp;
        print_message(
            "Blast %d: ZAP! Enemy loses %d dexterity and takes %d damage\n",
            i + 1, temp, damage);
        break;
      case 3:
        temp = random_number(3);
        *enemy_intelligence -= temp;
        print_message(
            "Blast %d: PING! Enemy loses %d intelligence and takes %d damage\n",
            i + 1, temp, damage);
        break;
      case 4:
        if (player->intelligence < MAX_INTELLIGENCE) {
          temp = player->intelligence;
          player->intelligence =
              (player->intelligence += random_number(3)) > MAX_INTELLIGENCE
                  ? MAX_INTELLIGENCE
                  : player->intelligence;
          print_message("Blast %d: Oops! You hit yourself and gained "
                        "%d intelligence!!!\n",
                        i + 1, temp - player->intelligence);
        } else {
          print_message(
              "Blast %d: Oops! You hit yourself but it had no effect.\n", i + 1,
              damage);
        }
        break;
      case 5:
        if (player->strength < MAX_STRENGTH) {
          temp = player->strength;
          player->strength =
              (player->strength + random_number(3)) > MAX_STRENGTH
                  ? MAX_STRENGTH
                  : player->strength;
          print_message(
              "Blast %d: Oops! You hit yourself and gained %d strength!!!\n",
              i + 1, temp - player->strength);
        } else {
          print_message(
              "Blast %d: Oops! You hit yourself but it had no effect.\n", i + 1,
              damage);
        }
        break;
      case 6:
        if (player->dexterity < MAX_DEXTERITY) {
          temp = player->dexterity;
          player->dexterity =
              (player->dexterity += random_number(3)) > MAX_DEXTERITY
                  ? MAX_DEXTERITY
                  : player->dexterity;
          print_message(
              "Blast %d: Oops! You hit yourself and gained %d dexterity!!!\n",
              i + 1, temp - player->dexterity);
        } else {
          print_message(
              "Blast %d: Oops! You hit yourself but it had no effect.\n",
              i + 1);
        }

        break;
      }
    } else {
      print_message("Blast %d: Deals %d damage\n", i + 1, damage);
    }
    total_damage += damage;
  }

  print_message("\nTotal damage dealt: %d\n", total_damage);
  *enemy_strength -= total_damage;

  return total_damage; // Return total damage dealt for any additional
                       // processing
}

void troll_crushing_attack(Player *player, GameState *game, int enemy_strength,
                           int enemy_dexterity) {
  print_message("\nThe Troll swings its massive fists in a crushing attack!\n");

  // Dexterity-based avoidance check
  int hit_chance = 50;
  hit_chance += (enemy_dexterity - 10) * 2; // Dexterity bonus
  hit_chance += (enemy_strength - 10); // Strength bonus for powerful attack
  hit_chance -= (player->dexterity - 10) * 2; // Player's dodge chance
  hit_chance += 15 * player->blindness_flag;  // Blindness penalty

  // Trolls are strong but somewhat clumsy
  if (hit_chance < 10)
    hit_chance = 10;
  if (hit_chance > 85)
    hit_chance = 85;

  if (random_number(100) > hit_chance) {
    print_message("You manage to dodge the Troll's mighty swing!\n");
    return;
  }

  // Base damage calculation
  int damage = random_number(6) + 2; // 3-8 base damage

  // Add Troll's strength bonus
  damage += enemy_strength / 8; // Trolls get better strength bonus for crushing

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  print_message("The crushing blow hits you for %d initial damage!\n", damage);

  // Apply and damage armor if present
  if (player->armor_type != 0) {
    int armor_protection =
        player->armor_type + random_number(2); // 1-2 random bonus
    damage -= armor_protection;

    // Trolls damage armor more with crushing blows
    player->armor_points -=
        (armor_protection * 3); // Triple armor damage from crushing

    if (damage < 0) {
      player->armor_points += damage; // Adjust for overkill protection
      damage = 0;
    }

    print_message("Your armor absorbs %d damage but is severely dented!\n",
                  armor_protection);

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message("Your armor has been crushed and destroyed!  Good luck "
                    "adventurer.\n");
    }
  }

  if (damage <= 0) {
    print_message("Your defenses completely absorbed the attack!\n");
    return;
  }

  player->strength -= damage;
  print_message("You take %d final damage from the crushing attack!\n", damage);

  if (player->strength <= 0) {
    print_message("\nThe troll has crushed you into paste!\n");
    game->game_over = 1;
  }
}

void minotaur_charge_attack(Player *player, GameState *game, int enemy_strength,
                            int *enemy_dexterity) {
  print_message("\nThe Minotaur lowers its horns and charges at you!\n");

  // Charging is hard to dodge due to momentum
  int hit_chance = 60; // Higher base chance due to charging momentum
  hit_chance += (*enemy_dexterity - 10) * 2;
  hit_chance += (enemy_strength - 10); // Strength helps control the charge
  hit_chance -=
      (player->dexterity - 10) * 1; // Harder to dodge (only 1x multiplier)
  hit_chance += 15 * player->blindness_flag;

  if (hit_chance < 10)
    hit_chance = 10;
  if (hit_chance > 85)
    hit_chance = 85;

  if (random_number(100) > hit_chance) {
    print_message("You dive out of the way of the charging Minotaur!\n");
    print_message(
        "The Minotaur slams into the wall, briefly stunning itself!\n");
    *enemy_dexterity -= 1; // Minotaur is briefly less effective after a miss
    return;
  }

  // Charge damage calculation
  int damage = random_number(4) +
               random_number(4); // 2-8 base damage for a powerful charge

  // Add Minotaur's strength bonus
  damage += enemy_strength / 8; // Better strength bonus for charging

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  print_message("The charge hits you for %d initial damage!\n", damage);

  // Apply and damage armor if present
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type + random_number(2);
    damage -= armor_protection;

    // Charging does significant armor damage
    player->armor_points -= (armor_protection * 2);

    if (damage < 0) {
      player->armor_points += damage;
      damage = 0;
    }

    print_message("Your armor absorbs %d damage but is badly dented!\n",
                  armor_protection);

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message("Your armor has been shattered by the Minotaur's charge!  "
                    "Good luck adventurer.\n");
    }
  }

  if (damage <= 0) {
    print_message("Your defenses absorbed the charge attack!\n");
    return;
  }

  player->strength -= damage;
  print_message("You take %d final damage from the charge!\n", damage);

  // Chance to stun (temporarily reduce dexterity)
  if (random_number(3) == 1) {          // 33% chance to stun
    int stun_amount = random_number(3); // 1-3 dexterity reduction
    if (player->temp_dexterity == 0) {
      player->temp_dexterity = player->dexterity; // Store original dexterity
    }
    player->dexterity -= stun_amount;
    if (player->dexterity < 3)
      player->dexterity = 3; // Don't reduce below 3
    print_message(
        "The impact leaves you stunned! You lose %d dexterity points!\n",
        stun_amount);
  }

  if (player->strength <= 0) {
    print_message("\nThe Minotaur's charge has gored you to death!\n");
    game->game_over = 1;
  }
}

void chimera_attack(Player *player, GameState *game, int enemy_strength,
                    int enemy_dexterity, int enemy_intelligence) {
  // Randomly choose which head attacks
  int head = random_number(3);

  switch (head) {
  case 1: // Lion head - Rending attack
    print_message("\nThe Chimera's lion head lunges with razor-sharp fangs!\n");

    int hit_chance = 50;
    hit_chance += (enemy_dexterity - 10) * 2;
    hit_chance += (enemy_strength - 10);
    hit_chance -= (player->dexterity - 10) * 2;
    hit_chance += 15 * player->blindness_flag;

    if (hit_chance < 10)
      hit_chance = 10;
    if (hit_chance > 85)
      hit_chance = 85;

    if (random_number(100) > hit_chance) {
      print_message("You evade the lion's fierce bite!\n");
      return;
    }

    int damage = random_number(6) + random_number(6); // 2-12 damage
    damage += enemy_strength / 8;

    int player_defense = player->strength / 9 + player->dexterity / 6;
    damage -= player_defense;

    if (player->armor_type != 0) {
      int armor_protection = player->armor_type + random_number(2);
      damage -= armor_protection;
      player->armor_points -= armor_protection;

      if (damage < 0) {
        player->armor_points += damage;
        damage = 0;
      }

      print_message_formatted("Your armor absorbs %d damage!\n",
                              armor_protection);

      if (player->armor_points <= 0) {
        player->armor_points = 0;
        player->armor_type = 0;
        print_message_formatted("YOUR ARMOR HAS BEEN TORN APART!\n");
      }
    }

    if (damage <= 0) {
      print_message_formatted("Your defenses absorbed the attack!\n");
      return;
    }

    player->strength -= damage;
    print_message_formatted("The lion's bite deals %d damage!\n", damage);
    break;

  case 2: // Dragon/Goat head - Fire breath
    print_message_formatted("\nThe Chimera's dragon head breathes fire!\n");

    hit_chance = 50;
    hit_chance += (enemy_dexterity - 10) * 2;
    hit_chance += (enemy_intelligence - 10);
    hit_chance -= (player->dexterity - 10) * 2;
    hit_chance += 15 * player->blindness_flag;

    if (hit_chance < 10)
      hit_chance = 10;
    if (hit_chance > 85)
      hit_chance = 85;

    if (random_number(100) > hit_chance) {
      print_message_formatted("You dodge the gout of flame!\n");
      return;
    }

    damage =
        random_number(4) + random_number(4); // 2-8 damage (weaker than dragon)
    damage += enemy_intelligence / 8;

    player_defense = player->strength / 9 + player->dexterity / 6;
    damage -= player_defense;

    if (player->armor_type != 0) {
      int armor_protection = player->armor_type; // No random bonus vs fire
      damage -= armor_protection / 2;            // Armor less effective vs fire
      player->armor_points -= armor_protection * 2; // Fire damages armor more

      if (damage < 0) {
        player->armor_points += damage;
        damage = 0;
      }

      print_message_formatted("Your armor partially protects against the "
                              "flames but takes extra damage!\n");

      if (player->armor_points <= 0) {
        player->armor_points = 0;
        player->armor_type = 0;
        print_message_formatted("YOUR ARMOR HAS BEEN MELTED!\n");
      }
    }

    if (damage <= 0) {
      print_message_formatted("Your defenses absorbed the flames!\n");
      return;
    }

    player->strength -= damage;
    print_message_formatted("The flames deal %d damage!\n", damage);
    break;

  case 3: // Snake head - Poison bite
    print_message_formatted(
        "\nThe Chimera's serpent head strikes with venomous fangs!\n");

    hit_chance = 50;
    hit_chance += (enemy_dexterity - 10) * 2;
    hit_chance -= (player->dexterity - 10) * 2;
    hit_chance += 15 * player->blindness_flag;

    if (hit_chance < 10)
      hit_chance = 10;
    if (hit_chance > 90)
      hit_chance = 90; // Snake head more accurate

    if (random_number(100) > hit_chance) {
      print_message_formatted("You avoid the serpent's strike!\n");
      return;
    }

    damage = random_number(4); // 1-4 initial damage

    player_defense = player->strength / 9 + player->dexterity / 6;
    damage -= player_defense;

    if (player->armor_type != 0) {
      int armor_protection = player->armor_type + random_number(2);
      damage -= armor_protection;
      player->armor_points -= 1; // Minimal armor damage from bite

      if (damage < 0) {
        damage = 0;
      }

      print_message_formatted("Your armor absorbs %d damage!\n",
                              armor_protection);
    }

    // Poison effect occurs even if initial damage was blocked
    if (random_number(4) != 1) {            // 75% chance of poison
      int poison_damage = random_number(3); // 1-3 additional damage
      print_message_formatted(
          "The venom burns through your veins for %d additional damage!\n",
          poison_damage);
      damage += poison_damage;

      if (random_number(3) == 1) { // 33% chance of stat reduction
        print_message("The poison weakens you!\n");
        player->strength -= 1;
      }
    }

    if (damage <= 0) {
      print_message("The bite didn't penetrate your defenses!\n");
      return;
    }

    player->strength -= damage;
    print_message("The poisonous bite deals %d total damage!\n", damage);
    break;
  }

  if (player->strength <= 0) {
    switch (head) {
    case 1:
      print_message("\nThe Chimera's Lion head has torn you apart\n");
      break;
    case 2:
      print_message("\nThe Chimera's flames have reduced you to ash!\n");
      break;
    case 3:
      print_message("\nThe Chimer'as venom has stopped your hart.!\n");
      break;
    }
    game->game_over = 1;
  }
}

void wolf_frenzy_attack(Player *player, int enemy_strength,
                        int enemy_dexterity) {
  print_message("\nThe wolf's eyes glow with feral intensity!\n");

  // First attack hit chance calculation
  int hit_chance = 50;
  hit_chance += (enemy_dexterity - 10) * 2; // Normal dexterity bonus
  hit_chance +=
      (10 - get_minimum(10, enemy_strength)) * 2; // Up to +20% when injured
  hit_chance -= (player->dexterity - 10) * 2;     // Player's dodge chance
  hit_chance += 15 * player->blindness_flag;      // Blindness penalty

  if (hit_chance < 10)
    hit_chance = 10;
  if (hit_chance > 90)
    hit_chance = 90;

  if (random_number(100) > hit_chance) {
    print_message("The wolf's frenzied attack misses!\n");
    return;
  }

  // Base damage
  int damage = random_number(3) + 2; // 3-5 base damage

  // Bonus damage when wounded
  if (enemy_strength < 8) {         // Below 50% health
    damage += random_number(3) + 2; // Extra 3-5 damage when desperate
    print_message("The wolf attacks with desperate fury!\n");
  }

  // Apply normal combat modifiers
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  // Apply armor reduction if present
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type;
    damage -= armor_protection;
    player->armor_points -= 1; // Minor armor damage

    if (damage < 0) {
      damage = 0;
    }

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message("Your armor has been torn apart!\n");
    }
  }

  if (damage > 0) {
    player->strength -= damage;
    print_message("The wolf's frenzy attack deals %d damage!\n", damage);

    // Second attack attempt if unarmored - based on stats
    if (player->armor_type == 0) {
      // Calculate success chance based on the wolf's dexterity advantage
      if (enemy_attack_hits(player,
                            enemy_dexterity + (enemy_strength < 8 ? 2 : 0))) {
        int second_damage = random_number(2); // 1-2 damage
        player->strength -= second_damage;
        print_message(
            "The wolf's second bite hits, dealing %d additional damage!\n",
            second_damage);
      } else {
        print_message("The wolf's second bite misses!\n");
      }
    }
  } else {
    print_message("Your defenses absorb the attack!\n");
  }
}

void gargoyle_stone_form_attack(Player *player, GameState *game,
                                int enemy_strength, int enemy_dexterity) {
  print_message("\nThe Gargoyle's flesh turns to living stone!\n");

  // Attack uses normal hit chance calculation
  if (!enemy_attack_hits(player, enemy_dexterity)) {
    print_message("The Gargoyle's heavy stone fist misses you!\n");
    return;
  }

  // Heavy stone attack damage
  int damage = random_number(6) + 3; // 4-9 base damage
  damage += enemy_strength / 8;      // Bonus from strength

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  // Stone form vs different armor types
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type;

    // Stone skin spell creates magical stone armor (type 4)
    if (player->armor_type == STONE) {
      print_message("Your stone armor deflects most of the impact!\n");
      damage -= (armor_protection *
                 2); // Stone armor is extra effective vs stone attacks
      player->armor_points -= 1; // Minimal wear
    } else {
      damage -= armor_protection;
      player->armor_points -= armor_protection; // Normal armor damage

      if (player->armor_points <= 0) {
        player->armor_points = 0;
        player->armor_type = 0;
        print_message("Your armor crumbes undeer the stone impact!  Good luck "
                      "adeventurer.\n");
      }
    }
  }

  if (damage <= 0) {
    print_message("Your defenses absorb the stone attack!\n");
    return;
  }

  player->strength -= damage;
  print_message("The stone-form attack deals %d damage!\n", damage);

  // Chance to stagger player (unless wearing stone armor)
  if (player->armor_type != STONE &&
      enemy_attack_hits(player, enemy_dexterity + 2)) {
    if (player->temp_dexterity == 0) {
      player->temp_dexterity = player->dexterity;
    }
    int stagger = random_number(2); // 1-2 dexterity reduction
    player->dexterity -= stagger;
    print_message(
        "The heavy impact staggers you, reducing your dexterity by %d!\n",
        stagger);
  }

  if (player->strength <= 0) {
    print_message("\nThe Gargoyle's stone fist has crushed you!\n");
    game->game_over = 1;
  }
}

void bear_maul_attack(Player *player, GameState *game, int enemy_strength,
                      int enemy_dexterity) {
  print_message("\nThe Bear rears up and unleashes a savage mauling attack!\n");

  if (!enemy_attack_hits(player, enemy_dexterity)) {
    print_message("You manage to avoid the Bear's deadly claws!\n");
    return;
  }

  // Base damage
  int damage = random_number(5) + 2; // 3-7 base damage
  damage += enemy_strength / 8;      // Strength bonus

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  // Apply armor reduction if present
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type;
    damage -= armor_protection;
    player->armor_points -=
        armor_protection; // Significant armor damage from claws

    if (damage < 0) {
      damage = 0;
    }

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message("Your armor is torn to shreds by the Bear's claws.  Good "
                    "luck adventurer.!\n");
    }
  }

  if (damage > 0) {
    player->strength -= damage;
    print_message("The mauling attack deals %d damage!\n", damage);

    // Second claw attack
    if (enemy_attack_hits(player, enemy_dexterity)) {
      int claw_damage = random_number(4); // 1-4 damage

      if (player->armor_type != 0) {
        claw_damage -= player->armor_type;
        player->armor_points -= 1; // Minor additional armor damage

        if (claw_damage < 0) {
          claw_damage = 0;
        }
      }

      if (claw_damage > 0) {
        player->strength -= claw_damage;
        print_message(
            "The Bear's second claw rakes you for %d additional damage!\n",
            claw_damage);
      }
    }

    // Bleeding effect if unarmored
    if (player->armor_type == 0 && enemy_attack_hits(player, enemy_dexterity)) {
      int bleed = random_number(3); // 1-3 bleed damage
      player->strength -= bleed;
      print_message("The deep wounds cause %d bleeding damage!\n", bleed);
    }
  } else {
    print_message("Your defenses absorb the mauling attack!\n");
  }

  if (player->strength <= 0) {
    print_message("\nTHE BEAR HAS MAULED YOU TO DEATH!\n");
    game->game_over = 1;
  }
}

void ogre_rage_attack(Player *player, GameState *game, int enemy_strength,
                      int *enemy_dexterity) {
  print_message("\nThe Ogre works itself into a mindless rage!\n");

  if (!enemy_attack_hits(player, *enemy_dexterity)) {
    print_message("The Ogre's wild swing misses completely!\n");
    *enemy_dexterity -= 1; // Gets clumsier in rage
    return;
  }

  // Rage increases damage but reduces accuracy
  int damage = random_number(8); // 1-8 base damage
  damage += enemy_strength / 6;  // Better strength bonus while raging

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  // Apply armor reduction if present
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type;
    damage -= armor_protection;
    player->armor_points -= 2; // Heavy armor damage from powerful blows

    if (damage < 0) {
      damage = 0;
    }

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message(
          "YOUR ARMOR IS SMASHED BY THE OGRE'S TREMENDOUS STRENGTH!\n");
    }
  }

  if (damage > 0) {
    player->strength -= damage;
    print_message("The Ogre's rage-fueled attack deals %d damage!\n", damage);

    // Second wild swing with penalty
    if (enemy_attack_hits(player, *enemy_dexterity - 2)) {
      int second_damage = random_number(6); // 1-6 damage
      if (player->armor_type != 0) {
        second_damage -= player->armor_type;
        if (second_damage < 0) {
          second_damage = 0;
        }
      }

      if (second_damage > 0) {
        player->strength -= second_damage;
        print_message(
            "The Ogre's second wild swing connects for %d more damage!\n",
            second_damage);
      }
    } else {
      print_message("The Ogre's second attack misses as it loses balance!\n");
      *enemy_dexterity -= 1; // Gets even clumsier
    }
  } else {
    print_message("Your defenses absorb the Ogre's attack!\n");
  }

  if (player->strength <= 0) {
    print_message("\nTHE OGRE HAS BEATEN YOU TO DEATH IN ITS RAGE!\n");
    game->game_over = 1;
  }
}

void orc_battle_fury_attack(Player *player, GameState *game, int enemy_strength,
                            int enemy_dexterity) {
  print_message("\nThe Orc works itself into a battle fury!\n");

  // First attack
  if (!enemy_attack_hits(player, enemy_dexterity)) {
    print_message("The Orc's aggressive swing misses!\n");
    return;
  }

  // Base damage with fury bonus
  int damage = random_number(4) + 1; // 2-5 base damage (lower than ogre/troll)
  damage += enemy_strength / 8;      // Strength bonus
  enemy_strength += 1;               // Fury increases strength

  // Player's defensive calculations
  int player_defense = player->strength / 9 + player->dexterity / 6;
  damage -= player_defense;

  // Apply armor
  if (player->armor_type != 0) {
    int armor_protection = player->armor_type;
    damage -= armor_protection;
    player->armor_points -= 1; // Minimal armor damage (level 2 monster)

    if (damage < 0) {
      damage = 0;
    }

    if (player->armor_points <= 0) {
      player->armor_points = 0;
      player->armor_type = 0;
      print_message("YOUR ARMOR IS BROKEN BY THE ORC'S FURY!\n");
    }
  }

  if (damage > 0) {
    player->strength -= damage;
    print_message("The Orc's fury attack deals %d damage!\n", damage);

    // As fury builds, chance for quick second attack
    if (enemy_attack_hits(player, enemy_dexterity -
                                      2)) { // Harder to hit with quick attack
      int quick_damage = random_number(3);  // 1-3 damage for quick attack

      if (player->armor_type != 0) {
        quick_damage -= player->armor_type;
        if (quick_damage < 0) {
          quick_damage = 0;
        }
      }

      if (quick_damage > 0) {
        player->strength -= quick_damage;
        enemy_strength += 1; // Successful fury attack increases strength more
        print_message("The Orc's battle fury intensifies! Quick "
                      "strike deals %d more damage!\n",
                      quick_damage);
      }
    }
  } else {
    print_message("Your defenses absorb the Orc's attack!\n");
  }

  if (player->strength <= 0) {
    print_message("\nTHE ORC'S BATTLE FURY HAS OVERWHELMED YOU!\n");
    game->game_over = 1;
  }
}

void goblin_dirty_tricks_attack(Player *player, GameState *game,
                                int enemy_strength, int enemy_dexterity) {
  print_message("\nThe Goblin unleashes a series of dirty fighting tricks!\n");

  // Base attack with dirty fighting
  if (!enemy_attack_hits(player, enemy_dexterity)) {
    print_message("The Goblin's attack misses!\n");
    return;
  }

  // Choose a dirty trick
  int trick = random_number(5);
  switch (trick) {
  case 1: // Stolen spices - temporary
    if (player->blindness_flag == 0 && player->temp_blindness_flag == 0) {
      print_message("The Goblin throws stolen pepper in your eyes!\n");
      if (game->treasure[OPAL_EYE_INDEX]) {
        print_message("But the Opal Eye immediately cures your blindness!\n");
        player->temp_blindness_flag = 0;
        player->blindness_flag = 0;
      } else {
        player->temp_blindness_flag = 1;
        player->blindness_flag = 1;
        print_message("Your eyes burn terribly - you can't see!\n");
      }
    } else {
      trick = 5; // Fall back to regular attack if already blind
    }
    break;

  case 2: // Poisonous powder - permanent unless cured
    if (player->blindness_flag == 0 && player->temp_blindness_flag == 0) {
      print_message("The Goblin throws a vile black powder in your face!\n");
      if (game->treasure[OPAL_EYE_INDEX]) {
        print_message("But the Opal Eye immediately cures your blindness!\n");
        player->temp_blindness_flag = 0;
        player->blindness_flag = 0;
      } else {
        player->blindness_flag = 1;
        player->temp_blindness_flag = 0;
        print_message("The poison burns your eyes - you've been blinded!\n");
      }
    } else {
      trick = 5;
    }
    break;

  case 3: // Hot embers - temporary
    if (player->blindness_flag == 0 && player->temp_blindness_flag == 0) {
      print_message("The Goblin flings hot embers at your face!\n");
      if (game->treasure[OPAL_EYE_INDEX]) {
        print_message("But the Opal Eye immediately cures your blindness!\n");
        player->temp_blindness_flag = 0;
        player->blindness_flag = 0;
      } else {

        player->temp_blindness_flag = 1;
        player->blindness_flag = 1;
        print_message("The burning ash blinds you!\n");
      }
    } else {
      trick = 5;
    }
    break;

  case 4: // Mushroom spores - permanent unless cured
    if (player->blindness_flag == 0 && player->temp_blindness_flag == 0) {
      print_message("The Goblin throws toxic mushroom spores at you!\n");
      if (game->treasure[OPAL_EYE_INDEX]) {
        print_message("But the Opal Eye immediately cures your blindness!\n");
        player->temp_blindness_flag = 0;
        player->blindness_flag = 0;
      } else {
        player->blindness_flag = 1;
        player->temp_blindness_flag = 0;
        print_message("The spores infect your eyes - you've been blinded!\n");
      }
    } else {
      trick = 5;
    }
    break;

  case 5: // Opportunistic strike
    print_message("The Goblin makes an opportunistic attack!\n");
    int damage = random_number(4) + 2; // 3-6 base damage
    damage += enemy_strength / 9;      // Small strength bonus

    // Player's defensive calculations
    int player_defense = player->strength / 9 + player->dexterity / 6;
    damage -= player_defense;

    if (player->armor_type != 0) {
      int armor_protection = player->armor_type;
      damage -= armor_protection;
      player->armor_points -= 1; // Minor armor damage

      if (damage < 0) {
        damage = 0;
      }

      if (player->armor_points <= 0) {
        player->armor_points = 0;
        player->armor_type = 0;
        print_message("Your armor falls apart!\n");
      }
    }

    if (damage > 0) {
      player->strength -= damage;
      print_message("The strike deals %d damage!\n", damage);
    } else {
      print_message("Your defenses absorb the attack!\n");
    }
    break;
  }

  // Add a small amount of damage from blinding attacks
  if (trick < 5 && (player->blindness_flag || player->temp_blindness_flag)) {
    int blind_damage = random_number(2); // 1-2 damage from eye pain
    if (blind_damage > 0) {
      player->strength -= blind_damage;
      print_message("The pain deals %d additional damage!\n", blind_damage);
    }
  }

  if (player->strength <= 0) {
    print_message("\nTHE GOBLIN'S DIRTY FIGHTING HAS ENDED YOUR ADVENTURE!\n");
    game->game_over = 1;
  }
}

// First determine if monster drops any armor
bool should_drop_armor(int monster_type) {
  switch (monster_type) {
  case DRAGON:
    return random_number(5) <= 2; // 40% chance

  case BALROG:
    return random_number(4) <= 1; // 25% chance

  case MINOTAUR:
  case CHIMERA:
  case GARGOYLE:
    return random_number(6) <= 1; // ~17% chance

  case TROLL:
  case OGRE:
  case BEAR:
    return random_number(8) <= 1; // ~12% chance

  case KOBOLD:
  case ORC:
  case GOBLIN:
    return random_number(10) <= 1; // 10% chance

  case WOLF: // Wolves don't drop armor
    return false;

  default:
    return false;
  }
}

// First determine if monster drops any weapon
bool should_drop_weapon(int monster_type) {
  switch (monster_type) {
  case DRAGON:
    return random_number(4) <= 1; // 25% chance

  case BALROG:
  case MIMIC:
    return random_number(5) <= 1; // 20% chance

  case MINOTAUR:
  case CHIMERA:
  case GARGOYLE:
    return random_number(6) <= 1; // ~17% chance

  case TROLL:
  case OGRE:
    return random_number(8) <= 1; // ~12% chance

  case KOBOLD:
  case ORC:
  case GOBLIN:
    return random_number(10) <= 1; // 10% chance

  case WOLF: // Wolves don't drop weapons
  case BEAR: // Bears don't drop weapons
    return false;

  default:
    return false;
  }
}

// Returns armor type based on monster and chance roll
int get_monster_armor_drop(int monster_type) {
  switch (monster_type) {
  case DRAGON:
    // Dragons: 60% plate, 40% stone when they do drop
    return (random_number(10) <= 6) ? PLATE : STONE;
  case MIMIC:
    // Mimics: Mix of armor when they solidify after death
    switch (random_number(15)) {
    case 1:
    case 2:
      return PLATE; // ~13% PLATE
    case 3:
    case 4:
    case 5:
      return CHAINMAIL; // ~20% CHAINMAIL
    case 6:
    case 7:
    case 8:
    case 9:
      return LEATHER; // ~27% LEATHER
    default:
      return NOTHING; // ~40% nothing
    }
  case BALROG:
    // Balrog: 80% plate, 20% stone when they do drop
    return (random_number(10) <= 8) ? PLATE : STONE;

  case MINOTAUR:
  case CHIMERA:
  case GARGOYLE:
    // Level 8-10 monsters: 30% plate, 70% chainmail
    return (random_number(10) <= 3) ? PLATE : CHAINMAIL;

  case TROLL:
  case OGRE:
  case BEAR:
    switch (random_number(16)) {
    case 1:
    case 2:
      return PLATE;
    case 3:
    case 4:
    case 5:
    case 6:
      return CHAINMAIL;
    case 7:
    case 8:
    case 9:
    case 10:
      return LEATHER;
    default:
      return NOTHING;
    }

  case KOBOLD:
  case ORC:
  case GOBLIN:
    // Level 1-4 monsters: 70% leather, 30% chainmail
    return (random_number(10) <= 7) ? LEATHER : CHAINMAIL;

  default:
    return NOTHING;
  }
}

// Returns weapon type based on monster and chance roll
int get_monster_weapon_drop(int monster_type) {

  switch (monster_type) {
  case DRAGON:
    // Dragons: 80% sword, 20% excalibur when they do drop
    return (random_number(10) <= 8) ? SWORD : EXCALIBUR;

  case BALROG:
    // Balrog: 90% sword, 10% excalibur when they do drop
    return (random_number(10) <= 9) ? SWORD : EXCALIBUR;
  case MIMIC:
    // Mimics: Mix of armor types since they can absorb and form materials
    switch (random_number(15)) {
    case 1:
    case 2:
      return SWORD; // 20% plate
    case 3:
    case 4:
    case 5:
      return MACE; // 30% chainmail
    case 6:
    case 7:
    case 8:
    case 9:
      return DAGGER;
    default:
      return NOTHING; // 50% leather
    }
  case MINOTAUR:
  case CHIMERA:
  case GARGOYLE:
    // Level 8-10 monsters: 40% sword, 60% mace
    return (random_number(10) <= 4) ? SWORD : MACE;

  case TROLL:
  case OGRE:
    // Level 5-7 monsters: 20% sword, 50% mace, 30% dagger
    switch (random_number(18)) {
    case 1:
    case 2:
      return SWORD;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      return MACE;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      return DAGGER;
    default:
      return NOTHING;
    }

  case KOBOLD:
  case ORC:
  case GOBLIN:
    // Level 1-4 monsters: 60% dagger, 40% mace
    return (random_number(10) <= 6) ? DAGGER : MACE;

  default:
    return NOTHING;
  }
}

int can_cast_magic(Player *player, int muted) {
  if (muted) {
    return 0;
  }

  switch (player->race) {
  case DWARF:
  case ELF:
  case DROW:
    return player->intelligence >= 10;
  case HOBBIT:
    return player->intelligence >= 11;
  case HUMAN:
    return player->intelligence >= 12;
  default:
    return player->intelligence >= 14;
  }
}

int can_escape(Player *player, int is_vendor, const char *enemy_name) {
  if (is_vendor) {
    return 0;
  }

  // Dexterity over 18 isn't used for escaping.
  switch (player->race) {
  case HOBBIT:
    if (random_number(10) == 1 ||
        random_number(24 - get_minimum(18, player->dexterity)) == 1) {
      print_message("Using your hobbit stealth (and the fact that you are very "
                    "short), you sneak past the %s!\n",
                    enemy_name);
      return 1;
    }
    break;

  case DROW:
    if (random_number(12) == 1 ||
        random_number(26 - get_minimum(18, player->dexterity)) == 1) {
      print_message(
          "Using your Drow stealth, you slip past the %s unnoticed!\n",
          enemy_name);
      return 1;
    }
    break;

  case ELF:
    if (random_number(13) == 1 ||
        random_number(27 - get_minimum(18, player->dexterity)) == 1) {
      print_message(
          "Using your legendary Elf stealth, you slip past the %s unnoticed!\n",
          enemy_name);
      return 1;
    }
    break;

  case HUMAN:
    if (random_number(14) == 1 ||
        random_number(27 - get_minimum(18, player->dexterity)) == 1) {
      print_message("You slip past the %s unnoticed!\n", enemy_name);
      return 1;
    }
    break;

  case DWARF:
    if (random_number(15) == 1 ||
        random_number(26 - get_minimum(18, player->dexterity)) == 1) {
      print_message("Using your Dwarven... er, 'aerodynamic' build, you "
                    "somehow manage to roll past the %s unnoticed. Who knew "
                    "being stout had its advantages?\n",
                    enemy_name);
      return 1;
    }
    break;
  }
  return 0;
}

void mimic_special_attack(Player *player, GameState *game, int enemy_strength,
                          int *enemy_dexterity, int *enemy_intelligence) {
  int spell = random_number(100);
  int temp;

  if (spell <= 25) {
    // Camouflage
    int dex_boost = random_number(3) + 2;
    *enemy_dexterity += dex_boost;
    print_message(
        "The mimic's form shimmers and blends with its surroundings!\n");
  } else if (spell <= 50) {
    // Shape Weapon
    enemy_strength += random_number(2) + 2;
    *enemy_dexterity -= 1;
    print_message("The mimic reshapes its form into deadly weapons!\n");
  } else if (spell <= 70) {
    // Adhesive Trap
    print_message("The mimic exudes a powerful adhesive!\n");

    // Similar avoidance check to other monster abilities
    int avoidance_chance =
        (player->intelligence * 2 + player->strength + player->dexterity) / 4;
    avoidance_chance += random_number(10) - 5;

    if (avoidance_chance < 5)
      avoidance_chance = 5;
    if (avoidance_chance > 95)
      avoidance_chance = 95;

    if (random_number(100) < avoidance_chance) {
      print_message("You avoid getting caught in the adhesive!\n");
    } else {
      temp = random_number(3) + 1; // 1-3 point reduction
      player->dexterity -= temp;
      player->armor_points -= 1; // Minor armor damage
      print_message("You're stuck! You lose %d dexterity points and "
                    "your armor is damaged!\n",
                    temp);

      if (player->dexterity <= 0) {
        player->dexterity = 0;
        print_message("Unable to move, you collapse...\n");
        game->game_over = 1;
        return;
      }
    }
  } else if (spell <= 85) {
    // Perfect Copy - temporarily gains some of player's stats
    print_message("The mimic briefly takes your exact form!\n");
    int dex_gain = player->dexterity / 6;
    int int_gain = player->intelligence / 6;
    if (dex_gain <= 0)
      dex_gain = 1;
    if (int_gain <= 0)
      int_gain = 1;
    *enemy_dexterity += dex_gain;
    *enemy_intelligence += int_gain;
    print_message("It gains %d to dexterity and %d to intelligence!\n",
                  dex_gain, int_gain);
  } else {
    // Dissolving Touch - armor damage
    print_message("The mimic attempts to dissolve your equipment!\n");

    if (player->armor_type != 0) {
      temp = random_number(4) + 1; // 1-4 armor damage
      player->armor_points -= temp;
      print_message("Your armor loses %d points!\n", temp);

      if (player->armor_points <= 0) {
        player->armor_points = 0;
        player->armor_type = 0;
        print_message("Your armor has been completely dissolved!\n");
      }
    } else {
      // If no armor, does minor damage instead
      temp = random_number(2) + 1; // 1-2 damage
      player->strength -= temp;
      print_message("The dissolving touch deals %d damage!\n", temp);

      if (player->strength <= 0) {
        player->strength = 0;
        print_message("You've been dissolved completely!\n");
        game->game_over = 1;
        return;
      }
    }
  }
}
