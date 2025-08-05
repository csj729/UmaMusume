#include "FileSystem.h"

bool FileSystem::SavePlayerList(const std::string& filename, const std::vector<Horse>& playerList)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) return false;

    size_t count = playerList.size();
    ofs.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

    for (const auto& horse : playerList)
    {
        std::string name = horse.GetName();
        size_t nameLen = name.size();
        ofs.write(reinterpret_cast<const char*>(&nameLen), sizeof(size_t));
        ofs.write(name.c_str(), nameLen);

        int type = static_cast<int>(horse.GetHorseType());
        ofs.write(reinterpret_cast<const char*>(&type), sizeof(int));

        short speed = horse.GetBaseSpeed();
        short stam = horse.GetMaxStamina();
        short intel = horse.GetIntel();
        ofs.write(reinterpret_cast<const char*>(&speed), sizeof(short));
        ofs.write(reinterpret_cast<const char*>(&stam), sizeof(short));
        ofs.write(reinterpret_cast<const char*>(&intel), sizeof(short));

        // 스킬
        size_t skillCount = horse.GetSkillNumber();
        ofs.write(reinterpret_cast<const char*>(&skillCount), sizeof(size_t));
        for (int i = 0; i < skillCount; ++i)
        {
            std::string skillName = horse.GetSkill(i)->GetName();
            short speed = horse.GetSkill(i)->GetSpeed();
            float chance = horse.GetSkill(i)->GetChance();
            float stamina = horse.GetSkill(i)->GetStamina();
            float duration = horse.GetSkill(i)->GetDuration();

            size_t skillNameLen = skillName.size();
            ofs.write(reinterpret_cast<const char*>(&skillNameLen), sizeof(size_t));
            ofs.write(reinterpret_cast<const char*>(&speed), sizeof(short));
            ofs.write(reinterpret_cast<const char*>(&chance), sizeof(float));
            ofs.write(reinterpret_cast<const char*>(&stamina), sizeof(float));
            ofs.write(reinterpret_cast<const char*>(&duration), sizeof(float));
            ofs.write(skillName.c_str(), skillNameLen);
        }
    }

    return true;
}

bool FileSystem::LoadPlayerList(const std::string& filename, std::vector<Horse>& playerList)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return false;

    size_t count = 0;
    ifs.read(reinterpret_cast<char*>(&count), sizeof(size_t));
    playerList.clear();

    for (size_t i = 0; i < count; ++i)
    {
        Horse horse;

        // 이름
        size_t nameLen;
        ifs.read(reinterpret_cast<char*>(&nameLen), sizeof(size_t));
        std::string name(nameLen, ' ');
        ifs.read(&name[0], nameLen);
        horse.SetName(name);

        // 타입
        int type;
        ifs.read(reinterpret_cast<char*>(&type), sizeof(int));
        horse.SetHorseType(static_cast<HorseType>(type));

        // 능력치
        short speed, stam, intel;
        ifs.read(reinterpret_cast<char*>(&speed), sizeof(short));
        ifs.read(reinterpret_cast<char*>(&stam), sizeof(short));
        ifs.read(reinterpret_cast<char*>(&intel), sizeof(short));
        horse.SetBaseSpeed(speed);
        horse.SetMaxStamina(stam);
        horse.SetIntel(intel);

        // 스킬
        std::vector<Skill> skillList;
        size_t skillCount = 0;
        ifs.read(reinterpret_cast<char*>(&skillCount), sizeof(size_t));

        for (size_t j = 0; j < skillCount; ++j)
        {
            size_t skillNameLen;
            ifs.read(reinterpret_cast<char*>(&skillNameLen), sizeof(size_t));

            short skillSpeed;
            float chance, stamina, duration;

            ifs.read(reinterpret_cast<char*>(&skillSpeed), sizeof(short));
            ifs.read(reinterpret_cast<char*>(&chance), sizeof(float));
            ifs.read(reinterpret_cast<char*>(&stamina), sizeof(float));
            ifs.read(reinterpret_cast<char*>(&duration), sizeof(float));

            std::string skillName(skillNameLen, ' ');
            ifs.read(&skillName[0], skillNameLen);

            Skill skill(skillName); // 기본 생성자 + 이름 설정자
            skill.SetSpeed(skillSpeed);
            skill.SetBaseChance(chance);
            skill.SetStamina(stamina);
            skill.SetDuration(duration);

            skillList.push_back(skill);
        }

        horse.SetSkills(skillList);
        playerList.push_back(horse);
    }

    return true;
}