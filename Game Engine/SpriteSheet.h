#pragma once
#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include "Texture.h"
#include <SDL.h>

class SpriteSheet
{
public:
	SpriteSheet(LTexture spriteSheet, int row, int column);
	SpriteSheet();
	~SpriteSheet();
	void select_sprite(int x, int y);
	void render(int x, int y);
	int getWidth();
	int getHeight();
private:
	LTexture mSpriteSheet;
	SDL_Rect mClip; 
};
#endif // !SPRITESHEET_H