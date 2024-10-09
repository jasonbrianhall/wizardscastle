#include <stdio.h>
#include <stdlib.h>
#include "wizards-castle.h"

int main(void)
{
	Player player;
	GameState game;
        initialize_player(&player);
	initialize_game(&game);

	print_introduction();
	choose_race(&player);
	print_message("\n");
	choose_sex(&player);
	allocate_attributes(&player);
	buy_equipment(&player);
	buy_lamp_and_flares(&player);
	generate_castle(&game);
	main_game_loop(&player, &game);
	
	return 0;
}
