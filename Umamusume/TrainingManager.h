#pragma once
#include "Defines.h"
#include "RaceManager.h"
#include "Horse.h"
#include "SkillDatabase.h"

#define DECREASE_HP_TRAINING 15
#define INCREASE_AMOUNT_SPEED 1
#define INCREASE_AMOUNT_STAMINA 1
#define INCREASE_AMOUNT_INTELLIGENCE 10
#define DECREASE_AMOUNT_SPEED -1
#define DECREASE_AMOUNT_STAMINA -1
#define DECREASE_AMOUNT_INTELLIGENCE -5
#define DECREASE_CRITICAL_CASE -5
#define INCREASE_AMOUNT_TRAINHP 50
#define TRAINGING_DAYS 10

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
    // 상태 변수
    int m_trainingStage = 1;
    int m_trainMaxHp = 100;
    int m_trainHp = m_trainMaxHp;
    int m_SP = 0;

    // --- 내부 헬퍼 함수 ---
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
    void StatIncreaseAfterRace(Horse& trainee, RaceManager& RM);
    void ShowSkillHintMenu(Horse& trainee);

    int GetTrainingSelection() const;

    void PrintTrainingMenu(const Horse& trainee);

    // --- 상태 출력 ---
    void PrintTrainingResult(const Horse& trainee) const;
    void PrintStatus(const Horse& trainee) const;
    std::string PrintCondition(const Horse& trainee) const;

    // --- 저장/관리 ---
    void SaveHorse(const Horse& trainee, std::vector<Horse>& PlayerList);
    void HandleSaveOrDiscard(Horse& trainee, std::vector<Horse>& PlayerList);

public:
    // --- 주요 동작 ---
    Horse SelectTrainee();
    void InitTrainingManager();
    void TrainingLoop(Horse& trainee, std::vector<Horse>& PlayerList,
        RaceManager& RM, DoubleBuffering& DB, Tile(*_BG)[DF_BG_SIZE_X],
        Horse* AI_horse[]);
    void Training(Horse& trainee, TrainingType _type);
    void Rest(Horse& trainee);
    float TrainingProbability(int H, int MaxH) const;
    void StatChange(Horse& trainee, int statModifier, TrainingType _type);

    // --- Set 함수 ---
    void SetSP(int _SP) { m_SP = _SP; }

    // --- Get 함수 ---
    int GetSP() const { return m_SP; }
};