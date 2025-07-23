#include "DoubleBuffering.h"

static int g_bScreenIndex = 0;
static HANDLE g_hScreen[2];

void DoubleBuffering::ScreenInit()
{
	//버퍼링
	//화면 출력용 버퍼 도화지를 두개 해서 번갈아가면서 사용.
	g_hScreen[0] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	g_hScreen[1] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}
//좌표,출력할 내용
void DoubleBuffering::ScreenPrintUI(SHORT x, SHORT y, const char* _String)
{
	DWORD dw = 0;
	COORD Pos = { x,y };
	//버퍼에 커서 위치 설정
	SetConsoleCursorPosition(g_hScreen[g_bScreenIndex], Pos);
	//버퍼에 글씨 입력
	WriteFile(g_hScreen[g_bScreenIndex],
		_String, strlen(_String), &dw, NULL);
}

void DoubleBuffering::ScreenFlipping()
{
	//스크린버퍼 번갈아가면서 출력
	SetConsoleActiveScreenBuffer(g_hScreen[g_bScreenIndex]);
	g_bScreenIndex = !g_bScreenIndex;
}

//배열로 출력
void DoubleBuffering::ScreenPrint(Tile(*_BG)[DF_BG_SIZE_X])
{
	DWORD dw = 0;
	COORD Pos = { 0,0 };
	//1차원 배열로 만들면서 사이즈는 이차원 +1은 끝에 /null 넣으려고
	char TempBG[(DF_BG_SIZE_X * DF_BG_SIZE_Y) + 1] = {};

	unsigned short TempColor = _BG[0][0].Color;

	int i = 0;//2차원 배열을 1차원에 넣으려고 만든 숫자
	for (int y = 0; y < DF_BG_SIZE_Y; ++y)
	{
		for (int x = 0; x < DF_BG_SIZE_X; ++x)
		{
			TempBG[i++] = _BG[y][x].Text;

			//색상이 다른것이 있을때
			if (TempColor != _BG[y][x].Color)
			{
				SetConsoleCursorPosition(g_hScreen[g_bScreenIndex], Pos);
				//색상 설정
				SetConsoleTextAttribute(g_hScreen[g_bScreenIndex], TempColor);
				TempBG[i++] = '\0';
				//버퍼에 그림 그림
				WriteFile(g_hScreen[g_bScreenIndex], TempBG, strlen(TempBG), &dw, NULL);
				//초기화 0으로
				memset(TempBG, 0, sizeof(TempBG));
				i = 0;
				TempBG[i++] = _BG[y][x].Text;
				TempColor = _BG[y][x].Color;
				Pos.X = x;
				Pos.Y = y;
			}
		}
	}
	if (i != 0)
	{
		TempBG[i++] = '\0';
		//색상 설정
		SetConsoleTextAttribute(g_hScreen[g_bScreenIndex], TempColor);
		SetConsoleCursorPosition(g_hScreen[g_bScreenIndex], Pos);
		WriteFile(g_hScreen[g_bScreenIndex], TempBG, strlen(TempBG), &dw, NULL);
	}
}


void DoubleBuffering::ScreenClear()
{
	DWORD dw = 0;
	//버퍼 안에 ' '비어있는 문자로 채운다.
	FillConsoleOutputCharacter(
		g_hScreen[g_bScreenIndex],
		' ', DF_BG_SIZE_X * DF_BG_SIZE_Y,
		COORD(),
		&dw);
}
void DoubleBuffering::ScreenRelease()
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}