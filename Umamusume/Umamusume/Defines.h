#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <sstream>
#include <iomanip>

// 배경 크기
#define DF_BG_SIZE_X 200
#define DF_BG_SIZE_Y 40
#define HORSE_NUM 3
#define SKILL_NUM 3

#define LANE_HEIGHT 11       // 각 레인의 높이
#define LANE_SPACING 2       // 레인 사이 간격 (optional)
#define LANE_NUM 3
#define TRACK_START_Y 2      // 트랙의 시작 Y 좌표
#define START_LINE_X 3
#define SCROLL_SIZE 1000

#define HORSE1_NAME "말1"
#define HORSE2_NAME "말2"
#define HORSE3_NAME "말3"

const int worldWidth = SCROLL_SIZE;
const int FinishLine = worldWidth - 5;