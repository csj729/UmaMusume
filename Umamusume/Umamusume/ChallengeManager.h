#pragma once
#include "Defines.h"
#include "Horse.h"
#include "RaceManager.h"

struct RankEntry
{
    std::string text;       // 플레이어가 직접 입력한 이름 (별명, 아이디 등)
    std::string horseName;  // 육성한 말의 이름
    int stage;              // 도달한 스테이지

    // 정렬 기준: 높은 스테이지 순 (동일한 스테이지는 같은 순위로 처리)
    bool operator<(const RankEntry& other) const 
    {
        return stage > other.stage;
    }
};

class ChallengeManager 
{
    int m_stage = 1;

public:
    void RunChallenge(std::vector<Horse>& PlayerList, Horse& player,
        RaceManager& RM, DoubleBuffering& DB,
        Tile BG[DF_BG_SIZE_Y][DF_BG_SIZE_X]);
    void PrintRanking();

    void SetStage(int _stage) { m_stage = _stage; }
    int GetStage() const { return m_stage; }


private:
    void LoadRanking(std::vector<RankEntry>& ranking);
    void SaveRanking(const std::vector<RankEntry>& ranking);
    void RegisterRanking(const Horse& player, RaceManager& RM);
};

