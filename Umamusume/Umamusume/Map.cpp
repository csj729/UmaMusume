#include "Map.h"

Map::Map()
{
	

}

void Map::MapRender(Tile(*BG)[DF_BG_SIZE_X])
{
    const int laneHeight = LANE_HEIGHT; // 예: 10
    const int totalLanes = LANE_NUM;
    int startY = (DF_BG_SIZE_Y - (totalLanes * laneHeight + (totalLanes - 1))) / 2;
    
    for (int lane = 0; lane < totalLanes; ++lane)
    {
        int yOffset = startY + lane * (laneHeight + 1);
        for (int i = 0; i < laneHeight; ++i)
        {
            int y = yOffset + i;
            for (int x = 0; x < DF_BG_SIZE_X; ++x)
            {
                int worldX = m_scrollX + x;

                // 위아래 경계선
                if ( i == laneHeight - 1)
                    BG[y][x].Text = '-';
                else
                    BG[y][x].Text = ' ';

                // 시작선과 결승선 위치 (월드 좌표 기준)
                const int startLine = 3;

                if (i != 0 && i != laneHeight - 1 && worldX == startLine)
                    BG[y][x].Text = '|';
            }
        }
    }

    // 결승선 따로 그리기 (스크롤 반영 위치에만)
    int finishScreenX = m_finishLine - m_scrollX;
    if (finishScreenX >= 0 && finishScreenX < DF_BG_SIZE_X)
    {
        for (int lane = 0; lane < totalLanes; ++lane)
        {
            int yOffset = startY + lane * (laneHeight + 1);
            for (int i = 1; i < laneHeight - 1; ++i)
            {
                int y = yOffset + i;
                BG[y][finishScreenX].Text = '|';
            }
        }
    }
}

void SplitLines(const std::string& str, std::vector<std::string>& lines) 
{
    size_t pos = 0, prev = 0;
    while ((pos = str.find('\n', prev)) != std::string::npos) 
    {
        lines.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    if (prev < str.size())
    {
        lines.push_back(str.substr(prev));
    }
}


void Map::SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Map::IntroRender()
{
    std::string UmaMusume = R"(
                         :;!:          :;=$       
                        *$**!          !*$$       
                        *$;*$~~---~~~-**!=$       
                        *$!!***~!*****=*!=$       
                        *$!;!$$*$$$$###*!$$       
                        *$!;=**$*!!!!!**=$$       
                        *$=**=$*!;::::;;;;!=      
                        *$$=!$$*;::::::::::$$     
                      !$~.:;...=!::::::::::;;$    
                      !#~ :;   $!:::::;;;;;;:$    
                     !$$~ -- :;$!::~.:*;-*=;:;=   
                   ;$;  !#$- $$::::~.,;!;=$*!:$=  
                   ;$!,,!$$:,==:;;;!: ;#!=$*!;==  
                   ;$#$$=**$$;;:;!$!  ;#!=$!!;==  
                   :==$=**!=$;;:!!*:. ~!!****;==  
                   --!=*!!;*=;::$*..-:, !-.$#***  
             $$###=--!=!:::*$;:;$*  ~$~ !, =#     
           !=*****=!!*!;::;*$::;**  ~$~ -. =#!    
          =$$::::;*$$*::::!=$;;;**  ~$~    =#~    
         *!!!:::::;!!;::;;!=$==;!!  ~=-    ~:*;   
        *#!:*===*!;::;!!**=$=$$;;!!~.,.      $*   
        *#!:$$$#$*!::;****$$*$$;:!$;         $*   
        *#$=;::$$$=!!!**=$$$$$$$!;$!.      *=:    
        *##$:~~$$$=*****=$$$$##$=!#*.      =$     
          =$:=$******=$$*;~--**~$=~;$$$$$$$~~     
               $$####=::-----:;!--.::-:!$:        
               $=$$$#=;;~----;;!,-.::-:;$;~       
                   ;#;:!~----***:-  . -::$#~-####=
                   ;*::!~~-~:!!!:~,--,~;:$#;:===#=
                   ! .;*;:!=#::;:;**!::;;**$$, .#=
                  !! ~##!~=$$::;;:,~**!:!:-$#, .#=
        *=;:;!;==!=! ,;;!!===;:;;;;;!*!;;;:==-.,#=
       !*;:::****#$!   .;$*!;::!!!$*;***:;!**-,~#=
       !=***!****=$!  .,!$!;;:;;!!!!*!;!!:~~:!!!!;
      *=$$*;=***!*==::!!;:~:-,,:;:,~*=;~;!!!!!;;
    ;==$$;-=*****=$::*$;--~   ;~- ,!#!~~=$$$
     :;;;;~  =$$=~:::;!;~,.   ~..  -!*=;::::
             $$$$=-~;;;;::,    -    ,:*#;
               :::*!..~;:::::::-   -;:*#;
               $$$$$$$: -~:::::::::~  ;#;
               $$$$$##: ,-:::::;;::~  ;$;
             $$****=$$$$$$,  :::   ~$$!
              $****===$$=*-,,:~~...~$#!
             $$!**==**===*===..,==::!=*:
             $$**$=;$$=!=#:     $=::::*$;
             $$==*!:**=***:     **!!;:*$;
             $$$$-~--~!#;         :#=;:*$!
                                  *!:!==****
                                   ;=;;:;**$#
                                   ;=!;~~;!$$
                                   ;==!;!==;;
                                    :*$*!=$$

)";

    std::string str_UMA = R"(
  ___ ___ ___ ___ _______ 
 |   Y   |   Y   |   _   |
 |.  |   |.      |.  1   |
 |.  |   |. \_/  |.  _   |
 |:  1   |:  |   |:  |   |
 |::.. . |::.|:. |::.|:. |
 `-------`--- ---`--- ---')";

    std::string str_MUSUME = R"(
  ___ ___ ___ ___ _______ ___ ___ ___ ___ _______ 
 |   Y   |   Y   |   _   |   Y   |   Y   |   _   |
 |.      |.  |   |   1___|.  |   |.      |.  1___|
 |. \_/  |.  |   |____   |.  |   |. \_/  |.  __)_ 
 |:  |   |:  1   |:  1   |:  1   |:  |   |:  1   |
 |::.|:. |::.. . |::.. . |::.. . |::.|:. |::.. . |
 `--- ---`-------`-------`-------`--- ---`-------')";

    std::string str_PRETTY = R"(
  _______ _______ _______ _______ _______ ___ ___ 
 |   _   |   _   |   _   |       |       |   Y   |
 |.  1   |.  l   |.  1___|.|   | |.|   | |   1   |
 |.  ____|.  _   |.  __)_`-|.  |-`-|.  |-'\_   _/ 
 |:  |   |:  |   |:  1   | |:  |   |:  |   |:  |  
 |::.|   |::.|:. |::.. . | |::.|   |::.|   |::.|  
 `---'   `--- ---`-------' `---'   `---'   `---'  )";

    std::string str_DERBY = R"(
  ______   _______ _______ _______ ___ ___ 
 |   _  \ |   _   |   _   |   _   |   Y   |
 |.  |   \|.  1___|.  l   |.  1   |   1   |
 |.  |    |.  __)_|.  _   |.  _   \\_   _/ 
 |:  1    |:  1   |:  |   |:  1    \|:  |  
 |::.. . /|::.. . |::.|:. |::.. .  /|::.|  
 `------' `-------`--- ---`-------' `---'  )";
          
    std::string str_EXTRA = R"(
     _______ _______ ___ ___ _______      
    |   _   |   _   |   Y   |   _   |     
    |.  |___|.  1   |.      |.  1___|     
    |.  |   |.  _   |. \_/  |.  __)_      
    |:  1   |:  |   |:  |   |:  1   |     
    |::.. . |::.|:. |::.|:. |::.. . |     
    `-------`--- ---`--- ---`-------'     
  _______ _______ _______ _______ _______ 
 |   _   |       |   _   |   _   |       |
 |   1___|.|   | |.  1   |.  l   |.|   | |
 |____   `-|.  |-|.  _   |.  _   `-|.  |-'
 |:  1   | |:  | |:  |   |:  |   | |:  |  
 |::.. . | |::.| |::.|:. |::.|:. | |::.|  
 `-------' `---' `--- ---`--- ---' `---'  )";

    std::vector<std::string> leftLines;
    std::vector<std::string> rightLines;

    SplitLines(UmaMusume, leftLines);

    std::string combinedRight = str_UMA + "\n" + str_MUSUME + "\n" + str_PRETTY + "\n" + str_DERBY;
    SplitLines(combinedRight, rightLines);

    // --- 위치 조정 설정 ---
    int leftOffsetX = 20;
    int leftOffsetY = 3;
    int rightOffsetX = 20;
    int rightOffsetY = 10;
    int gapBetween = 6;

    // 왼쪽 줄 최대 길이 계산
    size_t maxLeftLen = 0;
    for (const auto& line : leftLines)
        if (line.length() > maxLeftLen)
            maxLeftLen = line.length();

    // 왼쪽 칼럼 폭 계산
    int leftWidth = static_cast<int>(leftOffsetX + maxLeftLen + gapBetween);

    // 총 출력 줄 수 계산
    size_t totalLines = max(leftLines.size() + leftOffsetY, rightLines.size() + rightOffsetY);

    // 출력 루프
    for (size_t i = 0; i < totalLines; ++i)
    {
        std::string left = "";
        std::string right = "";

        // 왼쪽 출력
        if (i >= static_cast<size_t>(leftOffsetY))
        {
            size_t index = i - leftOffsetY;
            if (index < leftLines.size())
                left = std::string(leftOffsetX, ' ') + leftLines[index];
        }

        // 오른쪽 출력
        if (i >= static_cast<size_t>(rightOffsetY))
        {
            size_t index = i - rightOffsetY;
            if (index < rightLines.size())
                right = std::string(rightOffsetX, ' ') + rightLines[index];
        }

        // 좌우 정렬 출력
        std::cout << std::left << std::setw(leftWidth) << left << right << '\n';
    }
}

void Map::SetScrollX(int x) 
{
    if (x < 0) x = 0;
    int maxScroll = m_mapSize - DF_BG_SIZE_X;
    if (x > maxScroll) x = maxScroll;
    m_scrollX = x;
}

void Map::SetMapSizeRandom()
{
    int randValue = rand() % 3;
    if (randValue == 0)
    {
        m_mapSize = SHORT_DISTANCE;
    }
    else if (randValue == 1)
    {
        m_mapSize = MEDIUM_DISTANCE;
    }
    else
    {
        m_mapSize = LONG_DISTANCE;
    }

    m_finishLine = m_mapSize - 5;
}

std::string Map::GetDistanceTypeString() const
{
    switch (m_mapSize)
    {
    case SHORT_DISTANCE: return "단거리";
    case MEDIUM_DISTANCE: return "중거리";
    case LONG_DISTANCE: return "장거리";
    default: return "알 수 없음";
    }
}
