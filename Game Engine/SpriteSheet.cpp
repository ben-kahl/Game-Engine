#ifndef SPRITESHEET_CPP
#define SPRITESHEET_CPP
#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(LTexture spriteSheet, int row, int column)
{
	mSpriteSheet = spriteSheet;
	mClip.w = mSpriteSheet.getWidth() / column;
	mClip.h = mSpriteSheet.getHeight() / row;
	mClip.x = 0;
	mClip.y = 0;
}

SpriteSheet::SpriteSheet()
{
	mClip.w = 0;
	mClip.h = 0;
	mClip.x = 0;
	mClip.y = 0;	
}

SpriteSheet::~SpriteSheet()
{
	//mSpriteSheet.free();
}

void SpriteSheet::select_sprite(int x, int y)
{
	mClip.x = x * mClip.w;
	mClip.y = y * mClip.h;
}

void SpriteSheet::render(int x, int y)
{
	mSpriteSheet.render(x, y, &mClip);
}

int SpriteSheet::getWidth()
{
	return mClip.w;
}

int SpriteSheet::getHeight()
{
	return mClip.h;
}
#endif // !SPRITESHEET_CPP
