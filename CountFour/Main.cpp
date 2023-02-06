#include "Game.h"

int main(int, char**)
{
	Game game;
	if (game.Initialize())
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}