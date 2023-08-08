//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 30;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class LTimer
{
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Walking animation
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
LTexture gSpriteSheetTexture;


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
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
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
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED /* | SDL_RENDERER_PRESENTVSYNC*/);
			if (gRenderer == NULL)
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
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if (!gSpriteSheetTexture.loadFromFile("foo.png"))
	{
		printf("Failed to load walking animation texture!\n");
		success = false;
	}
	else
	{
		//Set sprite clips
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 64;
		gSpriteClips[0].h = 205;

		gSpriteClips[1].x = 64;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 64;
		gSpriteClips[1].h = 205;

		gSpriteClips[2].x = 128;
		gSpriteClips[2].y = 0;
		gSpriteClips[2].w = 64;
		gSpriteClips[2].h = 205;

		gSpriteClips[3].x = 192;
		gSpriteClips[3].y = 0;
		gSpriteClips[3].w = 64;
		gSpriteClips[3].h = 205;
	}

	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Current animation frame
			int frame = 0;
			
			//rotation var
			double degrees = 0;
			
			//FPS Cap timers
			LTimer fpsTimer;
			LTimer capTimer;
			//set flip type
			SDL_RendererFlip flipType = SDL_FLIP_NONE; 
			
			//screwing around with basic movement
			int posX = 0;
			int posY = 0;
			
			//Frame counter
			int countedFrames = 0;
			fpsTimer.start();
			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					capTimer.start();
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
							case SDLK_a:
							degrees -=60;
							break;
							case SDLK_d:
							degrees += 60;
							break;
							case SDLK_q:
							flipType = SDL_FLIP_HORIZONTAL;
							break;
							case SDLK_w:
							flipType = SDL_FLIP_NONE;
							break;
							case SDLK_e:
							flipType = SDL_FLIP_VERTICAL;
							break;
							case SDLK_LEFT:
							if (posX <= 0) {
								posX = 0;
							}
							posX-= 10;	
							break;
							case SDLK_RIGHT:
							posX+=10;
							if (posX >= SCREEN_WIDTH) {
								posX = SCREEN_WIDTH;
							}
							break;
							case SDLK_UP:
							if (posY >= SCREEN_HEIGHT)
							{
								posY = SCREEN_HEIGHT;
							}
							posY -= 10;
							break;
							case SDLK_DOWN:
							if (posY <= 0)
							{
								posY = 0;
							}
							posY += 10;
							break;
						}
					}
				}
				float FPS = countedFrames / (fpsTimer.getTicks()/ 1000.f);
				if (FPS > 200000)
				{
					FPS = 0;
				}
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render current frame
				SDL_Rect* currentClip = &gSpriteClips[frame / 4];
				gSpriteSheetTexture.render(posX, posY, currentClip, degrees, NULL, flipType);

				//Update screen
				SDL_RenderPresent(gRenderer);

				//Go to next frame
				frame++;

				//Cycle animation
				if (frame / 4 >= WALKING_ANIMATION_FRAMES)
				{
					frame = 0;
				}
				countedFrames++;
				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICKS_PER_FRAME)
				{
					SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
