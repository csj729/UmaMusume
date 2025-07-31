#pragma once
#include "Defines.h"

class Skill
{
private:
    // --- �Ӽ� ---
    std::string m_name;
    int m_speed;
    float m_chance;
    float m_stamina;
    float m_duration;

    bool m_isActive;
    float m_timeLeft = 0.0f;         // ���� ���� �ð�
    bool m_speedApplied = false;     // �ӵ� ������ ���� ����Ǿ����� ����

public:
    // --- ������ ---
    Skill()
        : m_name(""), m_speed(0), m_chance(0.0f), m_stamina(0.0f), m_duration(0.0f),
        m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }

    Skill(const std::string& name, int speed, float chance, float stamina, float duration)
        : m_name(name), m_speed(speed), m_chance(chance), m_stamina(stamina), m_duration(duration),
        m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }

    // --- ��� �޼��� ---
    void Activate();
    void Deactivate();
    void UpdateDuration(float deltaTime);
    bool ShouldActivate(float raceProgress, float currentHp, float maxHp, bool isLeading);

    // --- Getter ---
    std::string GetName() const { return m_name; }
    int GetSpeed() const { return m_speed; }
    float GetChance() const { return m_chance; }
    float GetStamina() const { return m_stamina; }
    float GetDuration() const { return m_duration; }
    bool IsActive() const { return m_isActive; }
    bool IsActivated() const { return IsActive(); } // ���� �ǹ�

    // --- Setter ---
    void SetSpeedApplied(bool val) { m_speedApplied = val; }
    void SetName(std::string name) { m_name = name; }

    // --- ���� Ȯ�� ---
    bool IsSpeedApplied() const { return m_speedApplied; }
};