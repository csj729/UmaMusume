#pragma once
#include <Windows.h>
#include "Defines.h"
#include "Tile.h"

class DoubleBuffering
{


public:
	void ScreenInit();
	void ScreenFlipping();
	void ScreenPrintUI(
		SHORT x, SHORT y,
		const char* _String);

	void ScreenRelease();
	void ScreenClear();
	void ScreenPrint(Tile(*_BG)[DF_BG_SIZE_X]);
};

