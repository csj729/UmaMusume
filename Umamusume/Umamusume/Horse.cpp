#include "Horse.h"

void Horse::HorseRender(Tile(*BG)[DF_BG_SIZE_X], int scrollX)
{
	if (IsFinish()) return;

	int screenX = m_Position.X - scrollX;
	int y = m_Position.Y;

	// 4줄 높이, 9칸 너비: 화면 경계 검사
	if (screenX < 0 || screenX + 8 >= DF_BG_SIZE_X || y - 2 < 0 || y + 1 >= DF_BG_SIZE_Y)
		return;

	// 프레임 변수 (프레임 애니메이션용)
	static int frame = 0;
	frame = (frame + 1) % 2;

	// 프레임별 사람 모습 (4줄 × 9글자)
	const char* frame0[4] = 
	{
		"  __ O  /",
		" /  / -- ",
		"   ====  ",
		"__/   /  "
	};

	const char* frame1[4] = 
	{
		"  __  O /",
		" /   /-= ",
		"    ==== ",
		" __/   / "
	};

	const char** currFrame = (frame == 0) ? frame0 : frame1;

	// 출력 (4줄, 각 줄에 최대 9글자)
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			char ch = currFrame[i][j];
			if (ch != '\0')  // 널 문자 방지
				BG[y - 2 + i][screenX + j].Text = ch;
		}
	}
}

void Horse::InitHorse()
{
	isFinish = false;
	isRanked = false;

	// HorseDataTable에서 m_name에 맞는 데이터를 찾아 초기화
	auto it = HorseDataTable.find(m_name);
	if (it != HorseDataTable.end())
	{
		const HorseData& data = it->second;

		m_baseSpeed = data.baseSpeed;
		m_MaxStamina = data.maxStamina;
		m_stamina = m_MaxStamina;
		m_type = data.type;

		// m_skillList가 배열이라면, 벡터 크기만큼 복사 (최대 SKILL_NUM까지)
		for (size_t i = 0; i < data.skills.size() && i < SKILL_NUM; ++i)
			m_skillList.push_back(data.skills[i]);
	}
	else
	{
		// 이름이 데이터 테이블에 없으면 기본값 설정 (필요하면)
		m_baseSpeed = 0;
		m_MaxStamina = 0.0f;
		m_stamina = 0.0f;
		m_type = NONE; // 예: 기본값
		// 스킬 초기화도 필요하면 여기서
	}

}

std::string Horse::SelectName(const std::string horseName[])
{
	return m_name = horseName[rand() % HORSETABLE_NUM];
}

void Horse::HorseTick(int leader_X, float deltaTime, const Map& map)
{
	if (isFinish) return;

	float staminaDrain = 0.0f;
	int speedModType = 0, speedModStamina = 0;
	int distanceFromLeader = leader_X - m_Position.X;

	switch (m_type)
	{
	case PACESETTER: speedModType = 2; staminaDrain = 0.1f; break;
	case LEADER:     speedModType = 1; staminaDrain = 0.08f; break;
	case STALKER:
		speedModType = (distanceFromLeader >= 200) ? 1 : 0;
		staminaDrain = (distanceFromLeader >= 200) ? 0.08f : 0.04f;
		break;
	case CLOSER:
		speedModType = (leader_X >= map.GetFinishLine() * 2 / 3) ? 4 : -1;
		staminaDrain = (leader_X >= map.GetFinishLine() * 2 / 3) ? 0.15f : 0.02f;
		break;
	}

	m_stamina = max(0.0f, m_stamina - staminaDrain);

	float staminaRate = m_stamina / m_MaxStamina;
	if (staminaRate > 0.75f) m_vitStatus = ENERGETIC;
	else if (staminaRate > 0.5f) m_vitStatus = NORMAL;
	else if (staminaRate > 0.25f) m_vitStatus = TIRED;
	else m_vitStatus = EXHAUSTED;

	switch (m_vitStatus)
	{
	case ENERGETIC: speedModStamina = 2; break;
	case TIRED: speedModStamina = -1; break;
	case EXHAUSTED: speedModStamina = -3; break;
	default: break;
	}

	m_realSpeed = max(1, m_baseSpeed + speedModType + speedModStamina);
	bool isLeading = m_Position.X >= leader_X;
	float raceProgress = static_cast<float>(m_Position.X) / map.GetFinishLine();

	int totalSpeedBuff = 0;
	bool hpRecovered = false;

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
			if (!hpRecovered) 
			{
				m_stamina = min(m_MaxStamina, m_stamina + skill.GetStamina());
				hpRecovered = true;
			}
		}
	}

	m_realSpeed += totalSpeedBuff;
	m_Position.X += m_realSpeed;
}

void Horse::CheckFinish(const Map& map) 
{
	if (m_Position.X >= map.GetFinishLine()) isFinish = true;
}

const Skill* Horse::GetSkill(int index) const
{
	return (index >= 0 && index < m_skillList.size()) ? &m_skillList[index] : nullptr;
}

const Skill* Horse::GetActiveSkill() const 
{
	for (int i = 0; i < m_skillList.size(); ++i)
		if (m_skillList[i].IsActivated())
			return &m_skillList[i];
	return nullptr;
}

std::string Horse::GetHorseTypeName() const
{
	switch (m_type)
	{
	case LEADER: return "선행";
	case CLOSER: return "추입";
	case STALKER: return "선입";
	case PACESETTER: return "도주";
	default: return "???";
	}
}

void Horse::InitByName(const std::string& name)
{
	this->SetName(name);

	auto it = HorseDataTable.find(name);
	if (it != HorseDataTable.end())
	{
		this->SetBaseSpeed(it->second.baseSpeed);
		this->SetMaxStamina(it->second.maxStamina);
		this->SetStamina(m_MaxStamina);
		this->SetHorseType(it->second.type);
		this->SetSkills(it->second.skills);
	}
}

void Horse::ResetRaceState() {
	isFinish = false;
	isRanked = false;
	m_stamina = m_MaxStamina;

	for (Skill& skill : m_skillList)
		skill.Deactivate();
}

void Horse::InputSkill(Skill newskill)
{
	m_skillList.push_back(newskill);
}

void Horse::PrintPlayerList(const std::vector<Horse>& PlayerList)
{
	std::cout << "======================================================================================\n";
	std::cout << " No. 이름          타입   속도  스태미나  지능   스킬 목록\n";
	std::cout << "======================================================================================\n";

	for (size_t i = 0; i < PlayerList.size(); ++i)
	{
		const Horse& horse = PlayerList[i];

		// 기본 정보 열 너비 계산
		std::ostringstream oss;
		oss << std::left
			<< std::setw(4) << i + 1
			<< std::setw(14) << horse.GetName()
			<< std::setw(8) << horse.GetHorseTypeName()
			<< std::setw(6) << horse.GetBaseSpeed()
			<< std::setw(10) << horse.GetMaxStamina()
			<< std::setw(7) << horse.GetIntel();

		std::string baseLine = oss.str();
		std::cout << baseLine << "- " << horse.GetSkill(0)->GetName() << "\n";

		// 나머지 스킬 출력 시 위의 스킬 출력과 같은 열 공백 맞춤
		for (int j = 1; j < horse.m_skillList.size(); ++j)
		{
			std::cout << std::setw(baseLine.length()) << " "
				<< "- " << horse.GetSkill(j)->GetName() << "\n";
		}

		std::cout << "--------------------------------------------------------------------------------------\n";
	}
}
