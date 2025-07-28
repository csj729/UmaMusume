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
	float m_trainMaxHp = 100.0f;
	float m_trainHp = m_trainMaxHp;


public:
	void TrainingLoop(Horse& trainee);
	void Training(Horse& trainee, TrainingType _type);
	float TrainingProbability(float H, float MaxH);

};

