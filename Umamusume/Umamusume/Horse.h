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
	LEADER,		// 선행
	STALKER,	// 선입
	CLOSER		// 추입
};

class Horse	
{
private:
	std::string m_name;
	short m_baseSpeed;
	short m_realSpeed;
	float m_MaxStamina;
	float m_stamina;
	int m_intelligence;
	COORD m_Position; //위치
	Skill m_skillList[SKILL_NUM];
	bool isFinish;
	bool isRanked = false;
	int m_lane;
	HorseVitality m_vitStatus;
	HorseType m_type;
	float m_finishTime = 0.0f;

public:
	Horse() : m_name(""), m_baseSpeed(0), m_realSpeed(0), m_MaxStamina(0.0f), m_stamina(0.0f), m_intelligence(0), m_Position({ 0, 0 }), isFinish(false), m_lane(0), m_vitStatus(HorseVitality::ENERGETIC), m_type(HorseType::PACESETTER) {}
	void HorseRender(Tile(*BG)[DF_BG_SIZE_X], int scrollX);
	void InitHorse();
	std::string SelectName(const std::string horseName[]);
	void HorseTick(int leader_X, float deltaTime);


	void SetName(const std::string Newname) { m_name = Newname; };
	void SetBaseSpeed(const unsigned int Newspeed) { m_baseSpeed = Newspeed; };
	void SetRealSpeed(const unsigned int Newspeed) { m_realSpeed = Newspeed; };
	void SetMaxHp(const float NewMaxHp) { m_MaxStamina = NewMaxHp; };
	void SetHp(const float NewHp) { m_stamina = NewHp; };
	void SetPos(SHORT x, SHORT y)
	{
		m_Position.X = x; m_Position.Y = y;
	};
	void SetLane(const int Newlane) { m_lane = Newlane; };
	void SetRanked(bool val) { isRanked = val; };
	void SetFinishTime(float time) { m_finishTime = time; }
	void CheckFinish();
	const Skill* GetActiveSkill() const;
	std::string GetName() const { return m_name; };
	short GetBaseSpeed() const { return m_baseSpeed; };
	short GetRealSpeed() const { return m_realSpeed; };
	float GetMaxHp() const { return m_MaxStamina; };
	float GetHp() const { return m_stamina; };
	COORD GetPos() const { return m_Position; };
	bool IsFinish() const { return isFinish; };
	bool IsRanked() const { return isRanked; };
	int GetLane() const { return m_lane; };
	float GetFinishTime() const { return m_finishTime; }

};

