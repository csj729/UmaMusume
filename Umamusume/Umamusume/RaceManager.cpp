#include "RaceManager.h"
#include "ChallengeManager.h"

RaceManager::RaceManager(Map* map)
    : pMap(map)
{
    startClock = prevClock = clock();
}

void RaceManager::InitRace(Horse* horses[], Horse& player)
{
    rankIndex = 0;
    isRaceFinished = false;
    leader = nullptr;
    startClock = prevClock = clock();

    // --- 1. 이름 섞기 및 trainee 위치 결정 ---
    std::vector<std::string> shuffledNames(HorseName, HorseName + HORSETABLE_NUM);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffledNames.begin(), shuffledNames.end(), g);

    std::uniform_int_distribution<> dist(0, HORSE_NUM - 1);
    int playerIndex = dist(g);
    this->playerIndex = playerIndex;

    // --- 2. horses[] 초기화 ---
    int nameIdx = 0;
    for (int i = 0; i < HORSE_NUM; ++i)
    {
        if (i == playerIndex)
        {
            horses[i] = &player;
        }
        else
        {
            horses[i] = new Horse();
            horses[i]->InitByName(shuffledNames[nameIdx++]);
        }
    }
    
    //맵 임의로 설정
    pMap->SetMapSizeRandom();

    // --- 3. 위치 및 레인 설정 ---
    for (int i = 0; i < HORSE_NUM; i++)
    {
        int laneStartY = TRACK_START_Y + (LANE_HEIGHT + LANE_SPACING) * i;
        int horseTopY = laneStartY + (LANE_HEIGHT) / 2;

        horses[i]->SetPos(START_LINE_X, horseTopY + 2);
        horses[i]->SetLane(i + 1);
        // 플레이어 상태 초기화
        if (horses[i] == &player)
        {
            horses[i]->SetFinishTime(0.0f);
            horses[i]->ResetRaceState();
        }
    }
}
void RaceManager::InitRace(Horse* horses[], Horse& player, ChallengeManager& CM)
{
    rankIndex = 0;
    isRaceFinished = false;
    leader = nullptr;
    startClock = prevClock = clock();

    // --- 1. 이름 섞기 및 player 위치 결정 ---
    std::vector<std::string> shuffledNames(HorseName, HorseName + HORSETABLE_NUM);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffledNames.begin(), shuffledNames.end(), g);

    std::uniform_int_distribution<> dist(0, HORSE_NUM - 1);
    int playerIndex = dist(g);
    this->playerIndex = playerIndex;

    // --- 2. horses[] 초기화 ---
    int nameIdx = 0;
    for (int i = 0; i < HORSE_NUM; ++i)
    {
        if (i == playerIndex)
        {
            horses[i] = &player;
        }
        else
        {
            horses[i] = new Horse();
            horses[i]->InitByName(shuffledNames[nameIdx++]);
            horses[i]->SetBaseSpeed(horses[i]->GetBaseSpeed() + CM.GetStage() - 1);
            horses[i]->SetMaxStamina(horses[i]->GetMaxStamina() + CM.GetStage() - 1);
            horses[i]->SetStamina(horses[i]->GetMaxStamina());
            horses[i]->SetIntel(horses[i]->GetIntel() + CM.GetStage() * 10 - 10);
        }
    }

    //맵 임의로 설정
    pMap->SetMapSizeRandom();

    // --- 3. 위치 및 레인 설정 ---
    for (int i = 0; i < HORSE_NUM; i++)
    {
        int laneStartY = TRACK_START_Y + (LANE_HEIGHT + LANE_SPACING) * i;
        int horseTopY = laneStartY + (LANE_HEIGHT) / 2;

        horses[i]->SetPos(START_LINE_X, horseTopY + 2);
        horses[i]->SetLane(i + 1);
        // 플레이어 상태 초기화
        if (horses[i] == &player)
        {
            horses[i]->SetFinishTime(0.0f);
            horses[i]->ResetRaceState();
        }
    }
}

void RaceManager::RaceTick(Horse* horses[])
{
    clock_t now = clock();
    float deltaTime = static_cast<float>(now - prevClock) / CLOCKS_PER_SEC;
    prevClock = now;

    if (isRaceFinished)
        return;

    int leaderX = (leader != nullptr) ? leader->GetPos().X : 0;

    // 말들 Tick 처리
    for (int i = 0; i < HORSE_NUM; ++i)
        horses[i]->HorseTick(leaderX, deltaTime, *pMap);

    // 리더 재계산
    Horse* newLeader = nullptr;
    for (int i = 0; i < HORSE_NUM; ++i)
    {
        if (!horses[i]->IsFinish())
        {
            if (newLeader == nullptr || horses[i]->GetPos().X > newLeader->GetPos().X)
                newLeader = horses[i];
        }
    }

    if (newLeader != nullptr)
    {
        leader = newLeader;
        leader->CheckFinish(*pMap);

        // 리더 완주 시 순위 등록
        if (leader->IsFinish() && !leader->IsRanked())
        {
            if (rankIndex < HORSE_NUM)
            {
                float elapsedTime = static_cast<float>(now - startClock) / CLOCKS_PER_SEC;
                leader->SetFinishTime(elapsedTime);
                leader->SetRanked(true);
                rank[rankIndex++] = leader;
            }
        }

        // 스크롤 조정
        pMap->SetScrollX(leader->GetPos().X - DF_BG_SIZE_X * 2 / 3);
        if (pMap->GetScrollX() < 0)  pMap->SetScrollX(0);
        if (pMap->GetScrollX() > pMap->GetMapSize() - DF_BG_SIZE_X)
            pMap->SetScrollX(pMap->GetMapSize() - DF_BG_SIZE_X);

    }

    // 모든 말이 도착했는가?
    if (rankIndex == HORSE_NUM)
        isRaceFinished = true;
}

const Horse* RaceManager::GetRankedHorse(int index) const
{
    if (index >= 0 && index < rankIndex)
        return rank[index];
    return nullptr;
}

void RaceManager::Render(Tile(*BG)[DF_BG_SIZE_X], Horse* horses[])
{
    if (pMap) {
        pMap->MapRender(BG); // Map 렌더링 추가
    }

    for (int i = 0; i < HORSE_NUM; ++i)
    {
        horses[i]->HorseRender(BG, pMap->GetScrollX());
    }
}

void RaceManager::PrintUI(DoubleBuffering& DB, Horse* horses[])
{
    // UI 헤더 라인
    DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 1, "========================[ 경기 정보 ]========================");

    std::ostringstream infoStream;
    infoStream << "맵 거리: " << pMap->GetDistanceTypeString()
        << " (" << pMap->GetMapSize() << "칸)"
        << "   |   스크롤 위치: " << pMap->GetScrollX();

    DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 2, infoStream.str().c_str());

    const int baseWidth = 80; // 기본 출력 너비
    const int skillExtraWidth = 30; // 스킬명 최대 예상 너비

    for (int i = 0; i < HORSE_NUM; ++i)
    {
        std::ostringstream line;
        std::string msg;

        bool isPlayer = (i == playerIndex);

        if (!horses[i]->IsFinish())
        {
            line << std::setw(5) << horses[i]->GetLane() << "레인: ["
                << std::setw(12) << horses[i]->GetName() << "] "
                << "위치: " << std::setw(4) << horses[i]->GetPos().X
                << "   기력: " << std::fixed << std::setprecision(1) << std::setw(5) << horses[i]->GetStamina()
                << "   속도: " << std::setw(3) << horses[i]->GetRealSpeed();

            const Skill* activeSkill = horses[i]->GetActiveSkill();
            if (!isRaceFinished && activeSkill != nullptr && activeSkill->IsActive())
            {
                    line << "   *스킬: " << activeSkill->GetName();
            }

            msg = line.str();
            msg = (isPlayer ? "Player▶" : "        ") + msg;

            // 스킬명이 있으면 더 넓게 출력, 없으면 기본 너비
            int width = (activeSkill != nullptr) ? baseWidth + skillExtraWidth : baseWidth;
            if ((int)msg.length() < width)
                msg.resize(width, ' ');

            DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + i, msg.c_str());
        }
        else
        {
            std::ostringstream oss;
            oss << std::setw(5) << horses[i]->GetLane() << "레인: ["
                << std::setw(12) << horses[i]->GetName() << "] "
                << "결승선 도착!   "
                << std::fixed << std::setprecision(2)
                << horses[i]->GetFinishTime() << "초 완주";

            msg = oss.str();
            msg = (isPlayer ? "Player▶" : "        ") + msg;

            msg.resize(baseWidth, ' ');
            DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + i, msg.c_str());
        }
    }

    // 선두 출력
    if (leader != nullptr && !leader->IsFinish())
    {
        std::ostringstream leadStream;
        leadStream << "현재 선두: " << std::setw(2) << leader->GetLane() << "레인 "
            << std::setw(12) << leader->GetName() << " (위치 "
            << std::setw(4) << leader->GetPos().X << ")";
        std::string msg = leadStream.str();
        msg.resize(baseWidth, ' ');
        DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + HORSE_NUM, msg.c_str());
    }

    // 종료 메시지
    if (isRaceFinished)
    {
        std::string endLine = "== 경기 종료! 모든 말이 결승선을 통과했습니다! ==";
        DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 4 + HORSE_NUM, endLine.c_str());
     
        for (int i = 0; i < 3; ++i)
        {
            std::ostringstream rankLine;
            bool isPlayerRank = (rank[i] == horses[playerIndex]);

            rankLine << std::left
                << (isPlayerRank ? "Player▶ " : "        ")  // 8자리 고정
                << std::setw(3) << (i + 1) << "위 : "
                << std::setw(12) << rank[i]->GetName();

            DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 5 + HORSE_NUM + i, rankLine.str().c_str());
        }
    }
}

void RaceManager::RunRace(DoubleBuffering& DB, Tile(*_BG)[DF_BG_SIZE_X], Horse* RaceHorses[], Horse& trainee)
{
    // 콘솔 사이즈 조정 및 버퍼 초기화
    system("mode con:cols=200 lines=60");
    DB.ScreenInit();
    DB.ScreenFlipping();

    // 레이스 초기화
    InitRace(RaceHorses, trainee);

    // 레이스 루프
    while (!IsRaceFinished())
    {
        Sleep(100);
        DB.ClearScreen(_BG);
        RaceTick(RaceHorses);
        Render(_BG, RaceHorses);
        PrintUI(DB, RaceHorses);
        DB.PrintScreen(_BG);
    }

    // 종료 처리
    Sleep(2000);
    DB.ScreenRelease();
}
void RaceManager::RunRace(DoubleBuffering& DB, Tile(*_BG)[DF_BG_SIZE_X], Horse* RaceHorses[], Horse& trainee, ChallengeManager& CM)
{
    // 콘솔 사이즈 조정 및 버퍼 초기화
    system("mode con:cols=200 lines=60");
    DB.ScreenInit();
    DB.ScreenFlipping();

    // 레이스 초기화
    InitRace(RaceHorses, trainee, CM);

    // 레이스 루프
    while (!IsRaceFinished())
    {
        Sleep(100);
        DB.ClearScreen(_BG);
        RaceTick(RaceHorses);
        Render(_BG, RaceHorses);
        PrintUI(DB, RaceHorses);
        DB.PrintScreen(_BG);
    }

    // 종료 처리
    Sleep(2000);
    DB.ScreenRelease();
}

void RaceManager::SelectPlayerHorse(const std::vector<Horse>& PlayerList, Horse& player)
{
    system("cls");
    player.PrintPlayerList(PlayerList);

    int selNum;
    std::cout << "\n육성 완료된 말을 선택하세요 : ";
    std::cin >> selNum;

    while (true)
    {
        if (selNum < 1 || selNum > static_cast<int>(PlayerList.size()))
        {
            std::cout << "잘못된 입력입니다! 다시 선택해주세요 : ";
            std::cin >> selNum;
            continue;
        }

        player = PlayerList[selNum - 1];

        system("cls");
        std::cout << "\n" << player.GetName() << " 을 선택했습니다!!\n\n";

        std::cout << std::left << std::setw(10) << "이름" << ": " << player.GetName() << "\n";
        std::cout << std::left << std::setw(10) << "속도" << ": " << player.GetBaseSpeed() << "\n";
        std::cout << std::left << std::setw(10) << "스태미나" << ": " << player.GetMaxStamina() << "\n";
        std::cout << std::left << std::setw(10) << "지능" << ": " << player.GetIntel() << "\n";

        Sleep(2000);
        break;
    }
}