#include "Defines.h"
#include "DoubleBuffering.h"
#include "Tile.h"
#include "Horse.h"
#include "Skill.h"
#include "Map.h"
#include "RaceManager.h"

static Tile BG[DF_BG_SIZE_Y][DF_BG_SIZE_X];
static Map map;
static DoubleBuffering DB;
static Horse horses[HORSE_NUM];
static float deltaTime = 0.0f;
static RaceManager RM(&map);

static char LOGO[21][80] =
{

{" _   __    _____     ___    _____    ___   ___  ___  _____"},
{"| | / /   |  __ \   / _ \  |  __ \  / _ \  |  \/  | |  ___|"},
{"| |/ /    | |  \/  / /_\ \ | |  \/ / /_\ \ | .  . | | |__"},
{"|    \    | | __   |  _  | | | __  |  _  | | |\/| | |  __|"},
{"| |\  \   | |_\ \  | | | | | |_\ \ | | | | | |  | | | |___"},
{"\_| \_/    \____/  \_| |_/  \____/ \_| |_/ \_|  |_/ \____/"},
};

void PrintScreen()
{
    //끝자리에 '\0'널문자 넣어서 출력가능하도록
    BG[DF_BG_SIZE_Y - 1][DF_BG_SIZE_X - 1].Text = '\0';
    DB.ScreenClear();  //버퍼 초기화
    DB.ScreenPrint(BG); //내꺼 출력
    DB.ScreenFlipping(); //버퍼 교체
}

void PrintMainMenu_Console()
{
    std::cout << "========================[ 경마 게임 ]========================\n";
    std::cout << "1. 트레이닝 (육성)\n";
    std::cout << "2. 경주 시작\n";
    std::cout << "3. 저장\n";
    std::cout << "4. 불러오기\n";
    std::cout << "5. 종료\n";
    std::cout << "=============================================================\n";
    std::cout << ">> 선택: ";
}

//시작
void BeginPlay()
{
	//윈도우 창 사이즈 설정
	system("mode con:cols=200 lines=60");
	DB.ScreenInit();
	DB.ScreenFlipping();
	Sleep(500); //3초간 프로그램을 멈춤
}

//프레임마다 반복 실행
void Tick()
{ 
    RM.RaceTick(horses);
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
    RM.Render(BG, horses);
}

// 여기 
void UIPrint()
{
    RM.PrintUI(DB, horses);
}

void SelectAction()
{
    system("cls");
    PrintMainMenu_Console(); // 콘솔로 메뉴 출력

    int selNum = 0;
    std::cin >> selNum; // 정수 입력

    switch (selNum)
    {
    case TRAINING:
        std::cout << "\n[ 트레이닝 시스템은 아직 구현되지 않았습니다. ]\n";
        system("pause");
        break;

    case RACE:
        system("mode con:cols=200 lines=60");
        DB.ScreenInit();
        DB.ScreenFlipping();

        RM.InitRace(horses);

        while (!RM.IsRaceFinished())
        {
            Sleep(100);
            ClearScreen();
            RM.RaceTick(horses);
            RM.Render(BG, horses);
            RM.PrintUI(DB, horses);
            PrintScreen(); // DoubleBuffer로 화면 출력
        }

        Sleep(2000);
        DB.ScreenRelease(); 
        break;

    case SAVE:
        std::cout << "\n[ 저장 시스템은 아직 구현되지 않았습니다. ]\n";
        system("pause");
        break;

    case LOAD:
        std::cout << "\n[ 불러오기 시스템은 아직 구현되지 않았습니다. ]\n";
        system("pause");
        break;

    case FINISH:
        std::cout << "게임을 종료합니다.\n";
        exit(0);

    default:
        std::cout << "1~5번 중에서 선택해주세요.\n";
        system("pause");
        break;
    }
}


int main()
{
    srand(unsigned(time(NULL)));

    while (true)
    {
        ClearScreen(); // BG 배열 초기화
        SelectAction(); // 콘솔 UI + 레이스 실행
    }

    return 0;
}