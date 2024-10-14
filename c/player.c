#include "player.h"
#include "wizards-castle.h"
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

