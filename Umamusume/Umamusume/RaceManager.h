#pragma once
#include "Horse.h"
#include "Map.h"
#include "DoubleBuffering.h"

class RaceManager
{
private:
    Map* pMap = nullptr;

    Horse* leader = nullptr;                   // ���� ��
    Horse* rank[HORSE_NUM];                     // ���� ���
    int rankIndex = 0;                         // ���� ���� �ε���
    int playerIndex = 0;                       // �÷��̾� �� �ε���

    bool isRaceFinished = false;

    clock_t startClock = 0;                    // ��� ���� �ð�
    clock_t prevClock = 0;                     // ���� ƽ ���� �ð�

public:
    explicit RaceManager(Map* map);

    // �ʱ�ȭ
    void InitRace(Horse* horses[], Horse& trainee);

    // �����Ӻ� ó��
    void RaceTick(Horse* horses[]);

    // ������
    void Render(Tile(*BG)[DF_BG_SIZE_X], Horse* horses[]);
    void PrintUI(DoubleBuffering& DB, Horse* horses[]);

    // ��ü ���̽� ����
    void RunRace(DoubleBuffering& DB, Tile(*BG)[DF_BG_SIZE_X], Horse* horses[], Horse& trainee);

    // ���� ��ȸ
    bool IsRaceFinished() const { return isRaceFinished; }
    const Horse* GetRankedHorse(int index) const;
    const Horse* GetLeader() const { return leader; }
};