#include "Skill.h"

Skill::Skill(const std::string& Newname, short Newspeed, float Newprob, float NewHp)
	: m_name(Newname), m_speed(Newspeed), m_chance(Newprob), m_hp(NewHp)
{
	
}
