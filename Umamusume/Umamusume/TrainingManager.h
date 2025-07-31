#pragma once
#include "Defines.h"
#include "Horse.h"

// ��� ���� (�ʿ��ϸ� constexpr ������ ���� ����)
#define DECREASE_HP_TRAINING 15
#define INCREASE_AMOUNT_SPEED 1
#define INCREASE_AMOUNT_STAMINA 1
#define INCREASE_AMOUNT_INTELLIGENCE 10
#define DECREASE_AMOUNT_SPEED -1
#define DECREASE_AMOUNT_STAMINA -1
#define DECREASE_AMOUNT_INTELLIGENCE -5
#define DECREASE_CRITICAL_CASE -5
#define INCREASE_AMOUNT_TRAINHP 50
#define TRAINGING_DAYS 11

enum TrainingType
{
    SPEED = 1,
    STAMINA,
    INTELLIGENCE,
    REST
};

class TrainingManager
{
private:
    // ���� ����
    int m_trainingStage = 1;
    int m_trainMaxHp = 100;
    int m_trainHp = m_trainMaxHp;

    // --- ���� ���� �Լ� ---
    int GetIncreaseAmount(TrainingType type) const;
    int GetDecreaseAmount(TrainingType type) const;

    void IncreaseCondition(Horse& trainee);
    void DecreaseCondition(Horse& trainee);

    void DecreaseTrainingHp();
    void RecoverTrainHp(int amount);

    void HandleTrainingFailure(Horse& trainee, TrainingType _type);
    void HandleRestDuringFailure(Horse& trainee, TrainingType _type);
    void HandleRetryTraining(Horse& trainee, TrainingType _type);
    void ApplyCriticalFailure(Horse& trainee);

    int GetTrainingSelection() const;

    void PrintTrainingMenu(const Horse& trainee);

    // --- ���� ��� ---
    void PrintTrainingResult(const Horse& trainee) const;
    void PrintStatus(const Horse& trainee) const;
    std::string PrintCondition(const Horse& trainee) const;

    // --- ����/���� ---
    void SaveHorse(const Horse& trainee, std::vector<Horse>& PlayerList);
    void HandleSaveOrDiscard(Horse& trainee, std::vector<Horse>& PlayerList);

public:
    // --- �ֿ� ���� ---
    Horse SelectTrainee();
    void InitTrainingManager();
    void TrainingLoop(Horse& trainee, std::vector<Horse>& PlayerList);
    void Training(Horse& trainee, TrainingType _type);
    void Rest(Horse& trainee);
    float TrainingProbability(int H, int MaxH) const;
    void StatChange(Horse& trainee, int statModifier, TrainingType _type);
};