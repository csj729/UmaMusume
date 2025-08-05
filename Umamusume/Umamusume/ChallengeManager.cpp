#include "ChallengeManager.h"

void ChallengeManager::RunChallenge(std::vector<Horse>& PlayerList, Horse& player,
    RaceManager& RM, DoubleBuffering& DB,
    Tile BG[DF_BG_SIZE_Y][DF_BG_SIZE_X])
{
    RM.SelectPlayerHorse(PlayerList, player);
    Horse* AI_horses[HORSE_NUM];
    m_stage = 1;

    while (true)
    {
        std::cout << "���̽��� �����Ͻðڽ��ϴ�?? (Y/N) ";
        char sel;
        std::cin >> sel;

        if(sel == 'Y' || sel == 'y')
        {
            std::cout << m_stage << "��° ���̽��Դϴ�!\n";
            Sleep(1500);
            RM.RunRace(DB, BG, AI_horses, player, *this);

            if (RM.GetRankedHorse(0) == &player)
            {
                std::cout << m_stage++ << "���������� Ŭ�����߽��ϴ�!!\n";
            }

            else
            {
                if (m_stage == 1)
                {
                    std::cout << "Ż���Դϴ�... �� �����ϼ���.\n";
                    Sleep(1000);
                    break;
                }
                else
                {
                    std::cout << m_stage << "������������ Ŭ�����߽��ϴ�!!\n";
                    std::cout << "���� ����Ͻðڽ��ϱ�?? (Y/N): ";
                    std::cin >> sel;

                    if (sel == 'Y' || sel == 'y')
                    {
                        RegisterRanking(player, RM);
                    }

                    else
                    {
                        system("cls");
                        std::cout << "���� ����� �����մϴ�.\n";
                        Sleep(1000);
                    }
                    break;
                }
            }
        }

        else
        {
            std::cout << "������ �����մϴ�.\n";
            if (m_stage != 1)
            {
                std::cout << "���� ����Ͻðڽ��ϱ�?? (Y/N): ";
                std::cin >> sel;

                if (sel == 'Y' || sel == 'y')
                {
                    RegisterRanking(player, RM);
                }

                else
                {
                    system("cls");
                    std::cout << "���� ����� �����մϴ�.\n";
                }
            }
            Sleep(1000);
            break;
        }
    }
}

void ChallengeManager::RegisterRanking(const Horse& player, RaceManager& RM)
{
    std::vector<RankEntry> ranking;
    LoadRanking(ranking);

    // �̸� ���� �Է�
    std::string playerText;
    std::cout << "�̸��� �Է��ϼ���: ";
    std::cin >> playerText;

    // ���
    ranking.push_back({ playerText, player.GetName(), m_stage });

    std::sort(ranking.begin(), ranking.end());

    SaveRanking(ranking);

    std::cout << "��ŷ�� ����Ǿ����ϴ�!\n";
    Sleep(1500);
}

void ChallengeManager::LoadRanking(std::vector<RankEntry>& ranking)
{
    std::ifstream ifs("ranking.dat", std::ios::binary);
    if (!ifs) return;

    size_t size;
    ifs.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    for (size_t i = 0; i < size; ++i)
    {
        RankEntry entry;

        size_t len;

        // name
        ifs.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        entry.text.resize(len);
        ifs.read(&entry.text[0], len);

        // horseName
        ifs.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        entry.horseName.resize(len);
        ifs.read(&entry.horseName[0], len);

        // stage
        ifs.read(reinterpret_cast<char*>(&entry.stage), sizeof(int));

        ranking.push_back(entry);
    }

    ifs.close();
}

void ChallengeManager::SaveRanking(const std::vector<RankEntry>& ranking)
{
    std::ofstream ofs("ranking.dat", std::ios::binary);
    size_t size = ranking.size();
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size_t));

    for (const auto& entry : ranking)
    {
        size_t len;

        // name
        len = entry.text.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        ofs.write(entry.text.c_str(), len);

        // horseName
        len = entry.horseName.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
        ofs.write(entry.horseName.c_str(), len);

        // stage
        ofs.write(reinterpret_cast<const char*>(&entry.stage), sizeof(int));
    }

    ofs.close();
}

void ChallengeManager::PrintRanking()
{
    std::vector<RankEntry> ranking;
    LoadRanking(ranking);

    if (ranking.empty())
    {
        std::cout << "��ŷ �����Ͱ� �����ϴ�.\n";
        system("pause");
        return;
    }

    const int rankWidth = 6;
    const int nameWidth = 18;
    const int horseWidth = 15;
    const int stageWidth = 8;
    const int totalWidth = rankWidth + nameWidth + horseWidth + stageWidth + 9; // ������ ����

    // ���м�
    auto printLine = [&]() {
        std::cout << std::string(totalWidth, '=') << "\n";
        };

    printLine();
    std::cout << std::left
        << std::setw(rankWidth) << "����" << " | "
        << std::setw(nameWidth) << "�÷��̾��" << " | "
        << std::setw(horseWidth) << "�� �̸�" << " | "
        << std::setw(stageWidth) << "��������" << "\n";
    printLine();

    int rank = 1;
    int prevStage = -1;
    int displayRank = 1;

    for (const auto& entry : ranking)
    {
        // ���� ���������� ���� ���� ���
        if (prevStage != -1 && entry.stage < prevStage)
        {
            displayRank = rank;
        }

        std::cout << std::left
            << std::setw(rankWidth) << displayRank << " | "
            << std::setw(nameWidth) << entry.text << " | "
            << std::setw(horseWidth) << entry.horseName << " | "
            << std::setw(stageWidth) << entry.stage << "\n";

        prevStage = entry.stage;
        ++rank;
    }

    printLine();
    system("pause");
}