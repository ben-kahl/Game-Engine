#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <vector>
#include "SpriteSheet.h"

class Character
{
public:
	//Character Dimensions
	int CHARA_WIDTH;
	int CHARA_HEIGHT;
	//Set max velocity
	static const int CHARA_VEL = 10;

	//Constructor
	Character(int x, int y, SpriteSheet* charaSpriteSheet);
	Character();
	~Character();
	//Process key presses
	void handleEvent(SDL_Event& e);

	//Move character
	void move(std::vector<SDL_Rect>&);

	//Shows character on screen based on x and y coords
	void render();
	//Renders character based on camera
	void render(int x, int y);
	//Render with scale
	void render(SDL_Rect* scalar);
	//render with alt color
	//void render(Uint8 r, Uint8 g, Uint8 b);

	//Collision checker (Rects only)
	bool checkCollision(std::vector<SDL_Rect>&);

	//Getters
	int getPosX();
	int getPosY();
	std::vector<SDL_Rect>& getColliders();
private:
	//X and Y offsets
	int mPosX, mPosY;

	//Velocity of character
	int mVelX, mVelY;

	//Character Texture
	SpriteSheet* mCharaSpriteSheet;

	//Collision (hit) boxes
	std::vector<SDL_Rect> mColliders;

	//Move collision boxes relative to character offset
	void shiftColliders();
};

#endif // !CHARACTER_H