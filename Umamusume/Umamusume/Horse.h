#pragma once
#include "Defines.h"
#include "Tile.h"
#include "Skill.h"

class Horse
{
private:
	std::string m_name;
	unsigned int m_speed;
	float m_hp;
	COORD m_Position; //위치
	COORD m_Direction; //이동 방향
	Skill m_skillList[SKILL_NUM];

public:
	Horse() : m_name(""), m_speed(0), m_hp(0.0f), m_Position({ 0, 0 }), m_Direction({ 0, 0 }) {}
	Horse(const std::string& Newname);
	void HorseRender(Tile(*BG)[DF_BG_SIZE_X]);
	void HorseTick();

	void SetName(const std::string Newname);
	void SetSpeed(const unsigned int Newspeed);
	void SetHp(const float NewHp);

	std::string GetName();
	unsigned int GetSpeed();
	float GetHp();

};

