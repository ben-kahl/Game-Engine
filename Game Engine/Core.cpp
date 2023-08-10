#ifndef CORE_CPP
#define CORE_CPP


#include "Core.h"
#include "Character.h"
#include "Level.h"
#include "Texture.h"
#include "Window.h"
#include "Timer.h"

enum GameState
{
	Title = 0, // Title Screen
	CharacterSelect = 1, // Character Select Screen
	StageSelect = 2, // Stage Select Screen
	Fight = 3 // Actual gameplay state
};



//Globals
//The window we'll be rendering to
LWindow gWindow;

//Renderer
SDL_Renderer* gRenderer;

//Global font
TTF_Font* gFont;

//Scene textures
LTexture gFPSTextTexture;
LTexture gBackgroundTexture;
LTexture gTitleBackgroundTexture;
LTexture gCharaSelBackgroundTexture;
LTexture gStageSelBackgroundTexture;
LTexture gStageBackgroundTexture;
LTexture gCharacterTexture;
LTexture gCharacterCollisionTestTexture;

//Global gamestate
GameState gGameState = GameState::Title;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		if (!(gWindow.init()))
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = gWindow.createRenderer();
			if (gRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("Fonts/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	if (!gTitleBackgroundTexture.loadFromFile("Textures/holyfuckthatssick.jpg"))
	{
		std::cout << "Failed to load title background texture!\n";
		success = false;
	}

	if (!gStageBackgroundTexture.loadFromFile("Textures/stage1Background.png"/*"Textures/characterSelectBackground.png"*/))
	{
		std::cout << "Failed to load character select background texture!\n";
		success = false;
	}

	if (!gStageSelBackgroundTexture.loadFromFile("Textures/canvas.png"/*"Textures/stageSelectbackground.png"*/))
	{
		std::cout << "Failed to load stage select background texture!\n";
		success = false;
	}

	if (!gCharaSelBackgroundTexture.loadFromFile("Textures/gillboyle.jpg"))
	{
		std::cout << "Failed to load character select background texture!\n";
		success = false;
	}
	if (!gCharacterTexture.loadFromFile("Textures/gillboyle.jpg"))
	{
		std::cout << "Failed to load character texture!\n";
		success = false;
	}
	if (!gCharacterCollisionTestTexture.loadFromFile("Textures/dot.bmp"))
	{
		std::cout << "Failed to load dot texture! \n";
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	gFPSTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void update(Character* p1, Character* p2, SDL_Rect* camera, Level* currentLevel)
{

	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	switch (gGameState)
	{
	case GameState::Title:
		//Render background
		currentLevel->render();
		//gTitleBackgroundTexture.render(0, 0);
		break;
	case GameState::CharacterSelect:
		currentLevel->render();
		//gCharaSelBackgroundTexture.render(0, 0);
		break;
	case GameState::StageSelect:
		//gStageSelBackgroundTexture.render(0, 0);
		currentLevel->render();
		break;
	case GameState::Fight:
		//Character stuff
		p1->move(p2->getColliders());
		//update character camera
		camera->x = (p1->getPosX() + p1->CHARA_WIDTH / 2) - SCREEN_WIDTH / 2;
		camera->y = (p1->getPosY() + p1->CHARA_HEIGHT / 2) - SCREEN_HEIGHT / 2;

		//Keep camera over level
		if (camera->x < 0)
		{
			camera->x = 0;
		}
		if (camera->y < 0)
		{
			camera->y = 0;
		}
		if (camera->x > currentLevel->getLevW() - camera->w)
		{
			camera->x = currentLevel->getLevW() - camera->w;
		}
		if (camera->y > currentLevel->getLevH() - camera->h)
		{
			camera->y = currentLevel->getLevH() - camera->h;
		}
		//Render background
		currentLevel->render(camera);
		//Render dots
		p1->render();
		p2->render();
		break;
	}


	//Render fps info in bottom left
	gFPSTextTexture.render(0, 1050);
	//Update screen
	SDL_RenderPresent(gRenderer);
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		std::cout << "Failed to initialize!\n";
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			std::cout << "Failed to load media!\n";
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Set text color as white
			SDL_Color textColor = { 255, 255, 255, 255 };

			//The frames per second timer
			LTimer fpsTimer;
			LTimer capTimer;
			//In memory text stream
			std::stringstream timeText;

			//Start counting frames per second
			int countedFrames = 0;
			fpsTimer.start();
			float avgFPS = 0;
			int frameTicks = 0;
			//Render camera
			SDL_Rect camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
			//Test characters
			Character test(0, 0,/*LEVEL_WIDTH / 2, SCREEN_HEIGHT / 1.4,*/ &gCharacterCollisionTestTexture);
			Character collisionTest(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &gCharacterCollisionTestTexture);
			//Initialize Levels
			Level title(&gTitleBackgroundTexture);
			Level characterSelect(&gCharaSelBackgroundTexture);
			Level stageSelect(&gStageSelBackgroundTexture);
			Level stage(&gStageBackgroundTexture);
			Level* currentLevel = &title;
			//While application is running
			while (!quit)
			{
				//Do processes while not minimized
				if (!gWindow.isMinimized())
				{
					capTimer.start();
					//Handle events on queue
					while (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}
						if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_LEFT: gGameState = GameState::Title;
								currentLevel = &title;
								break;
							case SDLK_RIGHT: gGameState = GameState::Fight;
								currentLevel = &stage;
								break;
							case SDLK_UP: gGameState = GameState::StageSelect;
								currentLevel = &stageSelect;
								break;
							case SDLK_DOWN: gGameState = GameState::CharacterSelect;
								currentLevel = &characterSelect;
								break;
							}
						}
						//handle character input
						test.handleEvent(e);
						//handle window events (dont really need this atm)
						gWindow.handleEvent(e);
					}
					//Calculate and correct fps
					avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
					if (avgFPS > 2000000)
					{
						avgFPS = 0;
					}

					//Set text to be rendered
					timeText.str("");
					timeText << "FPS: " << avgFPS;

					//Render text
					if (!gFPSTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
					{
						printf("Unable to render FPS texture!\n");
					}
					//Update and render objects
					update(&test, &collisionTest, &camera, currentLevel);
					countedFrames++;
					frameTicks = capTimer.getTicks();
					//vsync delay
					if (frameTicks < SCREEN_TICKS_PER_FRAME)
					{
						SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
					}
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

#endif // !CORE_CPP