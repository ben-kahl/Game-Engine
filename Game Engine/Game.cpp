#ifndef GAME_CPP
#define GAME_CPP

#include "Game.h"

LTimer fpsTimer;

LTimer gameTimer;

LTimer roundTimer;

std::stringstream roundTimerText;

int roundTime;

std::stringstream timeText;

int countedFrames;

float avgFPS;

int frameTicks;

SDL_Rect camera;

SDL_Event e;

SDL_Color textColor;

SDL_Rect scalar;

void initGame()
{

	gameTimer.start();

	//Set text color as white
	textColor = { 255, 255, 255, 255 };

	roundTime = 0;

	//Start counting frames per second
	countedFrames = 0;
	fpsTimer.start();
	avgFPS = 0;
	frameTicks = 0;
	//Render camera
	camera = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	application.gameState = GameState::Title;
	scalar = { 0, 0, 64, 205 };
}

void update(AssetManager* assets)
{
	//Clear Screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	switch (application.gameState)
	{
	case GameState::Title:
		//Render background
		//levels[GameState::Title].render();
		assets->levels[Title].render();
		assets->buttons[0].render();
		//gTitleBackgroundTexture.render(0, 0);
		break;
	case GameState::CharacterSelect:
		assets->levels[CharacterSelect].render();
		assets->healthBars[0].setPercent(0.1);
		//gCharaSelBackgroundTexture.render(0, 0);
		break;
	case GameState::StageSelect:
		//gStageSelBackgroundTexture.render(0, 0);
		assets->levels[StageSelect].render();
		assets->healthBars[0].setPercent(0.5);
		break;
	case GameState::Fight:
		//Character stuff
		assets->characters[0].move(assets->characters[1].getColliders());
		//update character camera
		camera.x = (assets->characters[0].getPosX() + assets->characters[0].CHARA_WIDTH / 2) - SCREEN_WIDTH / 2;
		camera.y = (assets->characters[0].getPosY() + assets->characters[0].CHARA_HEIGHT / 2) - SCREEN_HEIGHT / 2;

		//Keep camera over level
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.x > assets->levels[Fight].getLevW() - camera.w)
		{
			camera.x = assets->levels[Fight].getLevW() - camera.w;
		}
		if (camera.y > assets->levels[Fight].getLevH() - camera.h)
		{
			camera.y = assets->levels[Fight].getLevH() - camera.h;
		}
		//Render background
		assets->levels[Fight].render(&camera);
		//Render dots
		assets->characters[0].render(&scalar);
		assets->characters[1].render();

		//Render round clock
		roundTime = roundTimer.getTicks() / 1000;
		roundTimerText.str("");
		roundTimerText << 99 - roundTime;
		assets->roundTimerTexture.loadFromRenderedText(roundTimerText.str().c_str(), textColor, assets->font);
		assets->roundTimerTexture.render(920, 20);
		//Render health bars
		assets->healthBars[0].render();
		assets->healthBars[1].render();
		if (roundTime - 99 == 0)
		{
			application.gameState = Title;
		}
		break;
	}

	//Render text
	if (!assets->fontTexture.loadFromRenderedText(timeText.str().c_str(), textColor, assets->font))
	{
		printf("Unable to render FPS texture!\n");
	}

	//Render fps info in bottom left
	assets->fontTexture.render(0, 1050);

	//Update screen
	SDL_RenderPresent(gRenderer);

}

void gameLoop(AssetManager* assets)
{
	gameTimer.start();
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			application.quit = true;
		}
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_LEFT: application.gameState = GameState::Title;
				break;
			case SDLK_RIGHT: application.gameState = GameState::Fight;
				assets->characters[0].setPos(assets->levels[3].getLevW() / 3, assets->levels[3].getLevH() / 1.4);
				roundTimer.start();
				break;
			case SDLK_UP: application.gameState = GameState::StageSelect;
				break;
			case SDLK_DOWN: application.gameState = GameState::CharacterSelect;
				break;
			case SDLK_o:
				scalar.w -= 5;
				scalar.h -= 5;
				break;
			case SDLK_p:
				scalar.w += 5;
				scalar.h += 5;
				break;
			}
		}
		if (application.gameState == GameState::Fight)
		{
			//handle character input
			assets->characters[0].handleEvent(e);
		}
		if (application.gameState == GameState::Title)
		{
			//Handle button
			assets->buttons[0].handleEvent(e);
		}
		//handle window events (dont really need this atm)
		application.win.handleEvent(e);
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

	//Update and render objects
	update(assets);

	countedFrames++;
	frameTicks = gameTimer.getTicks();
	//vsync delay
	if (frameTicks < SCREEN_TICKS_PER_FRAME)
	{
		SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
	}
}
#endif // !GAME_CPP