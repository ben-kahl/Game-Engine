#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include <string>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Level.h"
#include "SpriteSheet.h"
#include "Character.h"
#include "Button.h"

//Load new 
//LTexture* loadMedia(std::string path);

class AssetManager
{
public:
	AssetManager();
	~AssetManager();
	
	bool loadFont(std::string path, int size);

	bool initAssets();

	TTF_Font* font;
	
	std::vector<Level> levels;
	
	std::vector<SpriteSheet> spriteSheets;

	std::vector<Character> characters;

	std::vector<Button> buttons;
	
	LTexture fontTexture;

private:
	LTexture titleTexture;

	LTexture charaSelTexture;

	LTexture stageSelTexture;

	LTexture stage1Texture;

	LTexture p1Texture;

	LTexture buttonTexture;
};

#endif // !ASSET_MANAGER_H
