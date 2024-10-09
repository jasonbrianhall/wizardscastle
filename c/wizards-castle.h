#ifndef WIZARDS_CASTLE_H
#define WIZARDS_CASTLE_H

#define MAP_SIZE 512
#define TREASURE_COUNT 8

// Structures
typedef struct {
    int race;
    int sex;
    int strength;
    int intelligence;
    int dexterity;
    int gold;
    int flares;
    int armor_type;
    int armor_points;
    int weapon_type;
    int lamp_flag;
    int x;
    int y;
    int level;
    int runestaff_flag;
    int orb_flag;
    
    int book_flag;
    int blindness_flag;
    int stickybook_flag;
    int has_runestaff;
    int has_orb;
    int treasure_count;

} Player;

typedef struct {
    int location_map[MAP_SIZE];
    int treasure[TREASURE_COUNT];
    int orb_location[3];
    int runestaff_location[3];
    int turn_count;
    int monster_count;
} GameState;

// Main game functions
void initialize_game(GameState *game);
void initialize_player(Player *player);

void print_introduction(void);
void main_game_loop(Player *player, GameState *game);

// Player creation and attribute functions
void choose_race(Player *player);
void choose_sex(Player *player);
void buy_lamp_and_flares(Player *player);
void allocate_attributes(Player *player);

// Map and room functions
void generate_castle(GameState *game);
int get_room_content(GameState *game, int x, int y, int level);
void set_room_content(GameState *game, int x, int y, int level, int content);

// Movement and action functions
void move_player(Player *player, GameState *game, char direction);
void handle_room_event(Player *player, GameState *game);
void fight_monster(Player *player, GameState *game);
void handle_vendor(Player *player, GameState *game);

// Item and treasure functions
void buy_equipment(Player *player);
void use_lamp(Player *player, GameState *game);
void use_flare(Player *player, GameState *game);
void open_chest(Player *player, GameState *game);
void drink_from_pool(Player *player);

// Magic and special abilities
void cast_spell(Player *player, GameState *game, char spell_type);
void teleport(Player *player, GameState *game);
void gaze_into_orb(Player *player, GameState *game);

// Utility functions
int random_number(int max_value);
void print_status(Player *player);
void display_map(GameState *game, int current_level);
void print_help();

// Game ending functions
int check_game_over(Player *player);
void end_game(Player *player, GameState *game);

// Input/Output functions
char get_user_input();
void print_message(const char *message);
void printStars(void);

void open_book(Player *player, GameState *game);

const char* get_race_name(int race);
#endif 
