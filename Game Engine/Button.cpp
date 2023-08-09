#ifndef BUTTON_CPP
#define BUTTON_CPP
#include "Button.h"

Button::Button()
{
	mButtonW = 0;
	mButtonH = 0;
	mButtonTexture = nullptr;
	mPosition.x = 0;
	mPosition.y = 0;
	mCurrentSprite = BUTTON_SPRITE_DEFAULT;
	mButtonSpriteClips = new SDL_Rect[3];
}

void Button::handleEvent(SDL_Event* e)
{

}

void Button::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void Button::render()
{
	mButtonTexture->render(mPosition.x, mPosition.y, &mButtonSpriteClips[mCurrentSprite]);
}

int Button::getW()
{
	return mButtonW;
}

int Button::getH()
{
	return mButtonH;
}

#endif // !BUTTON_CPP
