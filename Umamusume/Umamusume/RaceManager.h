#pragma once
#include "Horse.h"
#include "Map.h"
#include "DoubleBuffering.h"
#include "SkillDatabase.h"

class ChallengeManager;

class RaceManager
{
private:
    Map* pMap = nullptr;

    Horse* leader = nullptr;                   // 선두 말
    Horse* rank[HORSE_NUM];                     // 순위 기록
    int rankIndex = 0;                         // 순위 저장 인덱스
    int playerIndex = 0;                       // 플레이어 말 인덱스

    bool isRaceFinished = false;

    clock_t startClock = 0;                    // 경기 시작 시각
    clock_t prevClock = 0;                     // 이전 틱 기준 시간

public:
    explicit RaceManager(Map* map);

    // 플레이어 말 선택
    void SelectPlayerHorse(const std::vector<Horse>& PlayerList, Horse& player);

    // 초기화
    void InitRace(Horse* horses[], Horse& trainee);
    void InitRace(Horse* horses[], Horse& trainee, ChallengeManager& CM);

    // 프레임별 처리
    void RaceTick(Horse* horses[]);

    // 렌더링
    void Render(Tile(*BG)[DF_BG_SIZE_X], Horse* horses[]);
    void PrintUI(DoubleBuffering& DB, Horse* horses[]);

    // 전체 레이스 실행
    void RunRace(DoubleBuffering& DB, Tile(*BG)[DF_BG_SIZE_X], Horse* horses[], Horse& trainee);
    void RunRace(DoubleBuffering& DB, Tile(*BG)[DF_BG_SIZE_X], Horse* horses[], Horse& trainee, ChallengeManager& CM);

    // 상태 조회
    bool IsRaceFinished() const { return isRaceFinished; }
    const Horse* GetRankedHorse(int index) const;
    const Horse* GetLeader() const { return leader; }
};