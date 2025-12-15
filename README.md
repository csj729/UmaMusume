# 🏇 UmaMusume (C++ Console Project)

## 📋 목차

- [게임 소개](#-게임-소개)
- [주요 스크립트](#-주요-스크립트)
  - [말 객체](#-말-객체)
  - [레이스 관리](#-레이스-관리)
  - [육성 시뮬레이션](#️-육성-시뮬레이션)
  - [스킬 시스템](#-스킬-시스템)
- [기술 스택](#️-기술-스택)
- [개발자](#-개발자)

---

## 🎮 게임 소개

**C++와 Windows Console API**를 사용하여 제작한 우마무스메 프리티 더비 모작 프로젝트입니다.  
객체 지향 설계(OOP)를 바탕으로 캐릭터 **육성(Training)** 시뮬레이션과 박진감 넘치는 **레이스(Race)** 시스템을 콘솔 환경에서 구현하였습니다.

---

## 📜 주요 스크립트

게임의 핵심 로직을 담당하는 주요 클래스와 대표적인 구현 코드를 소개합니다.

### 🐴 말 객체
* **역할:** 말의 기본 스탯(속도, 스태미나, 지능), 현재 위치, 상태(컨디션)를 관리합니다.
* **핵심 기능:** 매 프레임마다 스태미나 잔량과 스킬 발동 여부에 따라 **실시간 속도(RealSpeed)**를 계산하고 이동합니다.

<details>
<summary><b>[코드 보기] Horse::HorseTick (이동 및 스태미나 로직)</b></summary>

```cpp
void Horse::HorseTick(int leader_X, float deltaTime, const Map& map)
{
    if (isFinish) return;

    // 1. 스태미나 감소 및 상태 변화 (탈진 시스템)
    // 각질(도주, 선행 등)과 선두와의 거리 등에 따라 스태미나 소모량이 달라짐
    float staminaDrain = 0.0f;
    int speedModType = 0, speedModStamina = 0;
    int distanceFromLeader = leader_X - m_Position.X;

    switch (m_type)
    {
    case PACESETTER: speedModType = 2; staminaDrain = 0.1f; break;
    case LEADER:     speedModType = 1; staminaDrain = 0.08f; break;
    // ... (중략) ...
    }

    m_stamina = max(0.0f, m_stamina - staminaDrain);

    // 2. 남은 스태미나 비율에 따른 컨디션(속도 보정) 결정
    float staminaRate = m_stamina / m_MaxStamina;
    if (staminaRate > 0.75f) m_vitStatus = ENERGETIC;
    else if (staminaRate > 0.5f) m_vitStatus = NORMAL;
    else if (staminaRate > 0.25f) m_vitStatus = TIRED;
    else m_vitStatus = EXHAUSTED;

    // 3. 최종 속도 계산 (기본 속도 + 각질 보정 + 스태미나 보정)
    switch (m_vitStatus)
    {
    case ENERGETIC: speedModStamina = 2; break;
    case TIRED: speedModStamina = -1; break;
    case EXHAUSTED: speedModStamina = -3; break;
    }
    m_realSpeed = max(1, m_baseSpeed + speedModType + speedModStamina);

    // 4. 스킬 처리 및 위치 업데이트
    int totalSpeedBuff = 0;
    for (int i = 0; i < m_skillList.size(); ++i) 
    {
        Skill& skill = m_skillList[i];
        skill.AddChance(*this);
        if (skill.IsActive()) 
        {
            skill.UpdateDuration(deltaTime);
            if (!skill.IsActive()) skill.Deactivate();
            totalSpeedBuff += skill.GetSpeed();
        }
        else if (skill.ShouldActivate(raceProgress, m_stamina, m_MaxStamina, isLeading)) 
        {
            skill.Activate();
            // ... (생략) ...
        }
    }

    m_realSpeed += totalSpeedBuff;
    m_Position.X += m_realSpeed;
}
```
</details>

---

### 🏁 레이스 관리
* **역할:** 레이스의 시작부터 종료까지의 흐름을 제어합니다.
* **핵심 기능:** 다수의 Horse 객체를 업데이트하고, 실시간 순위를 계산하며, 골인(Finish) 여부를 판정합니다.

<details> <summary><b>[코드 보기] RaceManager::RaceTick (레이스 루프)</b></summary>

```cpp
void RaceManager::RaceTick(Horse* horses[])
{
    clock_t now = clock();
    float deltaTime = static_cast<float>(now - prevClock) / CLOCKS_PER_SEC;
    prevClock = now;

    if (isRaceFinished) return;

    int leaderX = (leader != nullptr) ? leader->GetPos().X : 0;

    // 1. 모든 참가 말 업데이트
    for (int i = 0; i < HORSE_NUM; ++i)
        horses[i]->HorseTick(leaderX, deltaTime, *pMap);

    // 2. 실시간 선두 갱신 및 결승선 통과 체크
    Horse* newLeader = nullptr;
    for (int i = 0; i < HORSE_NUM; ++i)
    {
        if (!horses[i]->IsFinish())
        {
            if (newLeader == nullptr || horses[i]->GetPos().X > newLeader->GetPos().X)
                newLeader = horses[i];
        }
    }

    if (newLeader != nullptr)
    {
        leader = newLeader;
        leader->CheckFinish(*pMap);

        // 리더 완주 시 순위 등록
        if (leader->IsFinish() && !leader->IsRanked())
        {
            if (rankIndex < HORSE_NUM)
            {
                float elapsedTime = static_cast<float>(now - startClock) / CLOCKS_PER_SEC;
                leader->SetFinishTime(elapsedTime);
                leader->SetRanked(true);
                rank[rankIndex++] = leader;
            }
        }
        
        // 3. 카메라 스크롤 조정 (선두 기준)
        pMap->SetScrollX(leader->GetPos().X - DF_BG_SIZE_X * 2 / 3);
        // ... (경계 처리) ...
    }

    // 4. 경기 종료 판정
    if (rankIndex == HORSE_NUM)
        isRaceFinished = true;
}
```
</details>

---

### 🏋️ 육성 시뮬레이션
* **역할:** 플레이어가 선택한 훈련(스피드, 스태미나 등)을 수행하고 스탯을 성장시킵니다.
* **핵심 기능:** 체력(HP)에 따른 **실패 확률(RNG)**을 계산하고, 성공/실패/대성공(Critical) 분기를 처리합니다.

<details> <summary><b>[코드 보기] TrainingManager::Training (훈련 로직)</b></summary>

```cpp
void TrainingManager::Training(Horse& trainee, TrainingType _type)
{
    DecreaseTrainingHp(); // 훈련 시 체력 소모

    int increaseStat = 0;
    float randFloat = static_cast<float>(rand()) / RAND_MAX;

    // 1. 훈련 성공 여부 판정 (체력이 낮을수록 실패 확률 증가)
    if (TrainingProbability(m_trainHp, m_trainMaxHp) >= randFloat)
    {
        // 2. 스킬 힌트 이벤트 (40% 확률)
        randFloat = static_cast<float>(rand()) / RAND_MAX;
        if (0.4f > randFloat)
        {
            // ... 무작위 스킬 힌트 레벨 상승 로직 ...
        }

        // 3. 대성공(20%) vs 성공
        randFloat = static_cast<float>(rand()) / RAND_MAX;
        if (randFloat < 0.2f) // 대성공
        {
            increaseStat = GetIncreaseAmount(_type) * 2;
            IncreaseCondition(trainee); // 컨디션 상승
            StatChange(trainee, increaseStat, _type);
            std::cout << "훈련 대성공!! " << increaseStat << "만큼 스탯이 상승했습니다!!\n";
        }
        else // 일반 성공
        {
            increaseStat = GetIncreaseAmount(_type);
            StatChange(trainee, increaseStat, _type);
            std::cout << "훈련 성공!! " << increaseStat << "만큼 상승했습니다!\n";
        }
    }
    else // 실패
    {
        HandleTrainingFailure(trainee, _type);
    }
}
```
</details>

---

### ✨ 스킬 시스템
* **역할:** 레이스 도중 특정 조건(구간, 순위, 확률)을 만족하면 발동하는 특수 능력을 정의합니다.
* **핵심 기능:** ShouldActivate 함수를 통해 스킬 발동 조건을 검사합니다.

<details> <summary><b>[코드 보기] Skill::ShouldActivate (발동 조건 검사)</b></summary>

```cpp
bool Skill::ShouldActivate(float raceProgress, float currentHp, float maxHp, bool isLeading)
{
    float adjustedChance = m_realchance;

    // 스킬별 고유 발동 조건 검사
    if (m_name == "존귀한 사명을 완수하기 위하여")
    {
        // 레이스 후반부(66% 이상)이고 체력이 1/3 이상 남았을 때 발동 확률 증가
        if (raceProgress >= 0.66f && currentHp > (maxHp / 3.0f))
            adjustedChance += 2.0f;
        else
            return false;
    }
    else if (m_name == "불침함, 출항!!")
    {
        // 레이스 중반(40~70%)에 선두가 아닐 경우 발동
        if (raceProgress >= 0.4f && raceProgress <= 0.7f && !isLeading)
            adjustedChance += 2.0f;
        else
            return false;
    }
    // ... (기타 스킬 조건) ...
    else
    {
        // 조건이 없는 일반 스킬은 기본 확률 사용
        adjustedChance = m_realchance;
    }

    // 최종 확률 체크 (RNG)
    float randVal = static_cast<float>(rand()) / RAND_MAX * 100;
    return randVal < adjustedChance;
}
```
</details>

---

### 🛠️ 기술 스택
* **Language:** C++
* **Platform:** Windows Console (Win32 API for cursor/screen handling)
* **Pattern:** OOP (Object-Oriented Programming), Manager Pattern
* **Rendering:*** Double Buffering (Flicker-free console rendering)

---

## 👨‍💻 개발자

<br>

**천성준**

<br>
<br>

[![GitHub](https://img.shields.io/badge/GitHub-csj729-181717?style=for-the-badge&logo=github)](https://github.com/csj729/UmaMusume)

<br>


<b>Github</b> <a href="https://www.google.com/search?q=https://github.com/csj729">csj729</a> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <b>E-mail</b> <a href="mailto:csj729@gmail.com">csj729@gmail.com</a> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <b>Phone</b> 010-2997-7295 </div>
