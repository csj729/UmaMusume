#include "TrainingManager.h"

void TrainingManager::TrainingLoop(Horse& trainee)
{
	while (1)
	{
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
			}
			Training(trainee, (TrainingType)selNum);


		}


	}
}

void TrainingManager::Training(Horse& trainee, TrainingType _type)
{
	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	// �Ʒ� ����
	if (TrainingProbability(m_trainHp, m_trainMaxHp) >= randFloat)
	{
		// Ư�� ���� ����ŭ ���� ���, ü�� ���� ����
	}
	// �Ʒ� ����
	else
	{
		// ü�� ���緮 ����. ���� �ú��ٴ� ���� �����ϵ���
	}
}

float TrainingManager::TrainingProbability(float H, float MaxH)
{
	float x = H / MaxH; // 0~1 ����ȭ
	const float a = 9.0f; // ���� ���

	float numerator = std::log(a * x + 1.0f);
	float denominator = std::log(a + 1.0f);

	return numerator / denominator; // 0~1 ���� Ȯ�� ��ȯ
}