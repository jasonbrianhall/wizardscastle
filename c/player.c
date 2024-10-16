#include "player.h"
#include "wizards-castle.h"
#include "utilities.h"
#include "wizardio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void initialize_player(Player *player)
{
	// Set initial attributes
	player->race = 0;
	player->sex = 0;
	player->strength = 2;
	player->intelligence = 8;
	player->dexterity = 14;
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
void choose_race(Player *player)
{
    const char *races[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    char user_input;
    int valid_choice = 0;

    do {
        print_message_formatted("ALL RIGHT, BOLD ONE.\n");
        print_message          ("You may be an (E)lf, (D)warf, (M)an, or (H)obbit.\n\n");
        print_message_formatted("YOUR CHOICE:  ");
        user_input=get_user_input();

        player->race = 0;

        switch (user_input) {
            case 'H':
                player->race = 1;  // Hobbit
                break;
            case 'E':
                player->race = 2;  // Elf
                break;
            case 'M':
                player->race = 3;  // Man/Human
                break;
            case 'D':
                player->race = 4;  // Dwarf
                break;
            default:
                // Handle invalid input if needed
                break;
            }
            
        if (player->race > 0) {
            player->strength += 2 * player->race;
            player->dexterity -= 2 * player->race;
            if (player->race == 2) {  // Elf gets extra points
                player->intelligence += 4;
            }
            print_message(           "You have chosen to be a ");
            print_message_formatted("%s\n", races[player->race - 1]);
            valid_choice = 1;
        } else {
            print_message("\n** That was incorrect. Please type E, D, M, OR H.\n");
        }
    } while (!valid_choice);
}

void choose_sex(Player *player)
{
    char user_input;
    int valid_choice = 0;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    do {
        print_message_formatted("WHICH SEX DO YOU PREFER?\n\n");
        print_message_formatted("YOUR CHOICE:  ");
        user_input=get_user_input();

        switch (user_input) {
            case 'M':
                player->sex = 1;
                valid_choice = 1;
                break;
            case 'F':
                player->sex = 0;
                valid_choice = 1;
                break;
            default:
                {
                    print_message_formatted("** CUTE ");
                    print_message_formatted("%s, REAL CUTE. TRY ", race_names[player->race - 1]);
                    print_message(          "M or F.\n");
                }
                break;
        }
    } while (!valid_choice);

    print_message_formatted("\n");

}

void allocate_attributes(Player *player)
{
    int other_points;
    int points_to_add;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    // Set other_points based on race
    if (player->race == 1) { // Hobbit
        other_points = 4;
    } else { // Elf, Human, Dwarf
        other_points = 8;
    }

    print_message_formatted("Ok, ");
    print_message_formatted("%s, YOU HAVE THE FOLLOWING ATTRIBUTES:\n", race_names[player->race - 1]);
    print_message          ("Strength = %d    Intelligence = %d    Dexterity = %d\n", 
           player->strength, player->intelligence, player->dexterity);
    print_message_formatted("AND %d OTHER POINTS TO ALLOCATE AS YOU WISH.\n\n", other_points);

    // Allocate points to Strength
    while (other_points > 0) {
        print_message_formatted("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR STRENGTH? ");
        points_to_add = get_user_input_number();

        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->strength += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            print_message_formatted("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    if (player->strength>18)
    {
        other_points+=player->strength-18;
        player->strength=18;
    }


    // Allocate points to Intelligence
    while (other_points > 0) {
        print_message_formatted("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR INTELLIGENCE? ");
        points_to_add = get_user_input_number();
        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->intelligence += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            print_message_formatted("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    if (player->intelligence>18)
    {
        other_points+=player->intelligence-18;
        player->intelligence=18;
    }

    // Allocate remaining points to Dexterity
    if (other_points > 0) {
        print_message_formatted("ALLOCATING REMAINING %d POINTS TO DEXTERITY.\n", other_points);
        player->dexterity += other_points;
    }

    print_message_formatted("\nYOUR ATTRIBUTES ARE NOW:\n");
    print_message          ("Strength = %d    Intelligence = %d    Dexterity = %d\n", 
           player->strength, player->intelligence, player->dexterity);
}

// Item and treasure functions
void buy_equipment(Player *player)
{
    char user_input;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    const char *armor_types[] = {"NO ARMOR", "LEATHER", "CHAINMAIL", "PLATE"};
    const char *weapon_types[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD"};
    int cost;
    char message[100];

    // Buy Armor
    print_message_formatted("\nOK, ");
    print_message_formatted(race_names[player->race - 1]);

    print_message_formatted(", YOU HAVE %d GOLD PIECES (GP'S).\n\n", player->gold);
    
    print_message_formatted("THESE ARE THE TYPES OF ARMOR YOU CAN BUY :\n");
    print_message(          "(P)late<30> (C)hainmail<20> (L)eather<10> (N)othing<0>\n\n");
    
    do {
        user_input = get_user_input();
        switch(user_input) {
            case 'P': player->armor_type = 3; cost = 30; break;
            case 'C': player->armor_type = 2; cost = 20; break;
            case 'L': player->armor_type = 1; cost = 10; break;
            case 'N': player->armor_type = 0; cost = 0; break;
            default:
                print_message_formatted("\n** ARE YOU A %s OR A FOOL? TRY AGAIN.\n\n", race_names[player->race - 1]);
                continue;
        }
    } while (1);

    player->gold -= cost;
    player->armor_points = player->armor_type * 7;

    // Buy Weapon
    snprintf(message, sizeof(message), "\nOK, %s, YOU HAVE %d GP'S LEFT.\n\n", race_names[player->race - 1], player->gold);
    print_message_formatted(message);
    print_message_formatted("THESE ARE THE TYPES OF WEAPONS YOU CAN BUY :\n");
    print_message_formatted("SWORD<30> MACE<20> DAGGER<10> NOTHING<0>\n\n");
    
    do {
        print_message_formatted("YOUR CHOICE:  ");
        user_input = get_user_input();

        switch(user_input) {
            case 'S': 
                if (player->gold < 30) {
                    print_message_formatted("** YOUR DUNGEON EXPRESS CARD - YOU LEFT HOME WITHOUT IT!\n\n");
                    continue;
                }
                player->weapon_type = 3; cost = 30; break;
            case 'M': 
                if (player->gold < 20) {
                    print_message_formatted("** SORRY SIR, I'M AFRAID I DON'T GIVE CREDIT!\n\n");
                    continue;
                }
                player->weapon_type = 2; cost = 20; break;
            case 'D': player->weapon_type = 1; cost = 10; break;
            case 'N': player->weapon_type = 0; cost = 0; break;
            default:
                print_message("** Try again, your choice must be S, M, D, or N.\n\n");
                continue;
        }
    } while (1);

    player->gold -= cost;

    print_message_formatted("\nYOU NOW HAVE %s ARMOR AND A %s.\n", 
           armor_types[player->armor_type], weapon_types[player->weapon_type]);
    print_message_formatted("YOU HAVE %d GOLD PIECES LEFT.\n", player->gold);

}

void buy_lamp_and_flares(Player *player)
{
    char user_input_yn;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    // Try to buy a lamp
    if (player->gold >= 20 && !player->lamp_flag) {
        print_message_formatted("\nDO YOU WANT TO BUY A LAMP FOR 20 GP'S?\n");
        do {
            print_message("Your choice (Y/N):  ");
            user_input_yn = get_user_input_yn();
            if (user_input_yn == 'Y') {
                player->lamp_flag = 1;
                player->gold -= 20;
                print_message_formatted("\nOK, LAMP PURCHASED. IT'S GUARANTEED TO OUTLIVE YOU!\n");
                break;
            } else if (user_input_yn == 'N') {
                break;
            } else {
                print_message_formatted("** PLEASE ANSWER YES OR NO.\n");
            }
        } while (1);
    }

   // Try to buy flares
    if (player->gold >= 1) {
        print_message_formatted("\nOK, %s, YOU HAVE %d GOLD PIECES LEFT.\n", race_names[player->race - 1], player->gold);
        print_message_formatted("FLARES COST 1 GP EACH. HOW MANY DO YOU WANT? ");
        
        int flares_to_buy;
        do {
            flares_to_buy=get_user_input_number();

            // Check if the input is "0" to explicitly buy no flares
            if (flares_to_buy == 0) {
                print_message_formatted("YOU CHOSE NOT TO BUY ANY FLARES.\n");
                return;
            } else if (flares_to_buy < 0) {
                print_message_formatted("** PLEASE ENTER A POSITIVE NUMBER.\n");
            } else if (flares_to_buy > player->gold) {
                print_message_formatted("** YOU CAN ONLY AFFORD %d. PLEASE ENTER A LOWER NUMBER.\n", player->gold);
            } else {
                player->flares += flares_to_buy;
                player->gold -= flares_to_buy;
                print_message_formatted("\nOK, YOU NOW HAVE %d FLARES AND %d GOLD PIECES LEFT.\n", player->flares, player->gold);
                return;
            }
            print_message_formatted("HOW MANY FLARES DO YOU WANT? (OR ENTER 0 TO BUY NONE) ");
        } while (1);
    }
}

const char* get_race_name(int race)
{
    switch(race)
    {
        case 1: return "HOBBIT";
        case 2: return "ELF\0";
        case 3: return "HUMAN";
        case 4: return "DWARF";
        default: return "UNKNOWN";
    }
}


void print_status(Player *player, GameState *game)
{
    char message[256];  // Buffer for formatting messages

    print_message("\n=== PLAYER STATUS ===\n");

    // Print player race and attributes
    print_message("Race: ");
    print_message_formatted(get_race_name(player->race));
    print_message("\n");
    snprintf(message, sizeof(message), "Strength: %d  Intelligence: %d  Dexterity: %d\n",
             player->strength, player->intelligence, player->dexterity);
    print_message(message);

    // Print player position
    snprintf(message, sizeof(message), "Location: Level %d, Room (%d, %d)\n",
             player->level, player->y, player->x);
    print_message(message);

    // Print player inventory
    snprintf(message, sizeof(message), "Gold Pieces: %d  Flares: %d\n",
             player->gold, player->flares);
    print_message(message);

    // Print armor and weapon
    const char* armor_types[] = {"No Armor", "Leather", "Chainmail", "Plate"};
    const char* weapon_types[] = {"No Weapon", "Dagger", "Mace", "Sword"};
    snprintf(message, sizeof(message), "Armor: %s  Weapon: %s\n",
             armor_types[player->armor_type], weapon_types[player->weapon_type]);
    print_message(message);

    // Print special items
    print_message("Special Items: ");
    if (player->lamp_flag) print_message_formatted("Lamp ");
    if (player->runestaff_flag) print_message_formatted("Runestaff ");
    if (player->orb_flag) print_message_formatted("Orb of Zot ");
    print_message_formatted("\n");

    // Print curses or blessings
    print_message_formatted("Status Effects: ");
    if (player->blindness_flag) print_message_formatted("Blind ");
    if (player->stickybook_flag) print_message_formatted("Sticky Book ");
    print_message_formatted("\n");

    // Print number of treasures
    snprintf(message, sizeof(message), "Treasures Found: %d\n", player->treasure_count);
    print_message_formatted(message);
    for (int i=0; i<TREASURE_END-TREASURE_START; i++)
    {
        if(game->treasure[i]==1)
        {
        	print_message_formatted("     ");
        	print_message_formatted(get_treasure_name(i));
        	print_message_formatted("\n");
        }
    }

    print_message("======================\n\n");
}


