#include "Defines.h"
#include "DoubleBuffering.h"
#include "Tile.h"
#include "Horse.h"
#include "Skill.h"
#include "Map.h"

static Map map;
static Tile BG[DF_BG_SIZE_Y][DF_BG_SIZE_X];
static DoubleBuffering DB;
static Horse horses[HORSE_NUM];

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
	system("mode con:cols=200 lines=50");
	DB.ScreenInit();

	for (int i = 0; i < 21; ++i)
		DB.ScreenPrintUI(0, i, LOGO[i]);

	DB.ScreenFlipping();
	Sleep(500); //3초간 프로그램을 멈춤
}
//프레임마다 반복 실행
void Tick()
{
	for (int i = 0; i < HORSE_NUM; i++)
		horses[i].HorseTick();

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
	for (int i = 0; i < HORSE_NUM; i++)
		horses[i].HorseRender();
}

// 여기 
void UIPrint()
{
	DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 1, "===================================================");
	char UI[DF_BG_SIZE_X] = {};
	//sprintf_s(UI, "FPS : %d     EXP : %d / %d     HP : %d     LEVEL : %d", GetFPS(), *GetEXP(), *GetMaxEXP(), *GetHP(), *GetLevel());
	DB.ScreenPrintUI(0, DF_BG_SIZE_Y + 2, UI);
	//GetFPS();
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
		Sleep(10);
		ClearScreen(); //배열 초기화

		Tick(); //로직 연산
		Render();//오브젝트 출력

		PrintScreen();//화면 출력
		UIPrint();
	}


	DB.ScreenRelease();
}