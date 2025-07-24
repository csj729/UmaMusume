#pragma once
#include "Defines.h"
#include "Tile.h"

class Map
{
private:
	int m_scrollX = 0;

public:
	Map();
	void MapRender(Tile(*BG)[DF_BG_SIZE_X]);
	void SetScrollX(int NewscrollX);
	int GetScrollX() const { return m_scrollX; };
};