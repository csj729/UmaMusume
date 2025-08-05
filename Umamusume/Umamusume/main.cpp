#include "Defines.h"
#include "DoubleBuffering.h"
#include "Tile.h"
#include "Horse.h"
#include "Skill.h"
#include "Map.h"
#include "RaceManager.h"
#include "TrainingManager.h"
#include "FileSystem.h"
#include "ChallengeManager.h"

static Tile BG[DF_BG_SIZE_Y][DF_BG_SIZE_X];
static Map map;
static DoubleBuffering DB;
static Horse* horses[HORSE_NUM];
static float deltaTime = 0.0f;
static RaceManager RM(&map);
static TrainingManager TM;
static Horse player;
static std::vector<Horse> PlayerList;
static FileSystem FS;
static ChallengeManager CM;

static char LOGO[21][80] =
{

{" _   __    _____     ___    _____    ___   ___  ___  _____"},
{"| | / /   |  __ \   / _ \  |  __ \  / _ \  |  \/  | |  ___|"},
{"| |/ /    | |  \/  / /_\ \ | |  \/ / /_\ \ | .  . | | |__"},
{"|    \    | | __   |  _  | | | __  |  _  | | |\/| | |  __|"},
{"| |\  \   | |_\ \  | | | | | |_\ \ | | | | | |  | | | |___"},
{"\_| \_/    \____/  \_| |_/  \____/ \_| |_/ \_|  |_/ \____/"},
};

void PrintMainMenu_Console()
{
    std::cout << "========================[ 경마 게임 ]========================\n";
    std::cout << "1. 트레이닝 (육성)\n";
    std::cout << "2. 연습 레이스 시작\n";
    std::cout << "3. 챌린지 레이스 시작\n";
    std::cout << "4. 저장\n";
    std::cout << "5. 불러오기\n";
    std::cout << "6. 육성 우마무스메 리스트\n";
    std::cout << "7. 종료\n";
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
        // 말 육성
        TM.TrainingLoop(player, PlayerList, RM, DB, BG, horses);
        break;

    case RACE:
        // 연습 레이스, 시뮬레이션 용
        RM.SelectPlayerHorse(PlayerList, player);
        RM.RunRace(DB, BG, horses, player);
        break;

    case CHALLENGE:
        int selNum;
        std::cout << "챌린지 레이스에 오신 것을 환영합니다!!\n\n";
        std::cout << "1. 챌린지 레이스 도전    2. 챌린지 레이스 순위 보기\n";
        std::cout << "입력 : ";
        std::cin >> selNum;
        system("cls");
        if (selNum == 1)
            CM.RunChallenge(PlayerList, player, RM, DB, BG);
        else if (selNum == 2)   
            CM.PrintRanking();
        else
        {
            std::cout << "잘못된 입력입니다.\n";
            Sleep(1000);
        }
        break;
    case SAVE:
        if (FS.SavePlayerList("PlayerList.dat", PlayerList))
            std::cout << "\n[ 저장 성공 ]\n";
        else
            std::cout << "\n[ 저장 실패 ]\n";
        system("pause");
        break;

    case LOAD:
        if (FS.LoadPlayerList("PlayerList.dat", PlayerList))
            std::cout << "\n[ 불러오기 성공 ]\n";
        else
            std::cout << "\n[ 불러오기 실패 ]\n";
        system("pause");
        break;

    case UMALIST:
        system("cls");
        player.PrintPlayerList(PlayerList);
        system("pause");
        break;

    case FINISH:
        std::cout << "게임을 종료합니다.\n";
        exit(0);

    default:
        std::cout << "1~7번 중에서 선택해주세요.\n";
        system("pause");
        break;
    }
}


int main()
{
    srand(unsigned(time(NULL)));

    while (true)
    {
        DB.ClearScreen(BG); // BG 배열 초기화
        SelectAction(); // 콘솔 UI + 레이스 실행
    }

    return 0;
}