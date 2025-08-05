#pragma once
#include "Defines.h"

class Horse;

class Skill
{
private:
    // --- 속성 ---
    std::string m_name;
    int m_speed;
    float m_basechance;
    float m_realchance;
    float m_stamina;
    float m_duration;
    int m_cost;
    int m_hintlevel;
    bool m_learned;

    bool m_isActive;
    float m_timeLeft = 0.0f;         // 남은 지속 시간
    bool m_speedApplied = false;     // 속도 버프가 현재 적용되었는지 여부

public:
    // --- 생성자 ---
    Skill()
        : m_name(""), m_speed(0), m_basechance(0.0f), m_realchance(0.0f), m_stamina(0.0f), m_duration(0.0f),
        m_cost(0), m_hintlevel(0), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }
    Skill(const std::string& name)
        : m_name(name), m_speed(0), m_basechance(0.0f), m_realchance(0.0f), m_stamina(0.0f), m_duration(0.0f),
        m_cost(0), m_hintlevel(0), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }
    // 기본 스킬 초기화
    Skill(const std::string& name, int speed, float chance, float stamina, float duration)
        : m_name(name), m_speed(speed), m_basechance(chance), m_realchance(chance), m_stamina(stamina), m_duration(duration),
        m_cost(0), m_hintlevel(0), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }
    // 배울 수 있는 스킬들 초기화
    Skill(const std::string& name, int speed, float chance, float stamina, float duration, int cost, int hintlevel)
        : m_name(name), m_speed(speed), m_basechance(chance), m_realchance(chance), m_stamina(stamina), m_duration(duration),
        m_cost(cost), m_hintlevel(hintlevel), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }

    // --- 기능 메서드 ---
    void Activate();
    void Deactivate();
    void UpdateDuration(float deltaTime);
    bool ShouldActivate(float raceProgress, float currentHp, float maxHp, bool isLeading);
    void AddChance(Horse& horse);

    // --- Getter ---
    std::string GetName() const { return m_name; }
    int GetSpeed() const { return m_speed; }
    float GetChance() const { return m_basechance; }
    float GetStamina() const { return m_stamina; }
    float GetDuration() const { return m_duration; }
    bool IsActive() const { return m_isActive; }
    bool IsActivated() const { return IsActive(); } // 동일 의미

    // --- Setter ---
    void SetSpeedApplied(bool val) { m_speedApplied = val; }
    void SetName(std::string name) { m_name = name; }
    void SetSpeed(int speed) { m_speed = speed; }
    void SetBaseChance(float chance) { m_basechance = chance; }
    void SetStamina(float stam) { m_stamina = stam; }
    void SetDuration(float duration) { m_duration = duration; }

    // --- 상태 확인 ---
    bool IsSpeedApplied() const { return m_speedApplied; }

    int GetHintLevel() const { return m_hintlevel; }
    void IncreaseHintLevel() {
        if (m_hintlevel < 5) ++m_hintlevel;
    }

    void SetLearned(bool val) { m_learned = val; }
    bool IsLearned() const { return m_learned; }

    // SP 비용은 힌트레벨이 높을수록 감소
    int GetEffectiveCost() const 
    {
        // 힌트 레벨 1당 10% 감소 (최대 50%)
        return m_cost * (10 - m_hintlevel) / 10;
    }
};