#ifndef BUTTON_CPP
#define BUTTON_CPP
#include "Button.h"

Button::Button()
{
	mButtonW = 0;
	mButtonH = 0;
	mButtonSpriteSheet = nullptr;
	mPosition.x = 0;
	mPosition.y = 0;
}

Button::Button(int x, int y, SpriteSheet* buttonSpriteSheet)
{
	mButtonW = buttonSpriteSheet->getWidth();
	mButtonH = buttonSpriteSheet->getHeight();
	mButtonSpriteSheet = buttonSpriteSheet;
	mPosition.x = x;
	mPosition.y = y;
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
			mButtonSpriteSheet->select_sprite(0,0);
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e.type)
			{
			case SDL_MOUSEMOTION:
				mButtonSpriteSheet->select_sprite(0, 1);
				break;

			case SDL_MOUSEBUTTONDOWN:
				mButtonSpriteSheet->select_sprite(0, 2);
				break;

			case SDL_MOUSEBUTTONUP:
				mButtonSpriteSheet->select_sprite(0, 0);
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
	mButtonSpriteSheet->render(mPosition.x, mPosition.y);
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
