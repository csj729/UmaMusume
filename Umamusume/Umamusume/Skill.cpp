#include "Skill.h"
#include "Horse.h"

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
    float adjustedChance = m_realchance;

    if (m_name == "존귀한 사명을 완수하기 위하여")
    {
        if (raceProgress >= 0.66f && currentHp > (maxHp / 3.0f))
            adjustedChance += 2.0f;
        else
            return false;
    }
    else if (m_name == "불침함, 출항!!")
    {
        if (raceProgress >= 0.4f && raceProgress <= 0.7f && !isLeading)
            adjustedChance += 2.0f;
        else
            return false;
    }
    else if (m_name == "승리의 고동")
    {
        if (raceProgress >= 0.66f)
            adjustedChance += 2.0f;
        else
            return false;
    }
    else if (m_name == "홍염 기어/LP1211-M")
    {
        if (isLeading && raceProgress >= 0.66f)
            adjustedChance += 2.0f;
        else
            return false;
    }
    else
    {
        // 조건이 없는 일반 스킬 → 기본 확률로 발동 시도
        adjustedChance = m_realchance;
    }

    float randVal = static_cast<float>(rand()) / RAND_MAX * 100;

    return randVal < adjustedChance;
}

void Skill::AddChance(Horse& horse)
{
    m_realchance = m_basechance + (horse.GetHorseCondition() * m_basechance / 10) + (horse.GetIntel() / 200.f);
}
