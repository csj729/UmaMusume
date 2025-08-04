#include "TrainingManager.h"

std::vector<Skill>& skills = SkillDatabase::GetAllSkills();

Horse TrainingManager::SelectTrainee()
{
	Horse trainee;
	while (true)
	{
		std::cout << "������ ���� ����ּ���!\n";
		for (int i = 0; i < HORSETABLE_NUM; ++i)
			std::cout << i + 1 << ". " << HorseName[i] << "  ";
		std::cout << "\n����� ����??? ";

		int selNum;
		std::cin >> selNum;

		if (selNum < 1 || selNum > HORSETABLE_NUM)
		{
			std::cout << "�߸��� ��ȣ!! �ٽ� �Է����ּ���.\n";
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

		if (m_trainingStage % 5 != 0) // �Ʒ�
		{
			PrintTrainingMenu(trainee);
			int selNum = GetTrainingSelection();
			if (selNum == REST)
				Rest(trainee);
			else
				Training(trainee, static_cast<TrainingType>(selNum));
		}
		else // ���̽�
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
	std::cout << "=======[ " << m_trainingStage++ << "���� �Ʒ� ]========\n";
	std::cout << "1. �ӵ� �Ʒ�\n2. ������ �Ʒ�\n3. ���� �Ʒ�\n4. �޽�\n";
	std::cout << "=======================\n";
	PrintStatus(trainee);
	std::cout << ">> ����: ";
}

int TrainingManager::GetTrainingSelection() const
{
	int selNum;
	while (true)
	{
		std::cin >> selNum;
		if (selNum >= SPEED && selNum <= REST)
			return selNum;
		std::cout << "�ùٸ��� ���� ���Դϴ�!! �ٽ� �Է����ּ���. (1~4): ";
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
		// ��ų ��Ʈ Level ���
		randFloat = static_cast<float>(rand()) / RAND_MAX; // ��: 30% Ȯ��
		if (0.3f > randFloat)
		{
			// ������ ��ų �ϳ� ����
			int randIndex = rand() % skills.size();
			skills[randIndex].IncreaseHintLevel();

			std::cout << "[��Ʈ] \"" << skills[randIndex].GetName() << "\" ��Ʈ ������ "
				<< skills[randIndex].GetHintLevel() << "�� �Ǿ����ϴ�!\n\n";
		}

		randFloat = static_cast<float>(rand()) / RAND_MAX;
		if (randFloat < 0.2f) // �뼺��
		{
			increaseStat = GetIncreaseAmount(_type) * 2;
			IncreaseCondition(trainee);
			StatChange(trainee, increaseStat, _type);
			std::cout << "�Ʒ� �뼺��!! " << increaseStat << "��ŭ ������ ����߽��ϴ�!!\n";
			std::cout << "������� �� �ܰ� ��µǾ����ϴ�!!\n";
		}
		else // ����
		{
			increaseStat = GetIncreaseAmount(_type);
			StatChange(trainee, increaseStat, _type);
			std::cout << "�Ʒ� ����!! " << increaseStat << "��ŭ ����߽��ϴ�!\n";
		}
		Sleep(2000);
	}
	else // ����
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
	std::cout << "�Ʒÿ� �����ߴ�.. ������� " << PrintCondition(trainee) << "���� ";
	DecreaseCondition(trainee);
	std::cout << PrintCondition(trainee) << "���� �����ߴ�...\n";

	while (true)
	{
		std::cout << "1. �޽��� �ϴ°� �¾�..\n2. �Ʒ��� �����ؾߵ�!\n��� ����? ";

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
			std::cout << "�ùٸ� ���� �Է����ּ���!\n";
			system("pause");
			system("cls");
		}
	}
}

void TrainingManager::HandleRestDuringFailure(Horse& trainee, TrainingType _type)
{
	float randFloat = static_cast<float>(rand()) / RAND_MAX;
	if (randFloat < 0.1f) // �����
	{
		ApplyCriticalFailure(trainee);
		std::cout << "�Ʒ� �����!! ��� ������ �����߽��ϴ�!\n";
		std::cout << "������� �־����� ����Ǿ����ϴ�..\n";
	}
	else
	{
		int decreaseStat = GetDecreaseAmount(_type);
		StatChange(trainee, decreaseStat, _type);
		std::cout << "�Ʒ� ����!! " << decreaseStat << "��ŭ ������ �����߽��ϴ�!\n";
	}
	Sleep(2000);
}

void TrainingManager::HandleRetryTraining(Horse& trainee, TrainingType _type)
{
	std::cout << "�Ʒ� �簳!!\n";
	float randFloat = static_cast<float>(rand()) / RAND_MAX;
	Sleep(500);

	if (randFloat < 0.2f) // ���Ʒ� ����
	{
		int increaseStat = GetIncreaseAmount(_type);
		StatChange(trainee, increaseStat, _type);
		IncreaseCondition(trainee);

		std::cout << "���Ʒ� ����! ������� " << PrintCondition(trainee) << "���� ";
		std::cout << PrintCondition(trainee) << "���� ����ߴ�!\n";
	}
	else // ���Ʒ� ����, �����
	{
		ApplyCriticalFailure(trainee);
		std::cout << "���Ʒ� ����! ��� ������ �����߽��ϴ�!\n";
		std::cout << "������� �־����� ����Ǿ����ϴ�..\n";
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

	if (randFloat > 0.3f) // �Ϲ� �޽�
	{
		RecoverTrainHp(INCREASE_AMOUNT_TRAINHP);
		std::cout << "�޽��� ���ߴ�! ü���� " << INCREASE_AMOUNT_TRAINHP << "��ŭ ȸ���ƴ�!\n";
		std::cout << "������� " << PrintCondition(trainee) << "���� ";
		IncreaseCondition(trainee);
		std::cout << PrintCondition(trainee) << "���� ����ߴ�!\n";
	}
	else if (randFloat > 0.1f) // �뼺�� �޽�
	{
		RecoverTrainHp(INCREASE_AMOUNT_TRAINHP + 20);
		std::cout << "�޽��� ���ߴ�! ���� ǫ ���. ü���� " << INCREASE_AMOUNT_TRAINHP + 20 << "��ŭ ȸ���ƴ�!\n";
		std::cout << "������� " << PrintCondition(trainee) << "���� ";
		trainee.SetHorseCondition(EXCELLENT);
		std::cout << PrintCondition(trainee) << "���� ����ߴ�!\n";
	}
	else // �޽� ���ʽ� ����
	{
		RecoverTrainHp(INCREASE_AMOUNT_TRAINHP - 20);
		std::cout << "�޽��� ���ߴ�! ����� ���� ���� ���ߴ�. ü���� " << INCREASE_AMOUNT_TRAINHP - 20 << "��ŭ ȸ���ƴ�!\n";
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
	std::cout << "���� ü�� : " << m_trainHp;
	std::cout << "    ����� : " << PrintCondition(trainee) << "\n";

	std::cout << std::setw(10) << std::left << "���ǵ� : " << trainee.GetBaseSpeed()
		<< std::setw(12) << std::left << "  ���¹̳� : " << trainee.GetMaxStamina()
		<< std::setw(10) << std::left << "  ���� : " << trainee.GetIntel() << "\n\n";
}

std::string TrainingManager::PrintCondition(const Horse& trainee) const
{
	switch (trainee.GetHorseCondition())
	{
	case VERYPOOR: return "����";
	case POOR: return "����";
	case FEELSOSO: return "����";
	case GOOD: return "��ȣ";
	case EXCELLENT: return "�ֻ�";
	default: return "�˼�����";
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
	std::cout << trainee.GetName() << " ���� �Ϸ�!!\n\n";
	std::cout << "���� ����\n\n";

	std::cout << std::left;
	std::cout << std::setw(12) << "���ǵ�" << ": " << std::setw(6) << trainee.GetBaseSpeed() << "\n";
	std::cout << std::setw(12) << "���¹̳�" << ": " << std::setw(6) << trainee.GetMaxStamina() << "\n";
	std::cout << std::setw(12) << "����" << ": " << std::setw(6) << trainee.GetIntel() << "\n\n";
}

void TrainingManager::HandleSaveOrDiscard(Horse& trainee, std::vector<Horse>& PlayerList)
{
	while (true)
	{
		char yOrn;
		std::cout << "�츶�����޸� �����Ͻðڽ��ϱ�?? (Y / N) ";
		std::cin >> yOrn;

		if (yOrn == 'Y' || yOrn == 'y')
		{
			SaveHorse(trainee, PlayerList);
			break;
		}
		else if (yOrn == 'N' || yOrn == 'n')
		{
			std::cout << "�ȳ� " << trainee.GetName() << "....\n";
			break;
		}
		else
		{
			std::cout << "�߸��� �Է��Դϴ�.\n";
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
		std::cout << "1�� !! ��� ������ 3��ŭ ����ߴ�!! ��ų����Ʈ 50�� �����!!\n";
	}

	else if (&trainee == RM.GetRankedHorse(1))
	{
		StatChange(trainee, 2, SPEED);
		StatChange(trainee, 2, STAMINA);
		StatChange(trainee, 20, INTELLIGENCE);
		SetSP(GetSP() + 50);
		std::cout << "2�� !! ��� ������ 2��ŭ ����ߴ�!! ��ų����Ʈ 50�� �����!!\n";
	}

	else if (&trainee == RM.GetRankedHorse(2))
	{
		StatChange(trainee, 1, SPEED);
		StatChange(trainee, 1, STAMINA);
		StatChange(trainee, 10, INTELLIGENCE);
		SetSP(GetSP() + 50);
		std::cout << "3�� !! ��� ������ 1��ŭ ����ߴ�!! ��ų����Ʈ 50�� �����!!\n";
	}

	else
	{
		SetSP(GetSP() + 50);
		std::cout << "��ų����Ʈ 50�� �����!!\n";
	}

	Sleep(2000);
}

void TrainingManager::ShowSkillHintMenu(Horse& trainee)
{
	std::cout << "\n--- ��� �� �ִ� ��ų ��� ---\n";

	bool hasAvailable = false;

	for (Skill& skill : skills)
	{
		if (skill.GetHintLevel() > 0 && !skill.IsLearned())
		{
			hasAvailable = true;
			int cost = skill.GetEffectiveCost();

			std::cout << skill.GetName() << " (��Ʈ Lv: " << skill.GetHintLevel()
				<< ", �ʿ� SP: " << cost << ") -> ";

			if (m_SP >= cost)
				std::cout << "[��� �� ����]\n";
			else
				std::cout << "[SP ����]\n";
		}
	}

	if (!hasAvailable)
	{
		std::cout << "��� �� �ִ� ��ų�� �����ϴ�.\n\n";
		Sleep(1500);
		return;
	}

	std::cout << "\n��ų�� ���ðڽ��ϱ�? (Y/N): ";
	char choice;
	std::cin >> choice;
	if (choice != 'Y' && choice != 'y') return;

	for (Skill& skill : skills)
	{
		if (skill.GetHintLevel() > 0 && !skill.IsLearned())
		{
			int cost = skill.GetEffectiveCost();
			if (m_SP < cost) continue;

			std::cout << "\n'" << skill.GetName() << "' ��ų�� ���ðڽ��ϱ�? (Y/N): ";
			char learnChoice;
			std::cin >> learnChoice;

			if (learnChoice == 'Y' || learnChoice == 'y')
			{
				m_SP -= cost;
				skill.SetLearned(true);

				trainee.InputSkill(skill);
				std::cout << skill.GetName() << " ��ų�� ������ϴ�! (���� SP: " << m_SP << ")\n";
			}
		}
	}

	std::cout << "\n";
	Sleep(2000);
}