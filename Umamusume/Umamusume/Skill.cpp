#include "Skill.h"

void Skill::Activate()
{
    m_isActive = true;
    m_timeLeft = m_duration;
    m_speedApplied = false;
}

void Skill::Deactivate()
{
    m_isActive = false;
    m_timeLeft = 0;
    m_speedApplied = false;
}

void Skill::UpdateDuration(float deltaTime)
{
    if (m_isActive)
    {
        m_timeLeft -= deltaTime;
        if (m_timeLeft <= 0)
        {
            Deactivate();
        }
    }
}

bool Skill::ShouldActivate(float raceProgress, float currentHp, float maxHp, bool isLeading)
{
    float adjustedChance = m_chance;

    if (m_name == "������ ����� �ϼ��ϱ� ���Ͽ�")
    {
        if (raceProgress >= 0.66f && currentHp > (maxHp * 2.0f / 3.0f))
            adjustedChance += 0.8f;
        else if (raceProgress >= 0.66f && currentHp > (maxHp / 3.0f))
        {
            adjustedChance += 0.8f;
            m_speed /= 2;
        }
        else
            return false;
    }
    else if (m_name == "��ħ��, ����!!")
    {
        if (raceProgress >= 0.4f && raceProgress <= 0.7f && !isLeading)
            adjustedChance += 1.0f;
        else
            return false;
    }
    else if (m_name == "�¸��� ��")
    {
        if (raceProgress >= 0.6f)
            adjustedChance += 0.5f;
        else
            return false;
    }
    else if (m_name == "ȫ�� ���/LP1211-M")
    {
        if (isLeading && raceProgress >= 0.66f)
            adjustedChance += 0.5f;
        else
            return false;
    }
    else
    {
        // ������ ���� �Ϲ� ��ų �� �⺻ Ȯ���� �ߵ� �õ�
        adjustedChance = m_chance;
    }
    // Ȯ�� ���� ����
    if (adjustedChance > 1.0f) adjustedChance = 1.0f;
    if (adjustedChance < 0.05f) adjustedChance = 0.05f;

    float randVal = static_cast<float>(rand()) / RAND_MAX * 100;

    return randVal < adjustedChance;
}