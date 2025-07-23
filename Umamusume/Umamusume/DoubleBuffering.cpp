#include "DoubleBuffering.h"

static int g_bScreenIndex = 0;
static HANDLE g_hScreen[2];

void DoubleBuffering::ScreenInit()
{
	//���۸�
	//ȭ�� ��¿� ���� ��ȭ���� �ΰ� �ؼ� �����ư��鼭 ���.
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
//��ǥ,����� ����
void DoubleBuffering::ScreenPrintUI(SHORT x, SHORT y, const char* _String)
{
	DWORD dw = 0;
	COORD Pos = { x,y };
	//���ۿ� Ŀ�� ��ġ ����
	SetConsoleCursorPosition(g_hScreen[g_bScreenIndex], Pos);
	//���ۿ� �۾� �Է�
	WriteFile(g_hScreen[g_bScreenIndex],
		_String, strlen(_String), &dw, NULL);
}

void DoubleBuffering::ScreenFlipping()
{
	//��ũ������ �����ư��鼭 ���
	SetConsoleActiveScreenBuffer(g_hScreen[g_bScreenIndex]);
	g_bScreenIndex = !g_bScreenIndex;
}

//�迭�� ���
void DoubleBuffering::ScreenPrint(Tile(*_BG)[DF_BG_SIZE_X])
{
	DWORD dw = 0;
	COORD Pos = { 0,0 };
	//1���� �迭�� ����鼭 ������� ������ +1�� ���� /null ��������
	char TempBG[(DF_BG_SIZE_X * DF_BG_SIZE_Y) + 1] = {};

	unsigned short TempColor = _BG[0][0].Color;

	int i = 0;//2���� �迭�� 1������ �������� ���� ����
	for (int y = 0; y < DF_BG_SIZE_Y; ++y)
	{
		for (int x = 0; x < DF_BG_SIZE_X; ++x)
		{
			TempBG[i++] = _BG[y][x].Text;

			//������ �ٸ����� ������
			if (TempColor != _BG[y][x].Color)
			{
				SetConsoleCursorPosition(g_hScreen[g_bScreenIndex], Pos);
				//���� ����
				SetConsoleTextAttribute(g_hScreen[g_bScreenIndex], TempColor);
				TempBG[i++] = '\0';
				//���ۿ� �׸� �׸�
				WriteFile(g_hScreen[g_bScreenIndex], TempBG, strlen(TempBG), &dw, NULL);
				//�ʱ�ȭ 0����
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
		//���� ����
		SetConsoleTextAttribute(g_hScreen[g_bScreenIndex], TempColor);
		SetConsoleCursorPosition(g_hScreen[g_bScreenIndex], Pos);
		WriteFile(g_hScreen[g_bScreenIndex], TempBG, strlen(TempBG), &dw, NULL);
	}
}


void DoubleBuffering::ScreenClear()
{
	DWORD dw = 0;
	//���� �ȿ� ' '����ִ� ���ڷ� ä���.
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