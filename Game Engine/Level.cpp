#ifndef LEVEL_CPP
#define LEVEL_CPP

#include "Level.h"

Level::Level(LTexture* levelTexture)
{
	mLevW = levelTexture->getWidth();
	mLevH = levelTexture->getHeight();
	mLevelTexture = levelTexture;
}

Level::Level()
{
	mLevW = 0;
	mLevH = 0;
	mLevelTexture = nullptr;
}

Level::~Level()
{
	mLevW = 0;
	mLevH = 0;
	//mLevelTexture->free();
}

bool Level::init(LTexture* levelTexture)
{
	if (levelTexture == nullptr)
	{
		return false;
	}
	mLevW = levelTexture->getWidth();
	mLevH = levelTexture->getHeight();
	mLevelTexture = levelTexture;
	return true;
}

int Level::getLevW()
{
	return mLevW;
}

int Level::getLevH()
{
	return mLevH;
}

void Level::render()
{
	mLevelTexture->render(0,0);
}

void Level::render(SDL_Rect* camera)
{
	mLevelTexture->render(0, 0, camera);
}
#endif // !LEVEL_CPP"