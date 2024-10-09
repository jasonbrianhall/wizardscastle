#include "wizards-castle.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void print_introduction(void)
{
	printStars();
        print_message("\n                * * * THE WIZARD'S CASTLE * * *\n\n");
	printStars();
	print_message("\n");
	print_message("MANY CYCLES AGO, IN THE KINGDOM OF N'DIC, THE GNOMIC\n");
	print_message("WIZARD ZOT FORGED HIS GREAT *ORB OF POWER*. HE SOON\n");
	print_message("VANISHED, LEAVING BEHIND HIS VAST SUBTERRANEAN CASTLE\n");
	print_message("FILLED WITH ESURIENT MONSTERS, FABULOUS TREASURES, AND\n");
	print_message("THE INCREDIBLE *ORB OF ZOT*. FROM THAT TIME HENCE, MANY\n");
	print_message("A BOLD YOUTH HAS VENTURED INTO THE WIZARD'S CASTLE. AS\n");
	print_message("OF NOW, *NONE* HAS EVER EMERGED VICTORIOUSLY! BEWARE!!\n\n");

}

void printStars(void)
{
	print_message("****************************************************************\n");

}

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

}

// Main game functions
void initialize_game(GameState *game)
{

}

void main_game_loop(Player *player, GameState *game)
{

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

    printf("OK, %s, YOU HAVE THE FOLLOWING ATTRIBUTES:\n", race_names[player->race - 1]);
    printf("STRENGTH = %d    INTELLIGENCE = %d    DEXTERITY = %d\n", 
           player->strength, player->intelligence, player->dexterity);
    printf("AND %d OTHER POINTS TO ALLOCATE AS YOU WISH.\n\n", other_points);

    // Allocate points to Strength
    while (other_points > 0) {
        printf("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR STRENGTH? ");
        fgets(user_input, sizeof(user_input), stdin);
        points_to_add = atoi(user_input);

        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->strength += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            printf("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    // Allocate points to Intelligence
    while (other_points > 0) {
        printf("HOW MANY POINTS DO YOU WISH TO ADD TO YOUR INTELLIGENCE? ");
        fgets(user_input, sizeof(user_input), stdin);
        points_to_add = atoi(user_input);

        if (points_to_add >= 0 && points_to_add <= other_points) {
            player->intelligence += points_to_add;
            other_points -= points_to_add;
            break;
        } else {
            printf("** INVALID INPUT. YOU HAVE %d POINTS TO ALLOCATE.\n\n", other_points);
        }
    }

    // Allocate remaining points to Dexterity
    if (other_points > 0) {
        printf("ALLOCATING REMAINING %d POINTS TO DEXTERITY.\n", other_points);
        player->dexterity += other_points;
    }

    printf("\nYOUR ATTRIBUTES ARE NOW:\n");
    printf("STRENGTH = %d    INTELLIGENCE = %d    DEXTERITY = %d\n", 
           player->strength, player->intelligence, player->dexterity);
}
          

// Map and room functions
void generate_castle(GameState *game)
{

}

int get_room_content(GameState *game, int x, int y, int level)
{

}

void set_room_content(GameState *game, int x, int y, int level, int content)
{

}

// Movement and action functions
void move_player(Player *player, GameState *game, char direction)
{

}

void handle_room_event(Player *player, GameState *game)
{

}

void fight_monster(Player *player, GameState *game)
{

}

void handle_vendor(Player *player, GameState *game)
{

}

// Item and treasure functions
void buy_equipment(Player *player)
{
    char user_input[10];
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};
    const char *armor_types[] = {"NO ARMOR", "LEATHER", "CHAINMAIL", "PLATE"};
    const char *weapon_types[] = {"NO WEAPON", "DAGGER", "MACE", "SWORD"};
    int cost;

    // Buy Armor
    print_message("\nOK, ");
    print_message(race_names[player->race - 1]);
    printf(", YOU HAVE %d GOLD PIECES (GP'S).\n\n", player->gold);
    
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
                printf("\n** ARE YOU A %s OR A FOOL? TRY AGAIN.\n\n", race_names[player->race - 1]);
                continue;
        }
        break;
    } while (1);

    player->gold -= cost;
    player->armor_points = player->armor_type * 7;

    // Buy Weapon
    printf("\nOK, %s, YOU HAVE %d GP'S LEFT.\n\n", race_names[player->race - 1], player->gold);
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

    printf("\nYOU NOW HAVE %s ARMOR AND A %s.\n", 
           armor_types[player->armor_type], weapon_types[player->weapon_type]);
    printf("YOU HAVE %d GOLD PIECES LEFT.\n", player->gold);

}

void buy_lamp_and_flares(Player *player)
{
    char user_input[10];
    const char *race_names[] = {"HOBBIT", "ELF", "HUMAN", "DWARF"};

    // Try to buy a lamp
    if (player->gold >= 20 && !player->lamp_flag) {
        print_message("\nDO YOU WANT TO BUY A LAMP FOR 20 GP'S?\n");
        do {
            print_message("YOUR CHOICE (Y/N):  ");
            fgets(user_input, sizeof(user_input), stdin);
            user_input[0] = toupper(user_input[0]);

            if (user_input[0] == 'Y') {
                player->lamp_flag = 1;
                player->gold -= 20;
                print_message("\nOK, LAMP PURCHASED. IT'S GUARANTEED TO OUTLIVE YOU!\n");
                break;
            } else if (user_input[0] == 'N') {
                break;
            } else {
                print_message("** PLEASE ANSWER YES OR NO.\n");
            }
        } while (1);
    }

    // Try to buy flares
    if (player->gold >= 1) {
        printf("\nOK, %s, YOU HAVE %d GOLD PIECES LEFT.\n", race_names[player->race - 1], player->gold);
        print_message("FLARES COST 1 GP EACH. HOW MANY DO YOU WANT? ");
        
        int flares_to_buy;
        do {
            fgets(user_input, sizeof(user_input), stdin);
            flares_to_buy = atoi(user_input);

            if (flares_to_buy >= 0 && flares_to_buy <= player->gold) {
                player->flares += flares_to_buy;
                player->gold -= flares_to_buy;
                printf("\nOK, YOU NOW HAVE %d FLARES AND %d GOLD PIECES LEFT.\n", player->flares, player->gold);
                break;
            } else if (flares_to_buy > player->gold) {
                printf("** YOU CAN ONLY AFFORD %d.\n", player->gold);
            } else {
                print_message("** IF YOU DON'T WANT ANY, JUST TYPE 0 (ZERO).\n");
            }
        } while (1);
    }
}

void use_lamp(Player *player, GameState *game)
{

}

void use_flare(Player *player, GameState *game)
{

}

void open_chest(Player *player, GameState *game)
{

}

void drink_from_pool(Player *player)
{

}

// Magic and special abilities
void cast_spell(Player *player, GameState *game, char spell_type)
{

}

void teleport(Player *player, GameState *game)
{

}

void gaze_into_orb(Player *player, GameState *game)
{

}

// Utility functions
int random_number(int max_value)
{

}

void print_status(Player *player)
{

}

void display_map(GameState *game, int current_level)
{

}

void print_help()
{

}

// Game ending functions
int check_game_over(Player *player, GameState *game)
{

}

void end_game(Player *player, GameState *game)
{

}

// Input/Output functions
char get_user_input()
{

}


void print_message(const char *message)
{
	printf("%s", message);
}

