#include "Map.h"

Map::Map()
{
	

}

void Map::MapRender(Tile(*BG)[DF_BG_SIZE_X])
{
    const int laneHeight = LANE_HEIGHT; // 예: 10
    const int totalLanes = LANE_NUM;
    int startY = (DF_BG_SIZE_Y - (totalLanes * laneHeight + (totalLanes - 1))) / 2;
    
    for (int lane = 0; lane < totalLanes; ++lane)
    {
        int yOffset = startY + lane * (laneHeight + 1);
        for (int i = 0; i < laneHeight; ++i)
        {
            int y = yOffset + i;
            for (int x = 0; x < DF_BG_SIZE_X; ++x)
            {
                int worldX = m_scrollX + x;

                // 위아래 경계선
                if ( i == laneHeight - 1)
                    BG[y][x].Text = '-';
                else
                    BG[y][x].Text = ' ';

                // 시작선과 결승선 위치 (월드 좌표 기준)
                const int startLine = 3;

                if (i != 0 && i != laneHeight - 1 && worldX == startLine)
                    BG[y][x].Text = '|';
            }
        }
    }

    // 결승선 따로 그리기 (스크롤 반영 위치에만)
    int finishScreenX = m_finishLine - m_scrollX;
    if (finishScreenX >= 0 && finishScreenX < DF_BG_SIZE_X)
    {
        for (int lane = 0; lane < totalLanes; ++lane)
        {
            int yOffset = startY + lane * (laneHeight + 1);
            for (int i = 1; i < laneHeight - 1; ++i)
            {
                int y = yOffset + i;
                BG[y][finishScreenX].Text = '|';
            }
        }
    }
}

void Map::SetScrollX(int x) 
{
    if (x < 0) x = 0;
    int maxScroll = m_mapSize - DF_BG_SIZE_X;
    if (x > maxScroll) x = maxScroll;
    m_scrollX = x;
}

void Map::SetMapSizeRandom()
{
    int randValue = rand() % 3;
    if (randValue == 0)
    {
        m_mapSize = SHORT_DISTANCE;
    }
    else if (randValue == 1)
    {
        m_mapSize = MEDIUM_DISTANCE;
    }
    else
    {
        m_mapSize = LONG_DISTANCE;
    }

    m_finishLine = m_mapSize - 5;
}

std::string Map::GetDistanceTypeString() const
{
    switch (m_mapSize)
    {
    case SHORT_DISTANCE: return "단거리";
    case MEDIUM_DISTANCE: return "중거리";
    case LONG_DISTANCE: return "장거리";
    default: return "알 수 없음";
    }
}