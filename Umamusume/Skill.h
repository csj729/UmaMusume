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
    int m_cost;
    int m_hintlevel;
    bool m_learned;

    bool m_isActive;
    float m_timeLeft = 0.0f;         // ���� ���� �ð�
    bool m_speedApplied = false;     // �ӵ� ������ ���� ����Ǿ����� ����

public:
    // --- ������ ---
    Skill()
        : m_name(""), m_speed(0), m_chance(0.0f), m_stamina(0.0f), m_duration(0.0f),
        m_cost(0), m_hintlevel(0), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }
    Skill(const std::string& name)
        : m_name(name), m_speed(0), m_chance(0.0f), m_stamina(0.0f), m_duration(0.0f),
        m_cost(0), m_hintlevel(0), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }
    // �⺻ ��ų �ʱ�ȭ
    Skill(const std::string& name, int speed, float chance, float stamina, float duration)
        : m_name(name), m_speed(speed), m_chance(chance), m_stamina(stamina), m_duration(duration),
        m_cost(0), m_hintlevel(0), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
    }
    // ��� �� �ִ� ��ų�� �ʱ�ȭ
    Skill(const std::string& name, int speed, float chance, float stamina, float duration, int cost, int hintlevel)
        : m_name(name), m_speed(speed), m_chance(chance), m_stamina(stamina), m_duration(duration),
        m_cost(cost), m_hintlevel(hintlevel), m_learned(false), m_isActive(false), m_timeLeft(0.0f), m_speedApplied(false) {
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

    int GetHintLevel() const { return m_hintlevel; }
    void IncreaseHintLevel() {
        if (m_hintlevel < 5) ++m_hintlevel;
    }

    void SetLearned(bool val) { m_learned = val; }
    bool IsLearned() const { return m_learned; }

    // SP ����� ��Ʈ������ �������� ����
    int GetEffectiveCost() const 
    {
        // ��Ʈ ���� 1�� 10% ���� (�ִ� 50%)
        return m_cost * (10 - m_hintlevel) / 10;
    }
};