#ifndef BUTTON_H
#define BUTTON_H
#include "SpriteSheet.h"

class Button
{
public:
	Button();
	Button(int x, int y, SpriteSheet* buttonSpriteSheet);
	//Handle mouse/controller events
	void handleEvent(SDL_Event& e);
	//Position Setter
	void setPosition(int x, int y);
	//Render
	void render();
	//Getters
	int getH();
	int getW();
private:
	//Button width
	int mButtonW;
	//Button height
	int mButtonH;
	//Button position
	SDL_Point mPosition;
	//Spritesheet
	SpriteSheet* mButtonSpriteSheet;
};
#endif // !BUTTON_H
