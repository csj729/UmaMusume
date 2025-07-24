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
	for (int i = 0; i < HORSE_NUM; i++)
	{
		int laneStartY = TRACK_START_Y + (LANE_HEIGHT + LANE_SPACING - 1) * i;
		int laneCenterY = laneStartY + LANE_HEIGHT / 2;

		horses[i].SelectName(HorseName);
		horses[i].InitHorse();
		horses[i].SetPos(START_LINE_X, laneCenterY); // 중앙에 위치시킴
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
	if (g_isRaceFinished)
		return;

	for (int i = 0; i < HORSE_NUM; i++)
		horses[i].HorseTick();

	// 꼴찌 말의 위치
	last = &horses[0];
	for (int i = 1; i < HORSE_NUM; ++i)
		if (horses[i].GetPos().X < last->GetPos().X)
			last = &horses[i];

	// 꼴찌 말 결승선 도착 시 레이스 종료
	if (last->GetPos().X >= FinishLine)
	{
		g_isRaceFinished = true;
	}
	
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
		if (!horses[i].IsFinish())
		{
			line << horses[i].GetLane() << "Lane: [" << horses[i].GetName() << "] 위치: "
				<< horses[i].GetPos().X
				<< "   기력: " << std::fixed << std::setprecision(1) << horses[i].GetHp()
				<< "   속도: " << horses[i].GetRealSpeed();
		}
		else
		{
			line << horses[i].GetLane() << "Lane: [" << horses[i].GetName() << "] 결승선 도착!";
		}
		DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + i, line.str().c_str());
	}

	// 선두 말 출력 (완주 안 한 말 중에서)
	std::ostringstream leadStream;
	leadStream << "현재 선두: " << leader->GetLane() << "레인 " << leader->GetName()
		<< " (위치 " << leader->GetPos().X << ")";
	DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 3 + HORSE_NUM, leadStream.str().c_str());

	// 경기 종료 여부
	if (g_isRaceFinished)
	{
		std::string endLine = "🎉 경기 종료! 모든 말이 결승선을 통과했습니다! 🎉";
		DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 4 + HORSE_NUM, endLine.c_str());
	}
}
void PrintScreen()
{
	//끝자리에 '\0'널문자 넣어서 출력가능하도록
	BG[DF_BG_SIZE_Y - 1][DF_BG_SIZE_X - 1].Text = '\0';
	DB.ScreenClear();//버퍼 초기화
	DB.ScreenPrint(BG);//내꺼 출력
	DB.ScreenFlipping();//버퍼 교체
}

int main()
{
	srand(unsigned(time(NULL)));
	BeginPlay();

	while (true)
	{
		Sleep(100);
		ClearScreen(); //배열 초기화

		Tick(); //로직 연산
		Render();//오브젝트 출력

		PrintScreen();//화면 출력
		UIPrint();
	}


	DB.ScreenRelease();
}