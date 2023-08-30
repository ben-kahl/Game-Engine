#include "Application.h"
#include "Game.h"

AssetManager assets;

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		std::cout << "Failed to initialize!\n";
	}
	else
	{
		if (!assets.initAssets())
		{
			application.quit = true;
		}
		initGame();
		while (!application.quit)
		{
			gameLoop(&assets);
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}