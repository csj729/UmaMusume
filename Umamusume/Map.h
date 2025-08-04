#pragma once
#include "Defines.h"
#include "Tile.h"

class Map
{
private:
	int m_scrollX = 0;
	int m_mapSize = 0;
	int m_finishLine = 0;

public:
	Map();
	void MapRender(Tile(*BG)[DF_BG_SIZE_X]);
	void SetScrollX(int NewscrollX);
	int GetScrollX() const { return m_scrollX; };
	int GetMapSize() const { return m_mapSize; };
	int GetFinishLine() const { return m_finishLine; }
	std::string GetDistanceTypeString() const;
	void SetMapSizeRandom();
};