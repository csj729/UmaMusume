#pragma once
#include "Defines.h"

class Skill
{
private:
    std::string m_name;
    int m_speed;
    float m_chance;
    float m_stamina;
    float m_duration;
    bool m_isActive;
    float m_timeLeft = 0.0f;   // 남은 지속 시간
    bool m_speedApplied = false; // 속도 버프가 현재 적용됐는지 여부

public:
    Skill() : m_name(""), m_speed(0), m_chance(0.0f), m_stamina(0.0f), m_duration(0.0f), m_isActive(false) {}
    Skill(const std::string& name, int speed, float chance, float hp, float duration)
        : m_name(name), m_speed(speed), m_chance(chance), m_stamina(hp), m_duration(duration),
        m_isActive(false), m_timeLeft(0), m_speedApplied(false) {
    }

    void Activate();
    void Deactivate();
    void UpdateDuration(float deltaTime);

    bool IsActive() const { return m_isActive; }
    int GetSpeed() const { return m_speed; }
    float GetChance() const { return m_chance; }
    float GetHp() const { return m_stamina; }
    std::string GetName() const { return m_name; }
    bool IsActivated() const { return IsActive(); }
    bool IsSpeedApplied() const { return m_speedApplied; }
    void SetSpeedApplied(bool val) { m_speedApplied = val; }
    bool ShouldActivate(float raceProgress, float currentHp, float maxHp, bool isLeading);
};