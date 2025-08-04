#include "TrainingManager.h"

std::vector<Skill>& skills = SkillDatabase::GetAllSkills();

Horse TrainingManager::SelectTrainee()
{
	Horse trainee;
	while (true)
	{
		std::cout << "육성할 말을 골라주세요!\n";
		for (int i = 0; i < HORSETABLE_NUM; ++i)
			std::cout << i + 1 << ". " << HorseName[i] << "  ";
		std::cout << "\n당신의 말은??? ";

		int selNum;
		std::cin >> selNum;

		if (selNum < 1 || selNum > HORSETABLE_NUM)
		{
			std::cout << "잘못된 번호!! 다시 입력해주세요.\n";
			Sleep(500);
			system("cls");
			continue;
		}

		trainee.SetName(HorseName[selNum - 1]);
		trainee.InitHorse();
		trainee.SetBaseSpeed(0);
		return trainee;
	}
}

void TrainingManager::InitTrainingManager()
{
	m_trainingStage = 1;
	m_trainMaxHp = 100;
	m_trainHp = m_trainMaxHp;
}

void TrainingManager::TrainingLoop(Horse& trainee, std::vector<Horse>& PlayerList, RaceManager& RM, DoubleBuffering& DB, Tile(*_BG)[DF_BG_SIZE_X], Horse* AI_horse[])
{
	trainee = SelectTrainee();
	InitTrainingManager();

	while (m_trainingStage <= TRAINGING_DAYS)
	{
		system("cls");

		if (m_trainingStage % 5 != 0) // 훈련
		{
			PrintTrainingMenu(trainee);
			int selNum = GetTrainingSelection();
			if (selNum == REST)
				Rest(trainee);
			else
				Training(trainee, static_cast<TrainingType>(selNum));
		}
		else // 레이스
		{
			Horse* RaceHorses[HORSE_NUM];

			ShowSkillHintMenu(trainee);
			RM.RunRace(DB, _BG, RaceHorses, trainee);
			StatIncreaseAfterRace(trainee, RM);
			m_trainingStage++;	
		}

	}

	PrintTrainingResult(trainee);
	HandleSaveOrDiscard(trainee, PlayerList);
}

void TrainingManager::PrintTrainingMenu(const Horse& trainee)
{
	std::cout << "          " << trainee.GetName() << "\n";
	std::cout << "=======[ " << m_trainingStage++ << "일차 훈련 ]========\n";
	std::cout << "1. 속도 훈련\n2. 지구력 훈련\n3. 지능 훈련\n4. 휴식\n";
	std::cout << "=======================\n";
	PrintStatus(trainee);
	std::cout << ">> 선택: ";
}

int TrainingManager::GetTrainingSelection() const
{
	int selNum;
	while (true)
	{
		std::cin >> selNum;
		if (selNum >= SPEED && selNum <= REST)
			return selNum;
		std::cout << "올바르지 않은 값입니다!! 다시 입력해주세요. (1~4): ";
	}
}

void TrainingManager::Training(Horse& trainee, TrainingType _type)
{
	DecreaseTrainingHp();

	int increaseStat = 0;
	int decreaseStat = 0;

	float randFloat = static_cast<float>(rand()) / RAND_MAX;
	if (TrainingProbability(m_trainHp, m_trainMaxHp) >= randFloat)
	{
		// 스킬 힌트 Level 상승
		randFloat = static_cast<float>(rand()) / RAND_MAX; // 예: 30% 확률
		if (0.3f > randFloat)
		{
			// 무작위 스킬 하나 선택
			int randIndex = rand() % skills.size();
			skills[randIndex].IncreaseHintLevel();

			std::cout << "[힌트] \"" << skills[randIndex].GetName() << "\" 힌트 레벨이 "
				<< skills[randIndex].GetHintLevel() << "이 되었습니다!\n\n";
		}

		randFloat = static_cast<float>(rand()) / RAND_MAX;
		if (randFloat < 0.2f) // 대성공
		{
			increaseStat = GetIncreaseAmount(_type) * 2;
			IncreaseCondition(trainee);
			StatChange(trainee, increaseStat, _type);
			std::cout << "훈련 대성공!! " << increaseStat << "만큼 스탯이 상승했습니다!!\n";
			std::cout << "컨디션이 한 단계 상승되었습니다!!\n";
		}
		else // 성공
		{
			increaseStat = GetIncreaseAmount(_type);
			StatChange(trainee, increaseStat, _type);
			std::cout << "훈련 성공!! " << increaseStat << "만큼 상승했습니다!\n";
		}
		Sleep(2000);
	}
	else // 실패
	{
		HandleTrainingFailure(trainee, _type);
	}
}

void TrainingManager::DecreaseTrainingHp()
{
	m_trainHp -= DECREASE_HP_TRAINING;
	if (m_trainHp < 0)
		m_trainHp = 0;
}

int TrainingManager::GetIncreaseAmount(TrainingType type) const
{
	switch (type)
	{
	case SPEED: return INCREASE_AMOUNT_SPEED;
	case STAMINA: return INCREASE_AMOUNT_STAMINA;
	case INTELLIGENCE: return INCREASE_AMOUNT_INTELLIGENCE;
	default: return 0;
	}
}

int TrainingManager::GetDecreaseAmount(TrainingType type) const
{
	switch (type)
	{
	case SPEED: return DECREASE_AMOUNT_SPEED;
	case STAMINA: return DECREASE_AMOUNT_STAMINA;
	case INTELLIGENCE: return DECREASE_AMOUNT_INTELLIGENCE;
	default: return 0;
	}
}

void TrainingManager::IncreaseCondition(Horse& trainee)
{
	int condition = static_cast<int>(trainee.GetHorseCondition());
	if (condition >= VERYPOOR && condition < EXCELLENT)
		trainee.SetHorseCondition(static_cast<HorseCondition>(condition + 1));
}

void TrainingManager::DecreaseCondition(Horse& trainee)
{
	int condition = static_cast<int>(trainee.GetHorseCondition());
	if (condition > VERYPOOR)
		trainee.SetHorseCondition(static_cast<HorseCondition>(condition - 1));
}

void TrainingManager::HandleTrainingFailure(Horse& trainee, TrainingType _type)
{
	std::cout << "훈련에 실패했다.. 컨디션이 " << PrintCondition(trainee) << "에서 ";
	DecreaseCondition(trainee);
	std::cout << PrintCondition(trainee) << "으로 감소했다...\n";

	while (true)
	{
		std::cout << "1. 휴식을 하는게 맞아..\n2. 훈련을 강행해야돼!\n어떻게 하지? ";

		int selNum;
		std::cin >> selNum;

		if (selNum == 1)
		{
			HandleRestDuringFailure(trainee, _type);
			break;
		}
		else if (selNum == 2)
		{
			HandleRetryTraining(trainee, _type);
			break;
		}
		else
		{
			std::cout << "올바른 값을 입력해주세요!\n";
			system("pause");
			system("cls");
		}
	}
}

void TrainingManager::HandleRestDuringFailure(Horse& trainee, TrainingType _type)
{
	float randFloat = static_cast<float>(rand()) / RAND_MAX;
	if (randFloat < 0.1f) // 대실패
	{
		ApplyCriticalFailure(trainee);
		std::cout << "훈련 대실패!! 모든 스탯이 감소했습니다!\n";
		std::cout << "컨디션이 최악으로 변경되었습니다..\n";
	}
	else
	{
		int decreaseStat = GetDecreaseAmount(_type);
		StatChange(trainee, decreaseStat, _type);
		std::cout << "훈련 실패!! " << decreaseStat << "만큼 스탯이 감소했습니다!\n";
	}
	Sleep(2000);
}

void TrainingManager::HandleRetryTraining(Horse& trainee, TrainingType _type)
{
	std::cout << "훈련 재개!!\n";
	float randFloat = static_cast<float>(rand()) / RAND_MAX;
	Sleep(500);

	if (randFloat < 0.2f) // 재훈련 성공
	{
		int increaseStat = GetIncreaseAmount(_type);
		StatChange(trainee, increaseStat, _type);
		IncreaseCondition(trainee);

		std::cout << "재훈련 성공! 컨디션이 " << PrintCondition(trainee) << "에서 ";
		std::cout << PrintCondition(trainee) << "으로 상승했다!\n";
	}
	else // 재훈련 실패, 대실패
	{
		ApplyCriticalFailure(trainee);
		std::cout << "재훈련 실패! 모든 스탯이 감소했습니다!\n";
		std::cout << "컨디션이 최악으로 변경되었습니다..\n";
	}
	Sleep(2000);
}

void TrainingManager::ApplyCriticalFailure(Horse& trainee)
{
	int decreaseStat = DECREASE_CRITICAL_CASE;
	StatChange(trainee, decreaseStat, SPEED);
	StatChange(trainee, decreaseStat, STAMINA);
	StatChange(trainee, decreaseStat, INTELLIGENCE);
	trainee.SetHorseCondition(VERYPOOR);
}

void TrainingManager::Rest(Horse& trainee)
{
	float randFloat = static_cast<float>(rand()) / RAND_MAX;
	int condition = static_cast<int>(trainee.GetHorseCondition());

	if (randFloat > 0.3f) // 일반 휴식
	{
		RecoverTrainHp(INCREASE_AMOUNT_TRAINHP);
		std::cout << "휴식을 취했다! 체력이 " << INCREASE_AMOUNT_TRAINHP << "만큼 회복됐다!\n";
		std::cout << "컨디션이 " << PrintCondition(trainee) << "에서 ";
		IncreaseCondition(trainee);
		std::cout << PrintCondition(trainee) << "으로 상승했다!\n";
	}
	else if (randFloat > 0.1f) // 대성공 휴식
	{
		RecoverTrainHp(INCREASE_AMOUNT_TRAINHP + 20);
		std::cout << "휴식을 취했다! 잠을 푹 잤다. 체력이 " << INCREASE_AMOUNT_TRAINHP + 20 << "만큼 회복됐다!\n";
		std::cout << "컨디션이 " << PrintCondition(trainee) << "에서 ";
		trainee.SetHorseCondition(EXCELLENT);
		std::cout << PrintCondition(trainee) << "으로 상승했다!\n";
	}
	else // 휴식 보너스 감소
	{
		RecoverTrainHp(INCREASE_AMOUNT_TRAINHP - 20);
		std::cout << "휴식을 취했다! 제대로 잠을 자지 못했다. 체력이 " << INCREASE_AMOUNT_TRAINHP - 20 << "만큼 회복됐다!\n";
	}
	Sleep(1500);
}

void TrainingManager::RecoverTrainHp(int amount)
{
	m_trainHp += amount;
	if (m_trainHp > m_trainMaxHp)
		m_trainHp = m_trainMaxHp;
}

void TrainingManager::PrintStatus(const Horse& trainee) const
{
	std::cout << "현재 체력 : " << m_trainHp;
	std::cout << "    컨디션 : " << PrintCondition(trainee) << "\n";

	std::cout << std::setw(10) << std::left << "스피드 : " << trainee.GetBaseSpeed()
		<< std::setw(12) << std::left << "  스태미나 : " << trainee.GetMaxStamina()
		<< std::setw(10) << std::left << "  지능 : " << trainee.GetIntel() << "\n\n";
}

std::string TrainingManager::PrintCondition(const Horse& trainee) const
{
	switch (trainee.GetHorseCondition())
	{
	case VERYPOOR: return "최저";
	case POOR: return "낮음";
	case FEELSOSO: return "보통";
	case GOOD: return "양호";
	case EXCELLENT: return "최상";
	default: return "알수없음";
	}
}

float TrainingManager::TrainingProbability(int H, int MaxH) const
{
	float x = static_cast<float>(H) / MaxH;

	if (x >= 0.5f)
		return 1.0f;

	const float a = 9.0f;
	float adjustedX = x * 2.0f;
	float numerator = std::log(a * adjustedX + 1.0f);
	float denominator = std::log(a + 1.0f);
	return numerator / denominator;
}

void TrainingManager::StatChange(Horse& trainee, int statModifier, TrainingType _type)
{
	if (_type == SPEED)
	{
		int newSpeed = trainee.GetBaseSpeed() + statModifier;
		trainee.SetBaseSpeed(newSpeed < 0 ? 0 : newSpeed);
	}
	else if (_type == STAMINA)
	{
		float newStamina = trainee.GetMaxStamina() + statModifier;
		trainee.SetMaxStamina(newStamina < 0 ? 0 : newStamina);
	}
	else if (_type == INTELLIGENCE)
	{
		int newIntel = trainee.GetIntel() + statModifier;
		trainee.SetIntel(newIntel < 0 ? 0 : newIntel);
	}
}

void TrainingManager::SaveHorse(const Horse& trainee, std::vector<Horse>& PlayerList)
{
	PlayerList.push_back(trainee);
}

void TrainingManager::PrintTrainingResult(const Horse& trainee) const
{
	std::cout << trainee.GetName() << " 육성 완료!!\n\n";
	std::cout << "최종 스탯\n\n";

	std::cout << std::left;
	std::cout << std::setw(12) << "스피드" << ": " << std::setw(6) << trainee.GetBaseSpeed() << "\n";
	std::cout << std::setw(12) << "스태미나" << ": " << std::setw(6) << trainee.GetMaxStamina() << "\n";
	std::cout << std::setw(12) << "지능" << ": " << std::setw(6) << trainee.GetIntel() << "\n\n";
}

void TrainingManager::HandleSaveOrDiscard(Horse& trainee, std::vector<Horse>& PlayerList)
{
	while (true)
	{
		char yOrn;
		std::cout << "우마무스메를 저장하시겠습니까?? (Y / N) ";
		std::cin >> yOrn;

		if (yOrn == 'Y' || yOrn == 'y')
		{
			SaveHorse(trainee, PlayerList);
			break;
		}
		else if (yOrn == 'N' || yOrn == 'n')
		{
			std::cout << "안녕 " << trainee.GetName() << "....\n";
			break;
		}
		else
		{
			std::cout << "잘못된 입력입니다.\n";
			system("cls");
		}
		Sleep(1000);
	}
}

void TrainingManager::StatIncreaseAfterRace(Horse& trainee, RaceManager& RM)
{
	if (&trainee == RM.GetRankedHorse(0))
	{
		StatChange(trainee, 3, SPEED);
		StatChange(trainee, 3, STAMINA);
		StatChange(trainee, 30, INTELLIGENCE);
		SetSP(GetSP() + 50);
		std::cout << "1등 !! 모든 스탯이 3만큼 상승했다!! 스킬포인트 50을 얻었다!!\n";
	}

	else if (&trainee == RM.GetRankedHorse(1))
	{
		StatChange(trainee, 2, SPEED);
		StatChange(trainee, 2, STAMINA);
		StatChange(trainee, 20, INTELLIGENCE);
		SetSP(GetSP() + 50);
		std::cout << "2등 !! 모든 스탯이 2만큼 상승했다!! 스킬포인트 50을 얻었다!!\n";
	}

	else if (&trainee == RM.GetRankedHorse(2))
	{
		StatChange(trainee, 1, SPEED);
		StatChange(trainee, 1, STAMINA);
		StatChange(trainee, 10, INTELLIGENCE);
		SetSP(GetSP() + 50);
		std::cout << "3등 !! 모든 스탯이 1만큼 상승했다!! 스킬포인트 50을 얻었다!!\n";
	}

	else
	{
		SetSP(GetSP() + 50);
		std::cout << "스킬포인트 50을 얻었다!!\n";
	}

	Sleep(2000);
}

void TrainingManager::ShowSkillHintMenu(Horse& trainee)
{
	std::cout << "\n--- 배울 수 있는 스킬 목록 ---\n";

	bool hasAvailable = false;

	for (Skill& skill : skills)
	{
		if (skill.GetHintLevel() > 0 && !skill.IsLearned())
		{
			hasAvailable = true;
			int cost = skill.GetEffectiveCost();

			std::cout << skill.GetName() << " (힌트 Lv: " << skill.GetHintLevel()
				<< ", 필요 SP: " << cost << ") -> ";

			if (m_SP >= cost)
				std::cout << "[배울 수 있음]\n";
			else
				std::cout << "[SP 부족]\n";
		}
	}

	if (!hasAvailable)
	{
		std::cout << "배울 수 있는 스킬이 없습니다.\n\n";
		Sleep(1500);
		return;
	}

	std::cout << "\n스킬을 배우시겠습니까? (Y/N): ";
	char choice;
	std::cin >> choice;
	if (choice != 'Y' && choice != 'y') return;

	for (Skill& skill : skills)
	{
		if (skill.GetHintLevel() > 0 && !skill.IsLearned())
		{
			int cost = skill.GetEffectiveCost();
			if (m_SP < cost) continue;

			std::cout << "\n'" << skill.GetName() << "' 스킬을 배우시겠습니까? (Y/N): ";
			char learnChoice;
			std::cin >> learnChoice;

			if (learnChoice == 'Y' || learnChoice == 'y')
			{
				m_SP -= cost;
				skill.SetLearned(true);

				trainee.InputSkill(skill);
				std::cout << skill.GetName() << " 스킬을 배웠습니다! (남은 SP: " << m_SP << ")\n";
			}
		}
	}

	std::cout << "\n";
	Sleep(2000);
}