#pragma once
#include "Defines.h"

class Skill
{
private:
	std::string m_name;
	unsigned int m_speed;
	float m_hp;

public:
	Skill() : m_name(""), m_speed(0), m_hp(0.0f) {}
	Skill(const std::string& Newname, unsigned int Newspeed, float NewHp);

	void SetName(const std::string Newname);
	void SetSpeed(const unsigned int Newspeed);
	void SetHp(const float NewHp);

	std::string GetName();
	unsigned int GetSpeed();
	float GetHp();
};

