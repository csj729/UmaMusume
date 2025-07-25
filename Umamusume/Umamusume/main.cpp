#include "Defines.h"
#include "DoubleBuffering.h"
#include "Tile.h"
#include "Horse.h"
#include "Skill.h"
#include "Map.h"

static Tile BG[DF_BG_SIZE_Y][DF_BG_SIZE_X];
static Map map;
static DoubleBuffering DB;
static Horse horses[HORSE_NUM];
static int scrollX = 0; 
static bool g_isRaceFinished = false;
static Horse* last;
static Horse* leader;
static Horse rank[HORSE_NUM];
static int rankIndex = 0;
static clock_t g_startClock = clock();
static clock_t g_prevClock = clock();
static float deltaTime = 0.0f;

static char LOGO[21][80] =
{

{" _   __    _____     ___    _____    ___   ___  ___  _____"},
{"| | / /   |  __ \   / _ \  |  __ \  / _ \  |  \/  | |  ___|"},
{"| |/ /    | |  \/  / /_\ \ | |  \/ / /_\ \ | .  . | | |__"},
{"|    \    | | __   |  _  | | | __  |  _  | | |\/| | |  __|"},
{"| |\  \   | |_\ \  | | | | | |_\ \ | | | | | |  | | | |___"},
{"\_| \_/    \____/  \_| |_/  \____/ \_| |_/ \_|  |_/ \____/"},
};
//시작
void BeginPlay()
{
	//윈도우 창 사이즈 설정
	system("mode con:cols=200 lines=60");
	DB.ScreenInit();

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
        int horseTopY = laneStartY + (LANE_HEIGHT - 4) / 2;

        horses[i].InitHorse();
        horses[i].SetPos(START_LINE_X, horseTopY + 2); // 중앙을 기준으로 Y좌표 설정
        horses[i].SetLane(i + 1);
    }
	

	for (int i = 0; i < 21; ++i)
		DB.ScreenPrintUI(0, i, LOGO[i]);

	DB.ScreenFlipping();
	Sleep(500); //3초간 프로그램을 멈춤
}
//프레임마다 반복 실행
void Tick()
{ 
    clock_t now = clock();
    deltaTime = static_cast<float>(now - g_prevClock) / CLOCKS_PER_SEC;
    g_prevClock = now;

	if (g_isRaceFinished)
		return;

	int leaderX = (leader != nullptr) ? leader->GetPos().X : 0;

	for (int i = 0; i < HORSE_NUM; i++)
		horses[i].HorseTick(leaderX, deltaTime);

	if (rankIndex == HORSE_NUM)
		g_isRaceFinished = true;
	
	// 선두 말 위치
	Horse* newLeader = nullptr;

	for (int i = 0; i < HORSE_NUM; ++i)
	{
		if (!horses[i].IsFinish())
		{
			if (newLeader == nullptr || horses[i].GetPos().X > newLeader->GetPos().X)
				newLeader = &horses[i];
		}

	}
	// 모든 말이 도착하지 않은 경우에만 처리
	if (newLeader != nullptr) 
	{
		leader = newLeader;
		leader->CheckFinish();

		// 선두 말이 결승선 통과 했는지 체크하고, 해당 말의 랭크를 등록
		if (leader->IsFinish() && !leader->IsRanked())
		{
			if (rankIndex < HORSE_NUM)
			{
				clock_t now = clock();
				double elapsedTime = static_cast<double>(now - g_startClock) / CLOCKS_PER_SEC;
				leader->SetFinishTime(static_cast<float>(elapsedTime)); // 소수점 단위 시간 저장

				leader->SetRanked(true);
				rank[rankIndex++] = *leader;
			}
		}
		scrollX = leader->GetPos().X - DF_BG_SIZE_X * 2 / 3;
		if (scrollX < 0) scrollX = 0;

		// 전체 트랙 길이보다 넘지 않게
		if (scrollX > worldWidth - DF_BG_SIZE_X)
			scrollX = worldWidth - DF_BG_SIZE_X;

		map.SetScrollX(scrollX);
	}

}

//내거 2차원 배열
void ClearScreen()
{
    unsigned short BaseColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
    for (int y = 0; y < DF_BG_SIZE_Y; ++y)
    {
        for (int x = 0; x < DF_BG_SIZE_X; ++x)
        {
            BG[y][x].Text = ' ';
            BG[y][x].Color = BaseColor;
        }
    }
}

// 오브젝트 화면에 그리기
void Render()
{
    map.MapRender(BG);
    for (int i = 0; i < HORSE_NUM; i++)
        horses[i].HorseRender(BG, scrollX);
}

// 여기 
void UIPrint()
{
    // UI 헤더 라인
    DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 1, "========================[ 경기 정보 ]========================");

    // 스크롤 위치 출력
    std::ostringstream scrollStream;
    scrollStream << "스크롤 위치: " << scrollX;
    DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 2, scrollStream.str().c_str());

    // 말들의 상태 출력
    for (int i = 0; i < HORSE_NUM; ++i)
    {
        std::ostringstream line;
        std::string msg = "";
        if (!horses[i].IsFinish())
        {
            line << horses[i].GetLane() << "Lane: [" << horses[i].GetName() << "] 위치: "
                << horses[i].GetPos().X
                << "   기력: " << std::fixed << std::setprecision(1) << horses[i].GetHp()
                << "   속도: " << horses[i].GetRealSpeed();

            // 🟡 스킬 발동 중이면 표시
            const Skill* activeSkill = horses[i].GetActiveSkill();
            if (activeSkill != nullptr)
            {
                line << "   ⚡스킬 발동중: " << activeSkill->GetName();
            }

            msg = line.str();
            msg.resize(100, ' ');
            DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + i, msg.c_str());
        }
        else
        {
            // 결승선 도달한 말 - 나머지 정보 출력 X
            std::ostringstream oss;
            oss << horses[i].GetLane() << "Lane: [" << horses[i].GetName() << "] 결승선 도착!   "
                << std::fixed << std::setprecision(2) << horses[i].GetFinishTime() << "초 완주";

            msg = oss.str();
            msg.resize(60, ' ');  // 60글자까지 공백으로 채움

            DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + i, msg.c_str());
            continue;
        }
    }

    // 선두 말 출력 (완주 안 한 말 중에서)
    std::ostringstream leadStream;
    leadStream << "현재 선두: " << leader->GetLane() << "레인 " << leader->GetName()
        << " (위치 " << leader->GetPos().X << ")";
    std::string msg = leadStream.str();
    msg.resize(60, ' ');
    DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + HORSE_NUM, msg.c_str());

    // 경기 종료 여부
    if (g_isRaceFinished)
    {
        std::ostringstream rankStream;
        std::string endLine = "🎉 경기 종료! 모든 말이 결승선을 통과했습니다! 🎉";
        rankStream << "1위 : " << rank[0].GetName() << "  2위 : " << rank[1].GetName() << "   3위 : " << rank[2].GetName();
        DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 4 + HORSE_NUM, endLine.c_str());
        DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 5 + HORSE_NUM, rankStream.str().c_str());
    }
}

void PrintScreen()
{
    //끝자리에 '\0'널문자 넣어서 출력가능하도록
    BG[DF_BG_SIZE_Y - 1][DF_BG_SIZE_X - 1].Text = '\0';
    DB.ScreenClear();  //버퍼 초기화
    DB.ScreenPrint(BG); //내꺼 출력
    DB.ScreenFlipping(); //버퍼 교체
}

int main()
{
    srand(unsigned(time(NULL)));
    BeginPlay();

    while (true)
    {
        Sleep(200);
        ClearScreen(); //배열 초기화

        Tick(); //로직 연산
        Render(); //오브젝트 출력

        PrintScreen(); //화면 출력
        UIPrint();
    }

    DB.ScreenRelease();
}