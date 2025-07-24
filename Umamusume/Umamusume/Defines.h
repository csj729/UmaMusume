#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <sstream>
#include <iomanip>

// ��� ũ��
#define DF_BG_SIZE_X 200
#define DF_BG_SIZE_Y 40
#define HORSE_NUM 3
#define SKILL_NUM 3

#define LANE_HEIGHT 11       // �� ������ ����
#define LANE_SPACING 2       // ���� ���� ���� (optional)
#define LANE_NUM 3
#define TRACK_START_Y 2      // Ʈ���� ���� Y ��ǥ
#define START_LINE_X 3
#define SCROLL_SIZE 1000

#define HORSE1_NAME "��1"
#define HORSE2_NAME "��2"
#define HORSE3_NAME "��3"

const int worldWidth = SCROLL_SIZE;
const int FinishLine = worldWidth - 5;