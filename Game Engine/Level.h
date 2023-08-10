#ifndef LEVEL_H
#define LEVEL_H

#include "Core.h"
#include "Texture.h"

class Level
{
public:
	//Constructor and Destructor
	Level(LTexture* levelTexture);
	Level();
	~Level();
	
	//Render Level at (0,0)
	void render();
	//Render level based on camera
	void render(SDL_Rect* camera);
	//Get level width
	int getLevW();
	//Get level height
	int getLevH();
private:
	//Level Size
	int mLevW;
	int mLevH;
	
	//Level Texture
	LTexture* mLevelTexture;
};


#endif // !LEVEL_H
