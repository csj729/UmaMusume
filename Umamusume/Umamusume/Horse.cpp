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
	const char* frame0[4] = {
		"  __ O  /",
		" /  / -- ",
		"   ====  ",
		"__/   /  "
	};

	const char* frame1[4] = {
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
	if (m_name == "메지로 맥퀸")
	{
		m_baseSpeed = 5;
		m_MaxStamina = 30.0f;
		m_stamina = m_MaxStamina;
		m_type = LEADER;
		isFinish = false;
		isRanked = false;
		m_skillList[0] = Skill("존귀한 사명을 완수하기 위하여", 15, 1.0f, 0.0f, 10.0f);  // 높은 발동률, 초반 스퍼트
		m_skillList[1] = Skill("스태미너 킵", 1, 0.8f, 3.0f, 1.0f); // 체력 회복 보조
		m_skillList[2] = Skill("속도 유지", 2, 0.6f, 0.0f, 4.0f);  // 안정적인 속도 유지
	}

	else if (m_name == "골드 쉽")
	{
		m_baseSpeed = 5;
		m_MaxStamina = 30.0f;
		m_stamina = m_MaxStamina;
		m_type = CLOSER;
		isFinish = false;
		isRanked = false;
		m_skillList[0] = Skill("불침함, 출항!!", 15, 0.5f, 0.0f, 5.0f);  // 조건 만족 시 확정 발동
		m_skillList[1] = Skill("추격", 4, 0.5f, 0.0f, 5.0f);  // 저확률로 반격 기회
		m_skillList[2] = Skill("후방 대기", 1, 0.8f, 1.0f, 5.0f); // 후반 지구력 보강
	}

	else if (m_name == "오구리 캡")
	{
		m_baseSpeed = 5;
		m_MaxStamina = 30.0f;
		m_stamina = m_MaxStamina;
		m_type = STALKER;
		isFinish = false;
		isRanked = false;
		m_skillList[0] = Skill("승리의 고동", 15, 0.5f, 0.0f, 5.0f);   // 리더 바로 뒤에 있을 때 효과적
		m_skillList[1] = Skill("속도 가속", 2, 0.8f, 0.0f, 5.0f);   // 꾸준한 주행
		m_skillList[2] = Skill("영양 보급", 0, 0.5f, 2.0f, 1.0f);  // 장거리 주행 유리
	}

	else if (m_name == "마루젠스키")
	{
		m_baseSpeed = 5;
		m_MaxStamina = 30.0f;
		m_stamina = m_MaxStamina;
		m_type = PACESETTER;
		isFinish = false;
		isRanked = false;
		m_skillList[0] = Skill("홍염 기어/LP1211-M", 13, 0.5f, 0.0f, 6.0f);  // 장시간 속도 유지
		m_skillList[1] = Skill("앞장서기", 2, 0.6f, 0.0f, 5.0f);  // 회복 중심
		m_skillList[2] = Skill("기어 시프트", 1, 0.8f, 0.0f, 5.0f);  // 스태미너-속도 균형
	}

}

std::string Horse::SelectName(const std::string horseName[])
{
	// HorseName에 있는 말들 중 하나를 랜덤하게 선택
	int roll = rand() % HORSETABLE_NUM;
	return m_name = horseName[roll];
}

void Horse::HorseTick(int leader_X, float deltaTime)
{
	if (isFinish)
		return;

	float staminaDrain = 0.0f;
	int speedModifier_type = 0;
	int speedModifier_stamina = 0;

	int distanceFromLeader = leader_X - m_Position.X;

	// 전략에 따른 속도/기력 소모 보정
	switch (m_type)
	{
	case PACESETTER:  // 도주
		speedModifier_type = 4;          // 초반 빠름
		staminaDrain = 0.12f;       // 기력 소모 큼
		break;

	case LEADER:  // 선행
		speedModifier_type = 2;          // 전체적으로 준수
		staminaDrain = 0.08f;       // 무난한 기력 소모
		break;

	case STALKER:  // 선입
		if (distanceFromLeader >= 200) 
		{
			speedModifier_type = 3;      // 선두와 거리 벌어지면 가속
			staminaDrain = 0.1f;
		}
		else
		{
			speedModifier_type = 1;
			staminaDrain = 0.04f;
		}
		break;

	case CLOSER:  // 추입
		if (leader_X >= FinishLine * 2 / 3) 
		{
			speedModifier_type = 8;      // 막판 급가속
			staminaDrain = 0.15f;
		}
		else 
		{
			speedModifier_type = -1;     // 초반은 느림
			staminaDrain = 0.02f;   // 체력 보존
		}
		break;
	}

	// 기력 감소
	m_stamina -= staminaDrain;
	if (m_stamina < 0.0f) m_stamina = 0.0f;

	// 상태 업데이트
	if (m_stamina > m_MaxStamina * 3.0f / 4.0f)
		m_vitStatus = ENERGETIC;
	else if (m_stamina > m_MaxStamina * 2.0f / 4.0f)
		m_vitStatus = NORMAL;
	else if (m_stamina > m_MaxStamina / 4.0f)
		m_vitStatus = TIRED;
	else
		m_vitStatus = EXHAUSTED;

	// 속도 계산
	
	switch (m_vitStatus)
	{
	case ENERGETIC:
		speedModifier_stamina = 2;  // 약간의 보너스
		break;

	case NORMAL:
		// 변화 없음
		break;

	case TIRED:
		speedModifier_stamina = -1;  // 살짝 느려짐
		break;

	case EXHAUSTED:
		speedModifier_stamina = -3;  // 너무 느려지지 않게 최소 보장
		break;
	}
	m_realSpeed = m_baseSpeed + speedModifier_type + speedModifier_stamina;

	// 레이스 진행도 및 선두 여부 계산
	float raceProgress = (float)m_Position.X / FinishLine;
	bool isLeading = (m_Position.X >= leader_X);

	// 스킬 처리
	bool hpRecoveredThisTick = false;
	int totalSpeedBuff = 0;

	for (int i = 0; i < SKILL_NUM; ++i)
	{
		Skill& skill = m_skillList[i];

		if (skill.IsActive())
		{
			skill.UpdateDuration(deltaTime);

			if (skill.IsActive())
			{
				totalSpeedBuff += skill.GetSpeed();
			}
			else
			{
				skill.Deactivate();
			}
		}
		else
		{
			// 조건에 따른 확률 적용 후 발동 시도
			if (skill.ShouldActivate(raceProgress, m_stamina, m_MaxStamina, isLeading))
			{
				skill.Activate();

				if (!hpRecoveredThisTick)
				{
					m_stamina += skill.GetHp();
					if (m_stamina > m_MaxStamina) m_stamina = m_MaxStamina;
					hpRecoveredThisTick = true;
				}
			}
		}
	
	}

	// 스킬 속도 증가 효과 한 번만 추가
	m_realSpeed += totalSpeedBuff;

	if (m_realSpeed < 1)
		m_realSpeed = 1;

	m_Position.X += m_realSpeed;
}

void Horse::CheckFinish()
{
	if (m_Position.X >= FinishLine)
		isFinish = true;
}

const Skill* Horse::GetActiveSkill() const
{
	for (int i = 0; i < SKILL_NUM; ++i)
	{
		if (m_skillList[i].IsActivated())
		{
			return &m_skillList[i];
		}
	}
	return nullptr;
}

