#ifndef BUTTON_H
#define BUTTON_H
#include "Texture.h"

enum ButtonSprite
{
	BUTTON_SPRITE_DEFAULT = 0,
	BUTTON_SPRITE_HOVER = 1,
	BUTTON_SPRITE_CLICK = 2
};

class Button
{
public:
	Button();
	Button(int x, int y, LTexture* buttonTexture);
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
	//Button sprite
	ButtonSprite mCurrentSprite;
	//Button sprite clip
	SDL_Rect* mButtonSpriteClips;
	//Button Texture
	LTexture* mButtonTexture;
};
#endif // !BUTTON_H
