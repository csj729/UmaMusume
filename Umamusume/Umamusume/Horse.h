#pragma once
#include "Defines.h"
#include "Tile.h"
#include "Skill.h"


const std::string HorseName[HORSE_NUM] =
{
	HORSE1_NAME,
	HORSE2_NAME,
	HORSE3_NAME
};

enum HorseVitality
{
	EXHAUSTED,   
	TIRED,
	NORMAL,
	ENERGETIC
};

class Horse
{
private:
	std::string m_name;
	short m_baseSpeed;
	short m_realSpeed;
	float m_Maxhp;
	float m_hp;
	COORD m_Position; //À§Ä¡
	Skill m_skillList[SKILL_NUM];
	bool isFinish;
	int m_lane;
	HorseVitality m_vitStatus;

public:
	Horse() : m_name(""), m_baseSpeed(0), m_realSpeed(0), m_Maxhp(0.0f), m_hp(0.0f), m_Position({ 0, 0 }), isFinish(false), m_lane(0), m_vitStatus(HorseVitality::ENERGETIC) {}
	void HorseRender(Tile(*BG)[DF_BG_SIZE_X], int scrollX);
	void InitHorse();
	std::string SelectName(const std::string horseName[]);
	void HorseTick();

	void SetName(const std::string Newname) { m_name = Newname; };
	void SetBaseSpeed(const unsigned int Newspeed) { m_baseSpeed = Newspeed; };
	void SetRealSpeed(const unsigned int Newspeed) { m_realSpeed = Newspeed; };
	void SetMaxHp(const float NewMaxHp) { m_Maxhp = NewMaxHp; };
	void SetHp(const float NewHp) { m_hp = NewHp; };
	void SetPos(SHORT x, SHORT y)
	{
		m_Position.X = x; m_Position.Y = y;
	};
	void SetLane(const int Newlane) { m_lane = Newlane; };

	std::string GetName() const { return m_name; };
	short GetBaseSpeed() const { return m_baseSpeed; };
	short GetRealSpeed() const { return m_realSpeed; };
	float GetMaxHp() const { return m_Maxhp; };
	float GetHp() const { return m_hp; };
	COORD GetPos() const { return m_Position; };
	bool IsFinish();
	int GetLane() const { return m_lane; };

};

