#include "TrainingManager.h"

void TrainingManager::TrainingLoop(Horse& trainee)
{
	while (1)
	{
		// 트레이닝
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
				continue;
			}
			Training(trainee, (TrainingType)selNum);

		}
		// 레이스
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
	// 훈련 성공
	if (TrainingProbability(m_trainHp, m_trainMaxHp) >= randFloat)
	{
		int increaseStat = 0 , success = 1;
		randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		if (randFloat < 0.1f)
		{
			success = 2;
			trainee.SetHorseCondition(EXCELLENT);
		}
		// 훈련 성공 연출 보여주고
		// 정해진 범위 값만큼 스탯 상승, 체력 감소
		
		if (_type == SPEED)
			increaseStat = INCREASE_AMOUNT_SPEED * success;

		else if (_type == STAMINA)
			increaseStat = INCREASE_AMOUNT_STAMINA * success;

		else if (_type == INTELLIGENCE)
			increaseStat = INCREASE_AMOUNT_INTELLIGENCE * success;

		StatChange(trainee, increaseStat, _type);
		std::cout << "훈련 성공!! " << increaseStat << "만큼 상승했습니다!\n";
		Sleep(2000);
	}
	// 훈련 실패
	else
	{
		// 성공 시와 동일한 체력 감소 및 컨디션 하락, 이후 선택지 두 개 선택
		// 1. 선택한 훈련에 해당하는 스탯 감소, 일정 확률로 대실패(컨디션 최하 & 모든 스탯 감소)
		// 2. 재훈련, 성공하면 스탯 소폭 상승과 컨디션이 상승하나 실패시 무조건 대실패

		// 훈련 실패 연출 보여주기
		std::cout << "훈련에 실패했다.. 컨디션이 " << trainee.GetHorseCondition() << "에서 ";
		int condition = (int)trainee.GetHorseCondition();
		if (condition < 0)
			trainee.SetHorseCondition((HorseCondition)--condition);
		std::cout << trainee.GetHorseCondition() << "으로 감소했다 ㅠ\n";
		
		while(1)
		{
			std::cout << "1. 휴식을 하는게 맞아..\n";
			std::cout << "2. 훈련을 강행해야돼!\n";
			std::cout << "어떻게 하지? ";

			int selNum;
			std::cin >> selNum;
			int decreaseStat = 0;
			// 휴식
			if (selNum == 1)
			{
				randFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				// 대실패
				if (randFloat < 0.1f)
				{
					StatChange(trainee, decreaseStat, SPEED);
					StatChange(trainee, decreaseStat, STAMINA);
					StatChange(trainee, decreaseStat, INTELLIGENCE);
					trainee.SetHorseCondition(VERYPOOR);
					std::cout << "훈련 대실패!! " << decreaseStat << "만큼 모든 스탯이 감소했습니다!\n";
					Sleep(2000);
					break;
				}
				// 실패
				StatChange(trainee, decreaseStat, _type);
				std::cout << "훈련 실패!! " << decreaseStat << "만큼 스탯이 감소했습니다!\n";
				Sleep(2000);
				break;
			}
			// 훈련 재개
			else if (selNum == 2)
			{

			}
			else
			{

			}
		}


	}
}

// 로그 함수로 훈련 성공 확률 감소
float TrainingManager::TrainingProbability(int H, int MaxH)
{
	float x = H / (float)MaxH; // 0 ~ 1 정규화

	if (x >= 0.5f)
	{
		return 1.0f;  // 체력이 절반 이상이면 성공 확률 100%
	}
	else
	{
		// 체력이 절반 미만일 경우 감소
		const float a = 9.0f; // 조절 상수 (값이 클수록 급격히 감소)
		float adjustedX = x * 2.0f; // 0.0 ~ 1.0로 다시 정규화 (0.5 -> 1.0, 0 -> 0)
		float numerator = std::log(a * adjustedX + 1.0f);
		float denominator = std::log(a + 1.0f);

		return numerator / denominator; // 0 ~ 1 사이 확률 반환
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
