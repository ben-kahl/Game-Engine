#pragma once
#ifndef GAME_H
#define GAME_H
#include "Timer.h"
#include "AssetManager.h"

#include <sstream>
#include <vector>

void initGame();

void gameLoop(AssetManager* assets);

void update(AssetManager* assets);

#endif // !GAME_H
