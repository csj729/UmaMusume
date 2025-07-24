#include "Horse.h"

void Horse::HorseRender(Tile(*BG)[DF_BG_SIZE_X], int scrollX)
{

	// ���� ��¼��� �����ϸ� �� �̻� �׸��� �ʱ�
	if (IsFinish())
		return;

	// ���� ��ǥ�� �ùٸ��� ���� ���� �� �׸��� �ʱ�
	int screenX = m_Position.X - scrollX;
	if (screenX < 0 || screenX >= DF_BG_SIZE_X)
		return;
	

	char symbol = 'H';
	if (m_name == HORSE1_NAME) symbol = '1';
	else if (m_name == HORSE2_NAME) symbol = '2';
	else if (m_name == HORSE3_NAME) symbol = '3';

	// ��ü
	BG[m_Position.Y][screenX].Text = symbol;

	// �Ӹ� (������)
	if (screenX + 1 < DF_BG_SIZE_X)
		BG[m_Position.Y][screenX + 1].Text = '>';

	// ���� (����)
	if (screenX - 1 >= 0)
		BG[m_Position.Y][screenX - 1].Text = '-';
}

void Horse::InitHorse()
{
	if (m_name == HORSE1_NAME)
	{
		m_baseSpeed = 4;
		m_Maxhp = 5.0f;
		m_hp = m_Maxhp;
		m_skillList[0] = Skill("��ų1", 2, 50.0f, 5.0f);
		m_skillList[1] = Skill("��ų2", 3, 50.0f, 5.0f);
		m_skillList[2] = Skill("��ų3", 4, 50.0f, 5.0f);
	}

	else if (m_name == HORSE2_NAME)
	{
		m_baseSpeed = 5;
		m_Maxhp = 5.0f;
		m_hp = m_Maxhp;
		m_skillList[0] = Skill("��ų1", 2, 10.0f, 5.0f);
		m_skillList[1] = Skill("��ų2", 3, 6.0f, 5.0f);
		m_skillList[2] = Skill("��ų3", 4, 4.0f, 5.0f);
	}

	else if (m_name == HORSE3_NAME)
	{
		m_baseSpeed = 6;
		m_Maxhp = 5.0f;
		m_hp = m_Maxhp;
		m_skillList[0] = Skill("��ų1", 2, 5.0f, 5.0f);
		m_skillList[1] = Skill("��ų2", 3, 3.0f, 5.0f);
		m_skillList[2] = Skill("��ų3", 4, 2.0f, 5.0f);
	}

}

std::string Horse::SelectName(const std::string horseName[])
{
	// HorseName�� �ִ� ���� �� �ϳ��� �����ϰ� ����
	int roll = rand() % HORSE_NUM;
	return m_name = horseName[roll];
}

void Horse::HorseTick()
{
	// �̵�
	if (isFinish)
		return;
	else
	{
		if (m_hp <= 0.0f)
			m_hp = 0.0f;
		else
			m_hp -= 0.05f;

		// ��� ����
		if (m_hp > m_Maxhp * 3.0f / 4.0f)
			m_vitStatus = ENERGETIC;
		else if (m_hp > m_Maxhp * 2.0f / 4.0f)
			m_vitStatus = NORMAL;
		else if (m_hp > m_Maxhp / 4.0f)
			m_vitStatus = TIRED;
		else
			m_vitStatus = EXHAUSTED;

		m_realSpeed = m_baseSpeed;
		switch (m_vitStatus)
		{
		case ENERGETIC:
			break;
		case NORMAL:
			m_realSpeed -= 1;
			break;
		case TIRED:
			m_realSpeed = -2;
			break;
		case EXHAUSTED:
			m_realSpeed = -3;
			break;
		}

		for (int i = 0; i < SKILL_NUM; ++i)
		{
			const Skill& skill = m_skillList[i];
			float randVal = static_cast<float>(rand()) / RAND_MAX * 100.0f;

			if (randVal < skill.GetChance())
			{
				// ��ų �ߵ�!
				m_realSpeed += skill.GetSpeed(); // ��ų�� ���� �ӵ� ����
				m_hp += skill.GetHp();           // ��ų�� ���� ü�� ȸ�� (or ����)

				// ü�� �ִ�ġ ���� �ʵ��� ����
				if (m_hp > m_Maxhp)
					m_hp = m_Maxhp;

				// ������ �޽���
				std::cout << m_name << " ���� " << skill.GetName() << " ��ų�� �ߵ�!\n";
			}

			if (m_realSpeed < 1)
				m_realSpeed = 1;
			m_Position.X += m_realSpeed;
		}

	}
}

bool Horse::IsFinish()
{
	if (m_Position.X >= FinishLine)
		return isFinish = true;
	else
		return isFinish = false;
}