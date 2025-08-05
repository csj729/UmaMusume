#include "SkillDatabase.h"

std::vector<Skill>& SkillDatabase::GetAllSkills() 
{
    static std::vector<Skill> skills = {
        Skill("����", 2, 0.5f, 0.0f, 3.5f, 20, 0),
        Skill("�ټ�", 0, 0.5f, 5.0f, 1.0f, 20, 0),
        Skill("������ȸ��", 0, 0.5f, 3.0f, 1.0f, 15, 0),
        Skill("��������", 5, 0.2f, 0.0f, 4.0f, 40, 0),
        Skill("����Ʈ", 4, 0.3f, 0.0f, 5.0f, 35, 0)
    };

    return skills;
}