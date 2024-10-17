#include "wizards-castle.h"
#include <stdio.h>
#include <string.h>

bool save_game(const char *filename, const Player *player, const GameState *game) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }

    // Save Player structure
    fprintf(file, "race:%d;\n", player->race);
    fprintf(file, "sex:%d;\n", player->sex);
    fprintf(file, "strength:%d;\n", player->strength);
    fprintf(file, "temp_strength:%d;\n", player->temp_strength);
    fprintf(file, "intelligence:%d;\n", player->intelligence);
    fprintf(file, "temp_intelligence:%d;\n", player->temp_intelligence);
    fprintf(file, "dexterity:%d;\n", player->dexterity);
    fprintf(file, "temp_dexterity:%d;\n", player->temp_dexterity);
    fprintf(file, "gold:%d;\n", player->gold);
    fprintf(file, "flares:%d;\n", player->flares);
    fprintf(file, "armor_type:%d;\n", player->armor_type);
    fprintf(file, "armor_points:%d;\n", player->armor_points);
    fprintf(file, "weapon_type:%d;\n", player->weapon_type);
    fprintf(file, "lamp_flag:%d;\n", player->lamp_flag);
    fprintf(file, "x:%d;\n", player->x);
    fprintf(file, "y:%d;\n", player->y);
    fprintf(file, "level:%d;\n", player->level);
    fprintf(file, "runestaff_flag:%d;\n", player->runestaff_flag);
    fprintf(file, "orb_flag:%d;\n", player->orb_flag);
    fprintf(file, "book_flag:%d;\n", player->book_flag);
    fprintf(file, "blindness_flag:%d;\n", player->blindness_flag);
    fprintf(file, "stickybook_flag:%d;\n", player->stickybook_flag);
    fprintf(file, "has_runestaff:%d;\n", player->has_runestaff);
    fprintf(file, "has_orb:%d;\n", player->has_orb);
    fprintf(file, "treasure_count:%d;\n", player->treasure_count);
    fprintf(file, "web_count:%d;\n", player->web_count);

    // Save GameState structure
    fprintf(file, "location_map:");
    for (int i = 0; i < MAP_SIZE; i++) {
        fprintf(file, " %d", game->location_map[i]);
    }
    fprintf(file, ";\n");

    fprintf(file, "discovered_rooms:");
    for (int i = 0; i < MAP_SIZE; i++) {
        fprintf(file, " %d", game->discovered_rooms[i]);
    }
    fprintf(file, ";\n");

    fprintf(file, "treasure:");
    for (int i = 0; i < TREASURE_COUNT; i++) {
        fprintf(file, " %d", game->treasure[i]);
    }
    fprintf(file, ";\n");

    fprintf(file, "orb_location: %d %d %d;\n", game->orb_location[0], game->orb_location[1], game->orb_location[2]);
    fprintf(file, "runestaff_location: %d %d %d;\n", game->runestaff_location[0], game->runestaff_location[1], game->runestaff_location[2]);
    
    fprintf(file, "turn_count:%d;\n", game->turn_count);
    fprintf(file, "monster_count:%d;\n", game->monster_count);
    fprintf(file, "game_over:%d;\n", game->game_over);
    fprintf(file, "victory:%d;\n", game->victory);
    fprintf(file, "vendor_attacked:%d;\n", game->vendor_attacked);

    fclose(file);
    return true;
}

bool load_game(const char *filename, Player *player, GameState *game) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    char line[1024];
    char key[50];
    int value;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^:]:%d;", key, &value) == 2) {
            if (strcmp(key, "race") == 0) player->race = value;
            else if (strcmp(key, "sex") == 0) player->sex = value;
            else if (strcmp(key, "strength") == 0) player->strength = value;
            else if (strcmp(key, "temp_strength") == 0) player->temp_strength = value;
            else if (strcmp(key, "intelligence") == 0) player->intelligence = value;
            else if (strcmp(key, "temp_intelligence") == 0) player->temp_intelligence = value;
            else if (strcmp(key, "dexterity") == 0) player->dexterity = value;
            else if (strcmp(key, "temp_dexterity") == 0) player->temp_dexterity = value;
            else if (strcmp(key, "gold") == 0) player->gold = value;
            else if (strcmp(key, "flares") == 0) player->flares = value;
            else if (strcmp(key, "armor_type") == 0) player->armor_type = value;
            else if (strcmp(key, "armor_points") == 0) player->armor_points = value;
            else if (strcmp(key, "weapon_type") == 0) player->weapon_type = value;
            else if (strcmp(key, "lamp_flag") == 0) player->lamp_flag = value;
            else if (strcmp(key, "x") == 0) player->x = value;
            else if (strcmp(key, "y") == 0) player->y = value;
            else if (strcmp(key, "level") == 0) player->level = value;
            else if (strcmp(key, "runestaff_flag") == 0) player->runestaff_flag = value;
            else if (strcmp(key, "orb_flag") == 0) player->orb_flag = value;
            else if (strcmp(key, "book_flag") == 0) player->book_flag = value;
            else if (strcmp(key, "blindness_flag") == 0) player->blindness_flag = value;
            else if (strcmp(key, "stickybook_flag") == 0) player->stickybook_flag = value;
            else if (strcmp(key, "has_runestaff") == 0) player->has_runestaff = value;
            else if (strcmp(key, "has_orb") == 0) player->has_orb = value;
            else if (strcmp(key, "treasure_count") == 0) player->treasure_count = value;
            else if (strcmp(key, "web_count") == 0) player->web_count = value;
            else if (strcmp(key, "turn_count") == 0) game->turn_count = value;
            else if (strcmp(key, "monster_count") == 0) game->monster_count = value;
            else if (strcmp(key, "game_over") == 0) game->game_over = value;
            else if (strcmp(key, "victory") == 0) game->victory = value;
            else if (strcmp(key, "vendor_attacked") == 0) game->vendor_attacked = value;
        } else if (strncmp(line, "location_map:", 13) == 0) {
            char *ptr = line + 13;
            for (int i = 0; i < MAP_SIZE; i++) {
                sscanf(ptr, "%d", &game->location_map[i]);
                ptr = strchr(ptr, ' ');
                if (ptr == NULL) break;
                ptr++;
            }
        } else if (strncmp(line, "discovered_rooms:", 17) == 0) {
            char *ptr = line + 17;
            for (int i = 0; i < MAP_SIZE; i++) {
                sscanf(ptr, "%d", &game->discovered_rooms[i]);
                ptr = strchr(ptr, ' ');
                if (ptr == NULL) break;
                ptr++;
            }
        } else if (strncmp(line, "treasure:", 9) == 0) {
            char *ptr = line + 9;
            for (int i = 0; i < TREASURE_COUNT; i++) {
                sscanf(ptr, "%d", &game->treasure[i]);
                ptr = strchr(ptr, ' ');
                if (ptr == NULL) break;
                ptr++;
            }
        } else if (strncmp(line, "orb_location:", 13) == 0) {
            sscanf(line + 13, "%d %d %d", &game->orb_location[0], &game->orb_location[1], &game->orb_location[2]);
        } else if (strncmp(line, "runestaff_location:", 19) == 0) {
            sscanf(line + 19, "%d %d %d", &game->runestaff_location[0], &game->runestaff_location[1], &game->runestaff_location[2]);
        }
    }

    fclose(file);
    return true;
}
