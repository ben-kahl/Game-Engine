#ifndef CORE_CPP
#define CORE_CPP


#include "Core.h"
#include "Character.h"
#include "Level.h"

//The window we'll be rendering to
LWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Globally used font
TTF_Font* gFont = nullptr;



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

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image %s! SDL_image Error: %s\n" << path.c_str() << IMG_GetError();
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture from %s! SDL Error: %s\n" << path.c_str() << SDL_GetError();
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			std::cout << "Unable to create texture from rendered text! SDL Error: %s\n" << SDL_GetError();
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		std::cout << "Unable to render text surface! SDL_ttf Error: %s\n" << TTF_GetError();
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
	return mPaused && mStarted;
}

LWindow::LWindow()
{
	//Initialize non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
}

bool LWindow::init()
{
	//Create window
	mWindow = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (mWindow != NULL)
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;
	}
	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer()
{
	return SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void LWindow::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_WINDOWEVENT)
	{
		bool updateCaption = false;


		switch (e.window.event)
		{
			//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent(gRenderer);
			break;

			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(gRenderer);
			break;
			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
			break;
			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			//Window maximized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
		}

		if (updateCaption)
		{
			std::stringstream caption;
			caption << "SDL Tutorial - MouseFocus:" << ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle(mWindow, caption.str().c_str());
		}
		//Enter exit full screen on return key
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
		{
			if (mFullScreen)
			{
				SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
				mFullScreen = false;
			}
			else
			{
				SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
				mFullScreen = true;
				mMinimized = false;
			}
		}
	}
}

int LWindow::getWidth()
{
	return mWidth;
}

int LWindow::getHeight()
{
	return mHeight;
}

bool LWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
	return mMinimized;
}

void LWindow::free()
{
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
}

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