#include "HealthBar.h"

HealthBar::HealthBar()
{
	HEALTH_BAR_WIDTH = 0;
	HEALTH_BAR_HEIGHT = 0;

	mPosX = 0;
	mPosY = 0;

	outlineTop.x = 0;
	outlineTop.y = 0;
	outlineTop.w = 0;
	outlineTop.h = 0;

	outlineBottom.x = 0;
	outlineBottom.y = 0;
	outlineBottom.w = 0;
	outlineBottom.h = 0;

	outlineLeft.x = 0;
	outlineLeft.y = 0;
	outlineLeft.w = 0;
	outlineLeft.h = 0;

	outlineRight.x = 0;
	outlineRight.y = 0;
	outlineRight.w = 0;
	outlineRight.h = 0;

	fill.x = 0;
	fill.y = 0;
	fill.w = 0;
	fill.h = 0;
}

HealthBar::HealthBar(int x, int y, int w, int h) {
	HEALTH_BAR_WIDTH = w;
	HEALTH_BAR_HEIGHT = h;

	mPosX = x;
	mPosY = y;
	
	outlineTop.x = x;
	outlineTop.y = y;
	outlineTop.w = w;
	outlineTop.h = 3;

	outlineBottom.x = x;
	outlineBottom.y = (y + h) - 3;
	outlineBottom.w = w;
	outlineBottom.h = 3;

	outlineLeft.x = x;
	outlineLeft.y = y;
	outlineLeft.w = 3;
	outlineLeft.h = h;

	outlineRight.x = (x+w) - 3;
	outlineRight.y = y;
	outlineRight.w = 3;
	outlineRight.h = h;

	fill.x = x;
	fill.y = y;
	fill.w = w;
	fill.h = h;
}

void HealthBar::setPercent(double per) {
	fill.w = HEALTH_BAR_WIDTH * per;
}

void HealthBar::setPos(int x, int y) {
	mPosX = x;
	mPosY = y;
	fill.x = mPosX;
	fill.y = mPosY;

	outlineTop.x = mPosX;
	fill.y = mPosY;
}

void HealthBar::render() {
	
	//Render red health rectangle
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &fill);

	//Render black outlineTop rectangle
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderFillRect(gRenderer, &outlineTop);
	SDL_RenderFillRect(gRenderer, &outlineBottom);
	SDL_RenderFillRect(gRenderer, &outlineLeft);
	SDL_RenderFillRect(gRenderer, &outlineRight);

}