#pragma once

#include <string>
#include <iostream>
#include "Defines.h"
#include "Horse.h"  // Horse 클래스 정의 포함
#include "Skill.h"  // Skill 클래스 정의 포함

class FileSystem
{
public:
    // Horse 정보를 텍스트 파일로 저장합니다.
    bool SavePlayerList(const std::string& filename, const std::vector<Horse>& playerList);

    // 텍스트 파일로부터 Horse 객체를 불러옵니다.
    bool LoadPlayerList(const std::string& filename, std::vector<Horse>& playerList);
};