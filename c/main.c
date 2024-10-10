#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "wizards-castle.h"

int main(void)
{
	Player player;
	GameState game;
	bool playagain=1;
        while (playagain) 
        {
		initialize_player(&player);
		print_introduction();
		choose_race(&player);
		print_message("\n");
		choose_sex(&player);
		allocate_attributes(&player);
		buy_equipment(&player);
		buy_lamp_and_flares(&player);
		generate_castle(&game);
		playagain=main_game_loop(&player, &game);
	}
	return 0;
}
