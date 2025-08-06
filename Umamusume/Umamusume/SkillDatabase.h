#pragma once
#include "Skill.h"
#include <vector>

class SkillDatabase
{
public:
    static std::vector<Skill>& GetAllSkills();
};