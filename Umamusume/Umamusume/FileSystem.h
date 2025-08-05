#pragma once

#include <string>
#include <iostream>
#include "Defines.h"
#include "Horse.h"  // Horse Ŭ���� ���� ����
#include "Skill.h"  // Skill Ŭ���� ���� ����

class FileSystem
{
public:
    // Horse ������ �ؽ�Ʈ ���Ϸ� �����մϴ�.
    bool SavePlayerList(const std::string& filename, const std::vector<Horse>& playerList);

    // �ؽ�Ʈ ���Ϸκ��� Horse ��ü�� �ҷ��ɴϴ�.
    bool LoadPlayerList(const std::string& filename, std::vector<Horse>& playerList);
};