#include "SkillDatabase.h"

std::vector<Skill>& SkillDatabase::GetAllSkills() 
{
    static std::vector<Skill> skills = {
        Skill("가속", 2, 0.5f, 0.0f, 3.5f, 20, 0),
        Skill("근성", 0, 0.5f, 5.0f, 1.0f, 20, 0),
        Skill("지구력회복", 0, 0.5f, 3.0f, 1.0f, 15, 0),
        Skill("폭발질주", 5, 0.2f, 0.0f, 4.0f, 40, 0),
        Skill("스퍼트", 4, 0.3f, 0.0f, 5.0f, 35, 0)
    };

    return skills;
}