#include "player.h"
#include "wizards-castle.h"
#include "utilities.h"
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
    char user_input[10];
    int valid_choice = 0;
    char message[100];  // Buffer for the message

    do {
        print_message("ALL RIGHT, BOLD ONE.\n");
        print_message("YOU MAY BE AN ELF, DWARF, MAN, OR HOBBIT.\n\n");
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);

        // Convert input to uppercase
        for (int i = 0; user_input[i]; i++) {
            user_input[i] = toupper(user_input[i]);
        }

        // Remove newline character if present
        user_input[strcspn(user_input, "\n")] = 0;

        player->race = 0;

        if (user_input[0] == 'H') player->race = 1;  // Hobbit
        else if (user_input[0] == 'E') player->race = 2;  // Elf
        else if (user_input[0] == 'M') player->race = 3;  // Man/Human
        else if (user_input[0] == 'D') player->race = 4;  // Dwarf

        if (player->race > 0) {
            player->strength += 2 * player->race;
            player->dexterity -= 2 * player->race;
            if (player->race == 2) {  // Elf gets extra points
                player->intelligence += 4;
            }
            snprintf(message, sizeof(message), "You have chosen to be a %s\n", races[player->race - 1]);
            print_message(message);
            valid_choice = 1;
        } else {
            print_message("\n** THAT WAS INCORRECT. PLEASE TYPE E, D, M, OR H.\n");
        }
    } while (!valid_choice);
}

void choose_sex(Player *player)
{
    char user_input[10];
    int valid_choice = 0;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    do {
        print_message("WHICH SEX DO YOU PREFER?\n\n");
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);

        // Convert input to uppercase
        for (int i = 0; user_input[i]; i++) {
            user_input[i] = toupper(user_input[i]);
        }

        // Remove newline character if present
        user_input[strcspn(user_input, "\n")] = 0;

        switch (user_input[0]) {
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
                    char message[100];
                    snprintf(message, sizeof(message), "** CUTE %s, REAL CUTE. TRY M OR F.\n", race_names[player->race - 1]);
                    print_message(message);
                }
                break;
        }
    } while (!valid_choice);

    print_message("\n");

}

void allocate_attributes(Player *player)
{
    int other_points;
    char user_input[10];
    int points_to_add;
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    // Set other_points based on race
    if (player->race == 1) { // Hobbit
        other_points = 4;
    } else { // Elf, Human, Dwarf
        other_points = 8;
    }

    print_message("OK, %s, YOU HAVE THE FOLLOWING ATTRIBUTES:\n", race_names[player->race - 1]);
    print_message("STRENGTH = %d    INTELLIGENCE = %d    DEXTERITY = %d\n", 
           player->strength, player->intelligence, player->dexterity);
    print_message("AND %d OTHER POINTS TO ALLOCATE AS YOU WISH.\n\n", other_points);

    // Allocate points to Strength
    while (other_points > 0) {
        print_message("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR STRENGTH? ");
        fgets(user_input, sizeof(user_input), stdin);
        points_to_add = atoi(user_input);

        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->strength += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            print_message("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    // Allocate points to Intelligence
    while (other_points > 0) {
        print_message("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR INTELLIGENCE? ");
        fgets(user_input, sizeof(user_input), stdin);
        points_to_add = atoi(user_input);

        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->intelligence += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            print_message("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    // Allocate remaining points to Dexterity
    if (other_points > 0) {
        print_message("ALLOCATING REMAINING %d POINTS TO DEXTERITY.\n", other_points);
        player->dexterity += other_points;
    }

    print_message("\nYOUR ATTRIBUTES ARE NOW:\n");
    print_message("STRENGTH = %d    INTELLIGENCE = %d    DEXTERITY = %d\n", 
           player->strength, player->intelligence, player->dexterity);
}

// Item and treasure functions
void buy_equipment(Player *player)
{
    char user_input[10];
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    const char *armor_types[] = {"NO ARMOR", "LEATHER", "CHAINMAIL", "PLATE"};
    const char *weapon_types[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD"};
    int cost;
    char message[100];

    // Buy Armor
    print_message("\nOK, ");
    print_message(race_names[player->race - 1]);

    print_message(", YOU HAVE %d GOLD PIECES (GP'S).\n\n", player->gold);
    
    print_message("THESE ARE THE TYPES OF ARMOR YOU CAN BUY :\n");
    print_message("PLATE<30> CHAINMAIL<20> LEATHER<10> NOTHING<0>\n\n");
    
    do {
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);
        user_input[0] = toupper(user_input[0]);

        switch(user_input[0]) {
            case 'P': player->armor_type = 3; cost = 30; break;
            case 'C': player->armor_type = 2; cost = 20; break;
            case 'L': player->armor_type = 1; cost = 10; break;
            case 'N': player->armor_type = 0; cost = 0; break;
            default:
                print_message("\n** ARE YOU A %s OR A FOOL? TRY AGAIN.\n\n", race_names[player->race - 1]);
                continue;
        }
        break;
    } while (1);

    player->gold -= cost;
    player->armor_points = player->armor_type * 7;

    // Buy Weapon
    snprintf(message, sizeof(message), "\nOK, %s, YOU HAVE %d GP'S LEFT.\n\n", race_names[player->race - 1], player->gold);
    print_message(message);
    print_message("THESE ARE THE TYPES OF WEAPONS YOU CAN BUY :\n");
    print_message("SWORD<30> MACE<20> DAGGER<10> NOTHING<0>\n\n");
    
    do {
        print_message("YOUR CHOICE:  ");
        fgets(user_input, sizeof(user_input), stdin);
        user_input[0] = toupper(user_input[0]);

        switch(user_input[0]) {
            case 'S': 
                if (player->gold < 30) {
                    print_message("** YOUR DUNGEON EXPRESS CARD - YOU LEFT HOME WITHOUT IT!\n\n");
                    continue;
                }
                player->weapon_type = 3; cost = 30; break;
            case 'M': 
                if (player->gold < 20) {
                    print_message("** SORRY SIR, I'M AFRAID I DON'T GIVE CREDIT!\n\n");
                    continue;
                }
                player->weapon_type = 2; cost = 20; break;
            case 'D': player->weapon_type = 1; cost = 10; break;
            case 'N': player->weapon_type = 0; cost = 0; break;
            default:
                print_message("** TRY AGAIN, YOUR CHOICE MUST BE S, M, D, OR N.\n\n");
                continue;
        }
        break;
    } while (1);

    player->gold -= cost;

    print_message("\nYOU NOW HAVE %s ARMOR AND A %s.\n", 
           armor_types[player->armor_type], weapon_types[player->weapon_type]);
    print_message("YOU HAVE %d GOLD PIECES LEFT.\n", player->gold);

}

void buy_lamp_and_flares(Player *player)
{
    char user_input_yn, user_input[10];
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    // Try to buy a lamp
    if (player->gold >= 20 && !player->lamp_flag) {
        print_message("\nDO YOU WANT TO BUY A LAMP FOR 20 GP'S?\n");
        do {
            print_message("YOUR CHOICE (Y/N):  ");
            user_input_yn = get_user_input_yn();
            if (user_input_yn == 'Y') {
                player->lamp_flag = 1;
                player->gold -= 20;
                print_message("\nOK, LAMP PURCHASED. IT'S GUARANTEED TO OUTLIVE YOU!\n");
                break;
            } else if (user_input_yn == 'N') {
                break;
            } else {
                print_message("** PLEASE ANSWER YES OR NO.\n");
            }
        } while (1);
    }

   // Try to buy flares
    if (player->gold >= 1) {
        print_message("\nOK, %s, YOU HAVE %d GOLD PIECES LEFT.\n", race_names[player->race - 1], player->gold);
        print_message("FLARES COST 1 GP EACH. HOW MANY DO YOU WANT? ");
        
        int flares_to_buy;
        char *endptr;
        do {
            fgets(user_input, sizeof(user_input), stdin);
            user_input[strcspn(user_input, "\n")] = 0; // Remove newline

            // Check if the input is "0" to explicitly buy no flares
            if (strcmp(user_input, "0") == 0) {
                print_message("YOU CHOSE NOT TO BUY ANY FLARES.\n");
                return;
            }

            // Convert input to integer and check for errors
            flares_to_buy = strtol(user_input, &endptr, 10);

            if (*endptr != '\0') {
                print_message("** INVALID INPUT. PLEASE ENTER A NUMBER OR 0 TO BUY NO FLARES.\n");
            } else if (flares_to_buy < 0) {
                print_message("** PLEASE ENTER A NON-NEGATIVE NUMBER.\n");
            } else if (flares_to_buy > player->gold) {
                print_message("** YOU CAN ONLY AFFORD %d. PLEASE ENTER A LOWER NUMBER.\n", player->gold);
            } else {
                player->flares += flares_to_buy;
                player->gold -= flares_to_buy;
                print_message("\nOK, YOU NOW HAVE %d FLARES AND %d GOLD PIECES LEFT.\n", player->flares, player->gold);
                return;
            }
            print_message("HOW MANY FLARES DO YOU WANT? (OR ENTER 0 TO BUY NONE) ");
        } while (1);
    }
}

const char* get_race_name(int race)
{
    switch(race)
    {
        case 1: return "HOBBIT";
        case 2: return "ELF";
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
    snprintf(message, sizeof(message), "Race: %s\n", get_race_name(player->race));
    print_message(message);
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
    if (player->lamp_flag) print_message("Lamp ");
    if (player->runestaff_flag) print_message("Runestaff ");
    if (player->orb_flag) print_message("Orb of Zot ");
    print_message("\n");

    // Print curses or blessings
    print_message("Status Effects: ");
    if (player->blindness_flag) print_message("Blind ");
    if (player->stickybook_flag) print_message("Sticky Book ");
    print_message("\n");

    // Print number of treasures
    snprintf(message, sizeof(message), "Treasures Found: %d\n", player->treasure_count);
    print_message(message);
    for (int i=0; i<TREASURE_END-TREASURE_START; i++)
    {
        if(game->treasure[i]==1)
        {
        	print_message("     ");
        	print_message(get_treasure_name(i));
        	print_message("\n");
        }
    }

    print_message("======================\n\n");
}


