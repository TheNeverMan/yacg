#pragma once
#include<vector>
#include<string>
#include<algorithm>
#include<random>

#include "game.h"
#include "logger.h"
#include "ai_data.h"

using namespace std;



class Game;

class AI
{
  private:
    Game *Main_Game;
    bool Recruit_Unit_In_City();
    void Move_Unit_Away_From_Borders(Unit_On_Map unit);
    void Settle_City_With_Unit();
    bool Can_Settle_City_With_Unit();
    int Find_Biggest_Parameter(vector<int> input);
    int Build_Random_Producing_Upgrade();
    bool Is_Income_For_Currently_Moving_Player_Is_Negative();
    void Move_All_Units_Not_In_Cities_Away_From_Borders();
    void Move_All_Units_Not_In_Cities_To_Enemy();
    void Move_Unit_Towards_Enemy(int x, int y, Unit u);
    void Change_Goverment_If_Necessary();
    void Build_Naval_Producing_Upgrades();
    void Build_Naval_Recruitment_Upgrades();
    bool Recruit_Naval_Units();
    bool Recruit_Unit_By_Class_And_Coords(int x, int y, string unit_class);
    array<int, 2> Get_Closest_Player_Capital_Location();
    void Change_Goverment_To_More_Advanced_One();
    double Change_Technology_Goal(double technologic_parameter, int tech_class);
    int Calculate_Distance_Between_Points(int p_x, int p_y, int g_x, int g_y);
    array<int,2> Get_Closest_Point(int x, int y, vector<array<int, 2>> points);
    void Heal_Units_In_Cities();
    bool Recruit_Non_Infantry_Unit();
    int Get_Currently_Moving_Player_Finances();
  public:
    AI_Data Process_Turn(AI_Data Data);
    AI(Game *m_g);
};
