#include <stdio.h>
#include <stdlib.h>
#include "wizards-castle.h"

int main(void)
{
	Player player;
	GameState game;
        initialize_player(&player);

	print_introduction();
	choose_race(&player);
	print_message("\n");
	choose_sex(&player);
	buy_equipment(&player);
	return 0;
}
