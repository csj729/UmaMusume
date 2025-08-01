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

// ��� ũ��
#define DF_BG_SIZE_X 200
#define DF_BG_SIZE_Y 40

// Ʈ�� ��, ���� ����
#define LANE_HEIGHT 9       // �� ������ ����
#define LANE_SPACING 1      // ���� ���� ���� (optional)
#define LANE_NUM 4
#define TRACK_START_Y 0     // Ʈ���� ���� Y ��ǥ
#define START_LINE_X 3
#define SCROLL_SIZE 2000	// Ʈ�� ����

// �÷��� ������Ʈ ����
#define HORSE_NUM 4
#define HORSETABLE_NUM 4
#define SKILL_NUM 3


const int worldWidth = SCROLL_SIZE;
const int FinishLine = worldWidth - 5;

enum Action
{
	TRAINING = 1,
	RACE,
	SAVE,
	LOAD,
	UMALIST,
	FINISH
};