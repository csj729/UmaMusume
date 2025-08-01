#pragma once
#include "Defines.h"
#include "Tile.h"
#include "Skill.h"

const std::string HorseName[HORSETABLE_NUM] =
{
    "메지로 맥퀸",
    "골드 쉽",
    "오구리 캡",
    "마루젠스키"
};

enum HorseVitality
{
    EXHAUSTED,
    TIRED,
    NORMAL,
    ENERGETIC
};
    
enum HorseType
{
    PACESETTER, // 도주
    LEADER,     // 선행
    STALKER,    // 선입
    CLOSER,      // 추입
    NONE
};

enum HorseCondition
{
    VERYPOOR,
    POOR,
    FEELSOSO,
    GOOD,
    EXCELLENT
};

struct HorseData
{
    int baseSpeed;
    float maxStamina;
    HorseType type;
    std::vector<Skill> skills;

    HorseData(int spd, float sta, HorseType t, std::initializer_list<Skill> skillList)
        : baseSpeed(spd), maxStamina(sta), type(t), skills(skillList) {
    }
};

const std::map<std::string, HorseData> HorseDataTable = 
{
    {   HorseName[0],
        {5, 30.0f, LEADER, 
            {
                Skill("존귀한 사명을 완수하기 위하여", 15, 1.0f, 0.0f, 10.0f),
                Skill("스태미너 킵", 1, 0.8f, 3.0f, 1.0f),
                Skill("속도 유지", 2, 0.6f, 0.0f, 4.0f)
            }
        }
    },

    {    HorseName[1],
        {5, 30.0f, CLOSER,
            {
                Skill("불침함, 출항!!", 15, 0.5f, 0.0f, 5.0f),
                Skill("추격", 4, 0.5f, 0.0f, 5.0f),
                Skill("후방 대기", 1, 0.8f, 1.0f, 5.0f)
            }
        }
    },
    { HorseName[2], {5, 30.0f, STALKER, {
        Skill("승리의 고동", 15, 0.5f, 0.0f, 5.0f),
        Skill("속도 가속", 2, 0.8f, 0.0f, 5.0f),
        Skill("영양 보급", 0, 0.5f, 2.0f, 1.0f)
    }}},
    { HorseName[3], {5, 30.0f, PACESETTER, {
        Skill("홍염 기어/LP1211-M", 13, 0.5f, 0.0f, 6.0f),
        Skill("앞장서기", 2, 0.6f, 0.0f, 5.0f),
        Skill("기어 시프트", 1, 0.8f, 0.0f, 5.0f)
    }}}
};

class Horse
{
private:
    // --- 멤버 변수 ---
    std::string m_name;
    short m_baseSpeed;
    short m_realSpeed;
    float m_MaxStamina;
    float m_stamina;
    int m_intelligence;
    COORD m_Position;
    std::vector<Skill> m_skillList;
    bool isFinish;
    bool isRanked = false;
    int m_lane;
    HorseVitality m_vitStatus;
    HorseType m_type;
    float m_finishTime = 0.0f;
    HorseCondition m_condition;

public:
    // --- 생성자 ---
    Horse()
        : m_name(""),
        m_baseSpeed(0),
        m_realSpeed(0),
        m_MaxStamina(0.0f),
        m_stamina(0.0f),
        m_intelligence(0),
        m_Position({ 0, 0 }),
        isFinish(false),
        isRanked(false),
        m_lane(0),
        m_vitStatus(HorseVitality::ENERGETIC),
        m_type(HorseType::PACESETTER),
        m_condition(HorseCondition::FEELSOSO)
    {
    }

    // --- 기능 함수 ---
    void HorseRender(Tile(*BG)[DF_BG_SIZE_X], int scrollX);
    void InitHorse();
    void InitByName(const std::string& name);
    std::string SelectName(const std::string horseName[]);
    void HorseTick(int leader_X, float deltaTime);
    void CheckFinish();

    // --- 설정자 (Setter) ---
    void SetName(const std::string Newname) { m_name = Newname; }
    void SetBaseSpeed(const unsigned int Newspeed) { m_baseSpeed = Newspeed; }
    void SetRealSpeed(const unsigned int Newspeed) { m_realSpeed = Newspeed; }
    void SetMaxStamina(const float NewMaxHp) { m_MaxStamina = NewMaxHp; }
    void SetStamina(const float NewHp) { m_stamina = NewHp; }
    void SetIntel(const int NewIntel) { m_intelligence = NewIntel; }
    void SetPos(SHORT x, SHORT y)
    {
        m_Position.X = x;
        m_Position.Y = y;
    }
    void SetSkill(int index, const Skill& skill)
    {
        if (index >= 0 && index < SKILL_NUM)
            m_skillList[index] = skill;
    }
    void SetSkills(const std::vector<Skill>& newSkills)
    {
        m_skillList = newSkills; // 벡터 대입으로 복사
    }
    void SetHorseType(HorseType type) { m_type = type; };
    void SetLane(const int Newlane) { m_lane = Newlane; }
    void SetRanked(bool val) { isRanked = val; }
    void SetFinishTime(float time) { m_finishTime = time; }
    void SetHorseCondition(HorseCondition Newcondition) { m_condition = Newcondition; }
    void SetFinish(bool val) { isFinish = val; }

    // --- 접근자 (Getter) ---
    const Skill* GetSkill(int index) const;
    const Skill* GetActiveSkill() const;
    std::string GetName() const { return m_name; }
    short GetBaseSpeed() const { return m_baseSpeed; }
    short GetRealSpeed() const { return m_realSpeed; }
    float GetMaxStamina() const { return m_MaxStamina; }
    float GetStamina() const { return m_stamina; }
    COORD GetPos() const { return m_Position; }
    bool IsFinish() const { return isFinish; }
    bool IsRanked() const { return isRanked; }
    int GetLane() const { return m_lane; }
    int GetIntel() const { return m_intelligence; }
    float GetFinishTime() const { return m_finishTime; }
    HorseCondition GetHorseCondition() const { return m_condition; }
    HorseType GetHorseType() const { return m_type; }
    std::string GetHorseTypeName() const;
};