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

Button::Button(int x, int y, LTexture* buttonTexture)
{
	mButtonW = buttonTexture->getWidth();
	mButtonH = buttonTexture->getHeight();
	mButtonTexture = buttonTexture;
	mPosition.x = x;
	mPosition.y = y;
	mCurrentSprite = BUTTON_SPRITE_DEFAULT;
	mButtonSpriteClips = new SDL_Rect[3];
}

void Button::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		int x,y;
		SDL_GetMouseState(&x,&y);
		bool inside = true;
		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + mButtonW)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + mButtonH)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_DEFAULT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e.type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_HOVER;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_CLICK;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_DEFAULT;
				break;
			}
		}
	}
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
