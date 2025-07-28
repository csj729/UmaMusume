#include "TrainingManager.h"

void TrainingManager::TrainingLoop(Horse& trainee)
{
	while (1)
	{
		if(m_trainingStage % 5 != 0)
		{ 
			std::cout << "=======[ " << m_trainingStage << "일차 훈련 ]========" << std::endl;
			std::cout << "1. 속도 훈련" << std::endl;
			std::cout << "2. 지구력 훈련" << std::endl;
			std::cout << "3. 지능 훈련" << std::endl;
			std::cout << "=======================" << std::endl;
			std::cout << ">> 선택: ";

			int selNum;
			std::cin >> selNum;

			if (selNum < SPEED || selNum > INTELLIGENCE)
			{
				std::cout << "올바르지 않은 값입니다!! 다시 입력해주세요. (1~3)";
			}
			Training(trainee, (TrainingType)selNum);


		}


	}
}

void TrainingManager::Training(Horse& trainee, TrainingType _type)
{
	float randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	// 훈련 성공
	if (TrainingProbability(m_trainHp, m_trainMaxHp) >= randFloat)
	{
		// 특정 범위 값만큼 스탯 상승, 체력 소폭 감소
	}
	// 훈련 실패
	else
	{
		// 체력 적당량 감소. 성공 시보다는 많이 감소하도록
	}
}

float TrainingManager::TrainingProbability(float H, float MaxH)
{
	float x = H / MaxH; // 0~1 정규화
	const float a = 9.0f; // 조절 상수

	float numerator = std::log(a * x + 1.0f);
	float denominator = std::log(a + 1.0f);

	return numerator / denominator; // 0~1 사이 확률 반환
}