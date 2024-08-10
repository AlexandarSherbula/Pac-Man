#include "Game.h"

int main()
{
	Game pacman;
	if (pacman.Construct(800, 600, 1, 1, false, true))
		pacman.Start();
	return 0;
}