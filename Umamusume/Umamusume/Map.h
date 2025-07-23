#pragma once
#include "Defines.h"
#include "Tile.h"

class Map
{
private:
	Tile BG[DF_BG_SIZE_Y][DF_BG_SIZE_X];
	int PosX;
	int PosY;

public:
	Map();
};

