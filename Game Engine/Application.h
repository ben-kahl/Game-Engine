#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

enum GameState
{
	Title = 0, // Title Screen
	CharacterSelect = 1, // Character Select Screen
	StageSelect = 2, // Stage Select Screen
	Fight = 3 // Actual gameplay state
};

class LWindow
{
public:
	//Intializes internals
	LWindow();

	//Creates window
	bool init();

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

private:
	//Window data
	SDL_Window* mWindow;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
};

//Wrapper for fundamental app functions
struct app
{
	bool quit;
	LWindow win;
	GameState gameState;
};

//Declarations 
//Main app handler
extern app application;

//The window renderer
extern SDL_Renderer* gRenderer;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();
#endif // !APPLICATION_H