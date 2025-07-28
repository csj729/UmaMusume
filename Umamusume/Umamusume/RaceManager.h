#pragma once
#include "Horse.h"
#include "Map.h"
#include "DoubleBuffering.h"

class RaceManager
{
private:

    Horse* leader = nullptr;
    Horse rank[HORSE_NUM];
    int rankIndex = 0;
    bool isRaceFinished = false;

    clock_t startClock;
    clock_t prevClock;

    Map* pMap = nullptr;

public:
    RaceManager(Map* map);
    void InitRace(Horse horses[]);
    void RaceTick(Horse horses[]);
    bool IsRaceFinished() const { return isRaceFinished; }
    const Horse* GetRankedHorse(int index) const;
    const Horse* GetLeader() const { return leader; }
    void Render(Tile(*BG)[DF_BG_SIZE_X], Horse horses[]);
    void PrintUI(DoubleBuffering& DB, Horse horses[]);
};