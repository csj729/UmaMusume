#include "TrainingManager.h"

void TrainingManager::TrainingLoop(Horse& trainee)
{
	while (1)
	{
		// Ʈ���̴�
		if(m_trainingStage % 5 != 0)
		{ 
			std::cout << "=======[ " << m_trainingStage << "���� �Ʒ� ]========" << std::endl;
			std::cout << "1. �ӵ� �Ʒ�" << std::endl;
			std::cout << "2. ������ �Ʒ�" << std::endl;
			std::cout << "3. ���� �Ʒ�" << std::endl;
			std::cout << "=======================" << std::endl;
			std::cout << ">> ����: ";

			int selNum;
			std::cin >> selNum;

			if (selNum < SPEED || selNum > INTELLIGENCE)
			{
				std::cout << "�ùٸ��� ���� ���Դϴ�!! �ٽ� �Է����ּ���. (1~3)";
				continue;
			}
			Training(trainee, (TrainingType)selNum);

		}
		// ���̽�
		else
		{

		}


	}
}

void TrainingManager::Training(Horse& trainee, TrainingType _type)
{
	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	this->m_trainHp -= DECREASE_HP_TRAINING;
	if (this->m_trainHp < 0)
		this->m_trainHp = 0;
	// �Ʒ� ����
	if (TrainingProbability(m_trainHp, m_trainMaxHp) >= randFloat)
	{
		int increaseStat = 0 , success = 1;
		randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		if (randFloat < 0.1f)
		{
			success = 2;
			trainee.SetHorseCondition(EXCELLENT);
		}
		// �Ʒ� ���� ���� �����ְ�
		// ������ ���� ����ŭ ���� ���, ü�� ����
		
		if (_type == SPEED)
			increaseStat = INCREASE_AMOUNT_SPEED * success;

		else if (_type == STAMINA)
			increaseStat = INCREASE_AMOUNT_STAMINA * success;

		else if (_type == INTELLIGENCE)
			increaseStat = INCREASE_AMOUNT_INTELLIGENCE * success;

		StatChange(trainee, increaseStat, _type);
		std::cout << "�Ʒ� ����!! " << increaseStat << "��ŭ ����߽��ϴ�!\n";
		Sleep(2000);
	}
	// �Ʒ� ����
	else
	{
		// ���� �ÿ� ������ ü�� ���� �� ����� �϶�, ���� ������ �� �� ����
		// 1. ������ �Ʒÿ� �ش��ϴ� ���� ����, ���� Ȯ���� �����(����� ���� & ��� ���� ����)
		// 2. ���Ʒ�, �����ϸ� ���� ���� ��°� ������� ����ϳ� ���н� ������ �����

		// �Ʒ� ���� ���� �����ֱ�
		std::cout << "�Ʒÿ� �����ߴ�.. ������� " << trainee.GetHorseCondition() << "���� ";
		int condition = (int)trainee.GetHorseCondition();
		if (condition < 0)
			trainee.SetHorseCondition((HorseCondition)--condition);
		std::cout << trainee.GetHorseCondition() << "���� �����ߴ� ��\n";
		
		while(1)
		{
			std::cout << "1. �޽��� �ϴ°� �¾�..\n";
			std::cout << "2. �Ʒ��� �����ؾߵ�!\n";
			std::cout << "��� ����? ";

			int selNum;
			std::cin >> selNum;
			int decreaseStat = 0;
			// �޽�
			if (selNum == 1)
			{
				randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				// �����
				if (randFloat < 0.1f)
				{
					StatChange(trainee, decreaseStat, SPEED);
					StatChange(trainee, decreaseStat, STAMINA);
					StatChange(trainee, decreaseStat, INTELLIGENCE);
					trainee.SetHorseCondition(VERYPOOR);
					std::cout << "�Ʒ� �����!! " << decreaseStat << "��ŭ ��� ������ �����߽��ϴ�!\n";
					Sleep(2000);
					break;
				}
				// ����
				StatChange(trainee, decreaseStat, _type);
				std::cout << "�Ʒ� ����!! " << decreaseStat << "��ŭ ������ �����߽��ϴ�!\n";
				Sleep(2000);
				break;
			}
			// �Ʒ� �簳
			else if (selNum == 2)
			{

			}
			else
			{

			}
		}


	}
}

// �α� �Լ��� �Ʒ� ���� Ȯ�� ����
float TrainingManager::TrainingProbability(int H, int MaxH)
{
	float x = H / (float)MaxH; // 0 ~ 1 ����ȭ

	if (x >= 0.5f)
	{
		return 1.0f;  // ü���� ���� �̻��̸� ���� Ȯ�� 100%
	}
	else
	{
		// ü���� ���� �̸��� ��� ����
		const float a = 9.0f; // ���� ��� (���� Ŭ���� �ް��� ����)
		float adjustedX = x * 2.0f; // 0.0 ~ 1.0�� �ٽ� ����ȭ (0.5 -> 1.0, 0 -> 0)
		float numerator = std::log(a * adjustedX + 1.0f);
		float denominator = std::log(a + 1.0f);

		return numerator / denominator; // 0 ~ 1 ���� Ȯ�� ��ȯ
	}
}

void TrainingManager::StatChange(Horse& trainee, int statModifier, TrainingType _type)
{
	if (_type == SPEED)
	{
		statModifier += trainee.GetBaseSpeed();
		trainee.SetBaseSpeed(statModifier);
	}
	else if (_type == STAMINA)
	{
		statModifier += trainee.GetMaxHp();
		trainee.SetMaxHp(statModifier);
	}
	else if (_type == INTELLIGENCE)
	{
		statModifier += trainee.GetIntel();
		trainee.SetIntel(statModifier);
	}
}
