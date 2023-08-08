#ifndef LEVEL_CPP
#define LEVEL_CPP

#include "Level.h"

Level::Level(LTexture* levelTexture)
{
	mLevW = levelTexture->getWidth();
	mLevH = levelTexture->getHeight();
	mLevelTexture = levelTexture;
}

Level::~Level()
{
	mLevW = 0;
	mLevH = 0;
	mLevelTexture->free();
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
	mLevelTexture->render(camera->x, camera->y);
}
#endif // !LEVEL_CPP"