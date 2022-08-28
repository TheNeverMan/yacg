#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<random>
#include<ctime>
#include<memory>
#include<algorithm>
#include<fstream>
#include<map>
#include<memory>

#include "tile.h"
#include "tech.h"
#include "unit.h"
#include "civ.h"
#include "map_generator_data.h"
#include "map.h"
#include "map_generator.h"
#include "upgrade.h"
#include "gov.h"
#include "game_saver.h"
#include "game_loader.h"
#include "logger.h"
#include "xml_serializable.h"
#include "help_object.h"
#include "ai.h"
#include "newspaper.h"
#include "ai_data.h"

using namespace rapidxml;
using namespace std;

class Game : public XML_Serializable
{
  private:
    bool autosave;
    vector<int> Eliminated_Players_List;
    vector<Tile> Tiles;
    vector<Tech> Technologies;
    vector<Unit> Units;
    vector<Civ> Civs;
    vector<Civ> Players;
    vector<Gov> Goverments;
    vector<Upgrade> Upgrades;
    vector<uint32_t> Player_Border_Colors;
    vector<bool> Is_Player_AI_List;
    map<int, AI_Data> AI_Data_For_Players;
    Map Game_Map;
    Newspaper Main_Newspaper;
    int currently_moving_player;
    int turn_counter;
    void XML_Load_Civs();
    void XML_Load_Data();
    void XML_Load_Tiles();
    void XML_Load_Techs();
    void XML_Load_Units();
    void Do_AI_Actions_For_Currently_Moving_Player();
    bool Is_Currently_Moving_Player_AI();
    void XML_Load_Upgrades();
    void XML_Load_Goverments();
    Map_Generator_Data Show_Map_Generator_UI();
    void Show_Players_Generator_UI();
    void Move_Unit(int unit_x, int unit_y, int dest_x, int dest_y, int cost);
    void Battle_Units(int unit_1_x, int unit_1_y, int unit_2_x, int unit_2_y);
    void Start_Turn_Of_Currently_Moving_Player();
    bool Is_Currently_Moving_Player_Eliminated();
    int First_Not_Eliminated_Player_Id();
    bool Is_Player_AI(int player_id);
    bool All_Humans_Are_Eliminated();
    vector<Coords> Tiles_To_Update;
  public:
    Unit Get_Unit_By_Name(string name);
    bool Is_Player_Eliminated(int player_id);
    bool Has_Currently_Moving_Player_Any_Actions_Left();
    vector<string> Get_Newspaper_Events();
    xml_node<>*  Serialize(memory_pool<>* doc);
    string Get_Current_Turn_By_Years();
    bool Is_Map_Update_Required();
    void Generate_Map(Map_Generator_Data User_Data);
    void Assign_Starting_Positions();
    Game(bool a);
    Game();
    Game(xml_node<>* Root_Node);
    void Deserialize(xml_node<>* Root_Node);
    void Add_Players(int player_id, int other_players_player_count, bool allow_duplicates);
    void Test();
    bool Save_Game(string path);
    tuple<bool, Game*> Load_Game(string path);
    int Get_Turn();
    bool End_Player_Turn();
    void Confirm_Pass_To_Game_Window();
    Civ *Get_Player_By_Id(int id);
    int Get_Currently_Moving_Player_Id();
    int Get_Amount_Of_Players();
    vector<Civ> Get_All_Civs();
    Civ* Get_Currently_Moving_Player();
    vector<Upgrade> Get_Upgrades();
    vector<Unit> Get_Units();
    void Check_For_Dead_Players();
    Upgrade Get_Upgrade_By_Name(string name);
    bool Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(int unit_x, int unit_y, int dest_x, int dest_y, int movement_cost, bool combat, int enemy_unit_x, int enemy_unit_y);
    Map *Get_Map();
    uint32_t Get_Border_Color_By_Player_Id(int id);
    void Change_Goverment_For_Currently_Moving_Player_By_Name(string name);
    void Build_City(int x, int y, int owner, int radius);
    vector<Coords> Get_Tiles_To_Update();
    void Recruit_Unit(string u, int x, int y);
    int Get_Total_Cost_Of_Technology_By_Name(string name);
};
