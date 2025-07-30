#include "RaceManager.h"

RaceManager::RaceManager(Map* map)
    : pMap(map)
{
    startClock = prevClock = clock();
}

void RaceManager::InitRace(Horse horses[])
{
    rankIndex = 0;
    isRaceFinished = false;
    leader = nullptr;
    startClock = prevClock = clock();

    // 중복된 말 없이 뽑아서 horses 배열에 넣기
    std::vector<std::string> shuffledNames(HorseName, HorseName + HORSETABLE_NUM);

    // 이름 섞기
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffledNames.begin(), shuffledNames.end(), g);

    for (int i = 0; i < HORSE_NUM; ++i)
    {
        horses[i].SetName(shuffledNames[i]);
    }

    // 뽑힌 말 이름을 비교해 초기화
    for (int i = 0; i < HORSE_NUM; i++)
    {
        int laneStartY = TRACK_START_Y + (LANE_HEIGHT + LANE_SPACING) * i;
        int horseTopY = laneStartY + (LANE_HEIGHT) / 2;

        horses[i].InitHorse();
        horses[i].SetPos(START_LINE_X, horseTopY + 2); // 중앙을 기준으로 Y좌표 설정
        horses[i].SetLane(i + 1);
    }

}

void RaceManager::RaceTick(Horse horses[])
{
    clock_t now = clock();
    float deltaTime = static_cast<float>(now - prevClock) / CLOCKS_PER_SEC;
    prevClock = now;

    if (isRaceFinished)
        return;

    int leaderX = (leader != nullptr) ? leader->GetPos().X : 0;

    // 말들 Tick 처리
    for (int i = 0; i < HORSE_NUM; ++i)
        horses[i].HorseTick(leaderX, deltaTime);

    // 리더 재계산
    Horse* newLeader = nullptr;
    for (int i = 0; i < HORSE_NUM; ++i)
    {
        if (!horses[i].IsFinish())
        {
            if (newLeader == nullptr || horses[i].GetPos().X > newLeader->GetPos().X)
                newLeader = &horses[i];
        }
    }

    if (newLeader != nullptr)
    {
        leader = newLeader;
        leader->CheckFinish();

        // 리더 완주 시 순위 등록
        if (leader->IsFinish() && !leader->IsRanked())
        {
            if (rankIndex < HORSE_NUM)
            {
                float elapsedTime = static_cast<float>(now - startClock) / CLOCKS_PER_SEC;
                leader->SetFinishTime(elapsedTime);
                leader->SetRanked(true);
                rank[rankIndex++] = *leader;
            }
        }

        // 스크롤 조정
        pMap->SetScrollX(leader->GetPos().X - DF_BG_SIZE_X * 2 / 3);
        if (pMap->GetScrollX() < 0)  pMap->SetScrollX(0);
        if (pMap->GetScrollX() > worldWidth - DF_BG_SIZE_X)
            pMap->SetScrollX(worldWidth - DF_BG_SIZE_X);

    }

    // 모든 말이 도착했는가?
    if (rankIndex == HORSE_NUM)
        isRaceFinished = true;
}

const Horse* RaceManager::GetRankedHorse(int index) const
{

    if (index >= 0 && index < rankIndex)
        return &rank[index];
    return nullptr;
}

void RaceManager::Render(Tile(*BG)[DF_BG_SIZE_X], Horse horses[])
{
    if (pMap) {
        pMap->MapRender(BG); // Map 렌더링 추가
    }

    for (int i = 0; i < HORSE_NUM; ++i)
    {
        horses[i].HorseRender(BG, pMap->GetScrollX());
    }
}

void RaceManager::PrintUI(DoubleBuffering& DB, Horse horses[])
{
    // UI 헤더 라인
    DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 1, "========================[ 경기 정보 ]========================");

    // 스크롤 위치 출력
    std::ostringstream scrollStream;
    scrollStream << "스크롤 위치: " << pMap->GetScrollX();
    DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 2, scrollStream.str().c_str());

    for (int i = 0; i < HORSE_NUM; ++i)
    {
        std::ostringstream line;
        std::string msg;

        if (!horses[i].IsFinish())
        {
            line << std::setw(5) << horses[i].GetLane() << "레인: ["
                << std::setw(12) << horses[i].GetName() << "] "
                << "위치: " << std::setw(4) << horses[i].GetPos().X
                << "   기력: " << std::fixed << std::setprecision(1) << std::setw(5) << horses[i].GetHp()
                << "   속도: " << std::setw(3) << horses[i].GetRealSpeed();

            const Skill* activeSkill = horses[i].GetActiveSkill();
            if (activeSkill != nullptr)
            {
                line << "   ⚡스킬: " << activeSkill->GetName();
            }

            msg = line.str();
            msg.resize(100, ' ');  // 너비 고정
            DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + i, msg.c_str());
        }
        else
        {
            std::ostringstream oss;
            oss << std::setw(5) << horses[i].GetLane() << "레인: ["
                << std::setw(10) << horses[i].GetName() << "] "
                << "결승선 도착!   "
                << std::fixed << std::setprecision(2)
                << horses[i].GetFinishTime() << "초 완주";

            msg = oss.str();
            msg.resize(60, ' ');
            DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + i, msg.c_str());
        }
    }

    // 선두 말 출력
    if (leader != nullptr && !leader->IsFinish())
    {
        std::ostringstream leadStream;
        leadStream << "현재 선두: " << leader->GetLane() << "레인 "
            << leader->GetName() << " (위치 "
            << leader->GetPos().X << ")";
        std::string msg = leadStream.str();
        msg.resize(60, ' ');
        DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + HORSE_NUM, msg.c_str());
    }

    // 경기 종료 여부
    if (isRaceFinished)
    {
        std::ostringstream rankStream;
        std::string endLine = "🎉 경기 종료! 모든 말이 결승선을 통과했습니다! 🎉";
        rankStream << "1위 : " << rank[0].GetName()
            << "  2위 : " << rank[1].GetName()
            << "   3위 : " << rank[2].GetName();
        DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 4 + HORSE_NUM, endLine.c_str());
        DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 5 + HORSE_NUM, rankStream.str().c_str());
    }
}