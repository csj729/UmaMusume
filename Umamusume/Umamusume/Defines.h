#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <cmath>
#include <map>
#include <fstream>

// 배경 크기
#define DF_BG_SIZE_X 200
#define DF_BG_SIZE_Y 40

// 트랙 폭, 길이 관련
#define LANE_HEIGHT 9       // 각 레인의 높이
#define LANE_SPACING 1      // 레인 사이 간격 (optional)
#define LANE_NUM 4
#define TRACK_START_Y 0     // 트랙의 시작 Y 좌표
#define START_LINE_X 3
#define SHORT_DISTANCE 2000	// 트랙 길이
#define MEDIUM_DISTANCE 3500
#define LONG_DISTANCE 5000

// 플레이 오브젝트 관련
#define HORSE_NUM 4
#define HORSETABLE_NUM 4
#define SKILL_NUM 3

enum Action
{
	TRAINING = 1,
	RACE,
	CHALLENGE,
	SAVE,
	LOAD,
	UMALIST,
	FINISH
};