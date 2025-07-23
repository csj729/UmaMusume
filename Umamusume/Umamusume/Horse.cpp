#include "Horse.h"

Horse::Horse(const std::string& Newname)
	: m_name(Newname)
{
	// ��ų�� ���� �ӽ�, ���� ����� �Է� ����
	if (m_name == "��1")
	{
		m_speed = 3;
		m_hp = 5.0f;
		m_Position = { START_LINE, DF_BG_SIZE_Y - 10 };
		m_Direction = { (short)m_speed , 0 };
		m_skillList[0] = Skill("��ų1", 2, 5.0f);
		m_skillList[1] = Skill("��ų2", 3, 5.0f);
		m_skillList[2] = Skill("��ų3", 4, 5.0f);
	}

	else if (m_name == "��2")
	{
		m_speed = 4;
		m_hp = 5.0f;
		m_Position = { START_LINE, DF_BG_SIZE_Y - 9 };
		m_Direction = { (short)m_speed , 0 };
		m_skillList[0] = Skill("��ų1", 2, 5.0f);
		m_skillList[1] = Skill("��ų2", 3, 5.0f);
		m_skillList[2] = Skill("��ų3", 4, 5.0f);
	}

	else if (m_name == "��3")
	{
		m_speed = 5;
		m_hp = 5.0f;
		m_Position = { START_LINE, DF_BG_SIZE_Y - 8 };
		m_Direction = { (short)m_speed , 0 };
		m_skillList[0] = Skill("��ų1", 2, 5.0f);
		m_skillList[1] = Skill("��ų2", 3, 5.0f);
		m_skillList[2] = Skill("��ų3", 4, 5.0f);
	}
}

void Horse::HorseRender(Tile(*BG)[DF_BG_SIZE_X])
{
}

void Horse::HorseTick()
{
}

void Horse::SetName(const std::string Newname)
{
	m_name = Newname;
}

std::string Horse::GetName()
{
	return m_name;
}

void Horse::SetSpeed(const unsigned int Newspeed)
{
	m_speed = Newspeed;
}

unsigned int Horse::GetSpeed()
{
	return m_speed;
}

void Horse::SetHp(const float NewHp)
{
	m_hp = NewHp;
}

float Horse::GetHp()
{
	return m_hp;
}
