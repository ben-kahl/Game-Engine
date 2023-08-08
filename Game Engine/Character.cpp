#ifndef CHARACTER_CPP
#define CHARACTER_CPP
#include "Character.h"



Character::Character(int x, int y, LTexture* charaTexture)
{
	CHARA_WIDTH = charaTexture->getWidth();
	CHARA_HEIGHT = charaTexture->getHeight();
	mPosX = x;
	mPosY = y;
	//initialize with one collision box, set to the dimensions of the character
	mColliders.resize(1);
	mColliders[0].w = CHARA_WIDTH;
	mColliders[0].h = CHARA_HEIGHT;
	shiftColliders();

	//Set Character Texture
	mCharaTexture = charaTexture;
}

void Character::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_w: mVelY -= CHARA_VEL;
			break;
		case SDLK_s: mVelY += CHARA_VEL;
			break;
		case SDLK_a: mVelX -= CHARA_VEL;
			break;
		case SDLK_d: mVelX += CHARA_VEL;
			break;
		}
	}
	//key release
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//negate velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_w: mVelY += CHARA_VEL;
			break;
		case SDLK_s: mVelY -= CHARA_VEL;
			break;
		case SDLK_a: mVelX += CHARA_VEL;
			break;
		case SDLK_d: mVelX -= CHARA_VEL;
			break;
		}
	}
}

bool Character::checkCollision(std::vector<SDL_Rect>& a)
{
	//Sides
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	//Set sides
	leftA = a[0].x;
	rightA = a[0].x + a[0].w;
	topA = a[0].y;
	bottomA = a[0].y + a[0].h;

	leftB = mPosX;
	rightB = mPosX + CHARA_WIDTH;
	topB = mPosY;
	bottomB = mPosY + CHARA_HEIGHT;

	if (bottomA <= topB)
	{
		return false;
	}
	if (topA >= bottomB)
	{
		return false;
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

void Character::move(std::vector<SDL_Rect>& otherCharaColliders)
{
	mPosX += mVelX;
	shiftColliders();
	//If the character went too far to the left or right
	if ((mPosX < 0) || (mPosX + CHARA_WIDTH > SCREEN_WIDTH) || checkCollision(otherCharaColliders))
	{
		//Move back
		mPosX -= mVelX;
		shiftColliders();
	}

	//Move the character up or down
	mPosY += mVelY;
	shiftColliders();
	//If the character went too far up or down
	if ((mPosY < 0) || (mPosY + CHARA_HEIGHT > SCREEN_HEIGHT) || checkCollision(otherCharaColliders))
	{
		//Move back
		mPosY -= mVelY;
		shiftColliders();
	}
}

void Character::shiftColliders()
{
	//row offset
	int r = 0;

	//Go through collision boxes
	for (int i = 0; i < mColliders.size(); ++i)
	{
		//center box
		mColliders[i].x = mPosX + (CHARA_WIDTH - mColliders[i].w) / 2;
		//set box at row offset
		mColliders[i].y = mPosY + r;
		//move row offset down the height of collision box
		r += mColliders[i].h;
	}
}

std::vector<SDL_Rect>& Character::getColliders()
{
	return mColliders;
}

int Character::getPosX()
{
	return mPosX;
}

int Character::getPosY()
{
	return mPosY;
}

void Character::render()
{
	//Show the character
	mCharaTexture->render(mPosX, mPosY);
}

void Character::render(Uint8 r, Uint8 g, Uint8 b)
{
	mCharaTexture->setColor(r, g, b);
	mCharaTexture->render(mPosX, mPosY);
}

#endif // !CHARACTER_CPP