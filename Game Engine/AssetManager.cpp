#include "AssetManager.h"

AssetManager::AssetManager()
{
	font = nullptr;
}

AssetManager::~AssetManager()
{
	font = nullptr;
	fontTexture.free();
}


bool AssetManager::loadFont(std::string path, int size)
{
	//Texture to be loaded
	font = TTF_OpenFont(path.c_str(), size);
	if (font == nullptr)
	{
		std::cout << "Failed to load new font!" << std::endl;
		return false;
	}
	return true;
}

bool AssetManager::initAssets()
{	
	bool success = false;
	if (!loadFont("Fonts/lazy.ttf", 28))
	{
		return success;
	}
	
	HealthBar p1Bar(50, 10, 800, 50);
	healthBars.push_back(p1Bar);
	HealthBar p2Bar(1070, 10, 800, 50);
	healthBars.push_back(p2Bar);

	if (!titleTexture.loadFromFile("Textures/testBackground.jpg"))
	{
		return success;
	}

	Level title(&titleTexture);
	levels.push_back(title);

	
	if (!charaSelTexture.loadFromFile("Textures/canvas.png"))
	{
		return success;
	}
	Level characterSelect(&charaSelTexture);
	levels.push_back(characterSelect);
	
	
	if (!stageSelTexture.loadFromFile("Textures/bg.png"))
	{
		return success;
	}
	Level stageSelect(&stageSelTexture);
	levels.push_back(stageSelect);

	
	if (!stage1Texture.loadFromFile("Textures/stage1Background.png"))
	{
		return success;
	}
	Level stage1(&stage1Texture);
	levels.push_back(stage1);

	if (!p1Texture.loadFromFile("Textures/foo.png"))
	{
		return success;
	}
	SpriteSheet p1SpriteSheet(p1Texture, 1, 4);
	spriteSheets.push_back(p1SpriteSheet);

	if (!dotTexture.loadFromFile("Textures/dot.bmp"))
	{
		return success;
	}
	SpriteSheet dotSpriteSheet(dotTexture, 1, 1);
	spriteSheets.push_back(dotSpriteSheet);

	if (!buttonTexture.loadFromFile("Textures/button.png"))
	{
		return success;
	}
	SpriteSheet buttonSpriteSheet(buttonTexture, 4, 1);
	spriteSheets.push_back(buttonSpriteSheet);

	Button button(SCREEN_WIDTH /2, SCREEN_HEIGHT /2, &spriteSheets[2]);
	buttons.push_back(button);
	Character p1(levels[3].getLevW() / 2, SCREEN_HEIGHT / 1.4, &spriteSheets[0]);
	characters.push_back(p1);

	Character p2(levels[3].getLevW() / 2, SCREEN_HEIGHT / 2, &spriteSheets[1]);
	characters.push_back(p2);

	success = true;
	return success;
}