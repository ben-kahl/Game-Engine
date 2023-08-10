#ifndef SPRITESHEET_CPP
#define SPRITESHEET_CPP
#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(LTexture* spriteSheet, int row, int column)
{
	mSpriteSheet = spriteSheet;
	mClip.w = mSpriteSheet->getWidth() / column;
	mClip.h = mSpriteSheet->getHeight() / row;
}

SpriteSheet::~SpriteSheet()
{
	mSpriteSheet->free();
}

void SpriteSheet::select_sprite(int x, int y)
{
	mClip.x = x * mClip.w;
	mClip.y = y * mClip.h;
}

void SpriteSheet::render(int x, int y)
{
	mSpriteSheet->render(x, y, &mClip);
}
#endif // !SPRITESHEET_CPP
