#include "FileSystem.h"
#include <fstream>

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
        for (int i = 0; i < SKILL_NUM; ++i)
        {
            std::string skillName = horse.GetSkill(i)->GetName();
            size_t skillNameLen = skillName.size();
            ofs.write(reinterpret_cast<const char*>(&skillNameLen), sizeof(size_t));
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
        std::vector<Skill> skillList(SKILL_NUM);
        for (int j = 0; j < SKILL_NUM; ++j)
        {
            size_t skillNameLen;
            ifs.read(reinterpret_cast<char*>(&skillNameLen), sizeof(size_t));
            std::string skillName(skillNameLen, ' ');
            ifs.read(&skillName[0], skillNameLen);

            skillList[j].SetName(skillName);  // Skill(string name) 생성자 필요
        }
        horse.SetSkills(skillList); // Skill[]을 설정하는 함수 필요

        playerList.push_back(horse);
    }

    return true;
}