#include "Skill.h"

Skill::Skill(const std::string& Newname, unsigned int Newspeed, float NewHp)
	: m_name(Newname), m_speed(Newspeed), m_hp(NewHp)
{
	
}

void Skill::SetName(const std::string Newname)
{
	m_name = Newname;
}

std::string Skill::GetName()
{
	return m_name;
}

void Skill::SetSpeed(const unsigned int Newspeed)
{
	m_speed = Newspeed;
}

unsigned int Skill::GetSpeed()
{
	return m_speed;
}

void Skill::SetHp(const float NewHp)
{
	m_hp = NewHp;
}

float Skill::GetHp()
{
	return m_hp;
}
