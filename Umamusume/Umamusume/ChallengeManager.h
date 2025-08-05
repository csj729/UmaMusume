#pragma once
#include "Defines.h"
#include "Horse.h"
#include "RaceManager.h"

struct RankEntry
{
    std::string text;       // �÷��̾ ���� �Է��� �̸� (����, ���̵� ��)
    std::string horseName;  // ������ ���� �̸�
    int stage;              // ������ ��������

    // ���� ����: ���� �������� �� (������ ���������� ���� ������ ó��)
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

