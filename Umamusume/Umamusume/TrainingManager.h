#pragma once
#include "Defines.h"
#include "Horse.h"

enum TrainingType
{
	SPEED = 1,
	STAMINA,
	INTELLIGENCE
};

class TrainingManager
{
private:
	int m_trainingStage = 1;
	int m_trainMaxHp = 100;
	int m_trainHp = m_trainMaxHp;


public:
	void TrainingLoop(Horse& trainee);
	void Training(Horse& trainee, TrainingType _type);
	float TrainingProbability(int H, int MaxH);
	void StatChange(Horse& trainee, int statModifier, TrainingType _type);

};

