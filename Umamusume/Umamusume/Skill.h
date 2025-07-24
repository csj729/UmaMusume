#pragma once
#include "Defines.h"

class Skill
{
private:
	std::string m_name;
	short m_speed;
	float m_chance;
	float m_hp;

public:
	Skill() : m_name(""), m_speed(0), m_chance(0.0f), m_hp(0.0f) {}
	Skill(const std::string& Newname, short Newspeed, float Newprob, float NewHp);

	void SetName(const std::string Newname) { m_name = Newname; };
	void SetSpeed(const short Newspeed) { m_speed = Newspeed; };
	void SetChance(const float Newprob) { m_chance = Newprob; };
	void SetHp(const float NewHp) { m_hp = NewHp; };

	std::string GetName() const { return m_name; };
	short GetSpeed() const { return m_speed; };
	float GetChance() const { return m_chance; };
	float GetHp() const { return m_hp; };
};

