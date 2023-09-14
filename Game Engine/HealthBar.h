#pragma once
#ifndef HEALTH_BAR_H
#include <SDL.h>
#include "Application.h"

class HealthBar
{
public:
	
	HealthBar();
	HealthBar(int x, int y, int w, int h);
	
	int HEALTH_BAR_WIDTH;
	int HEALTH_BAR_HEIGHT;

	void setPercent(double per);
	void setPos(int x, int y);

	void render();
private:
	int mPosX;
	int mPosY;	

	SDL_Rect outlineTop;
	SDL_Rect outlineBottom;
	SDL_Rect outlineLeft;
	SDL_Rect outlineRight;
	SDL_Rect fill;
};

#endif // !HEALTH_BAR_H
