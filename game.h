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
#include<tuple>
#include<memory>

//#include "magic_thread_communicator.h"
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
#include "radius_generator.h"
#include "ai.h"
#include "magic_thread_communicator.h"
#include "newspaper.h"
#include "xml_data_loader.h"
#include "ai_data.h"
#include "sound_manager.h"
#include "thread.h"
#include "culture.h"

using namespace rapidxml;
using std::string;
using std::mutex;
using std::vector;
using std::lock_guard;
using std::map;
using std::stoul;
using std::thread;
using std::shared_ptr;
using std::tuple;
using std::make_tuple;
using std::get;
using std::make_shared;
using std::fstream;

class Magic_Thread_Communicator;

class Game : public XML_Serializable
{
  private:
    bool is_in_thread = false;
    mutex Main_Mutex;
    bool spectator_mode;
    bool autosave;
    vector<int> Eliminated_Players_List;
    vector<Tile> Tiles;
    vector<Tech> Technologies;
    Radius_Generator Main_Radius_Generator;
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
    map<string, Culture> Cultures;
    vector<array<int, 2>> Tiles_To_Update;
    void XML_Load_Data();
    void Do_AI_Actions_For_Currently_Moving_Player(Magic_Thread_Communicator *Thread_Portal);
    void Move_Unit(int unit_x, int unit_y, int dest_x, int dest_y, int cost);
    void Battle_Units(int unit_1_x, int unit_1_y, int unit_2_x, int unit_2_y);
    void Start_Turn_Of_Currently_Moving_Player(Magic_Thread_Communicator *Thread_Portal);
    bool Is_Currently_Moving_Player_Eliminated();
    int First_Not_Eliminated_Player_Id();
    bool Is_Player_AI(int player_id);
    bool All_Humans_Are_Eliminated();
    void Add_Player_By_Name(string name, bool is_ai);
    void Generate_Map(Map_Generator_Data User_Data, bool load_starting_positions);
    void Add_Players(vector<tuple<string, bool>> players);
    void Assing_Starting_Position_For_Player(int player_id, int x, int y);
    void Asign_Random_Starting_Position_For_Player(int player_id);
    void Assign_Random_Starting_Positions();
    void Assign_Starting_Positions_From_Data(map<string, array<int,2>> starting_positions);
    array<int, 3> Get_Units_Stats_For_Battle(int unit_x, int unit_y);
    double Get_Defense_Bonus_For_Tile_And_Player(int x, int y, int player_id);
    Unit Get_Unit_By_Tile(int x, int y);
    void Remove_All_Missle_Units();
    Culture* Get_Culture_By_Player_Id(int player_id);
    void Update_Stability_For_Currently_Moving_Player();
    vector<array<int, 3>> Search_For_Connections(array<int, 2> Coords, int player_id);
  public:
    bool Is_Only_One_Player_Alive();
    Gov Get_Goverment_By_Name(string gov_name);
    bool Is_Currently_Moving_Player_AI();
    void Disband_Unit(int x, int y);
    Unit Get_Unit_By_Name(string name);
    void Set_Autosave(bool a);
    bool Is_Player_Eliminated(int player_id);
    bool Has_Currently_Moving_Player_Any_Actions_Left();
    vector<tuple<array<string,2>, int>> Get_Newspaper_Events();
    xml_node<>*  Serialize(memory_pool<>* doc);
    string Get_Current_Turn_By_Years();
    bool Is_Map_Update_Required();
    Game(bool a, Map_Generator_Data Map_Data, vector<tuple<string, bool>> players, bool load_starting_positions, bool spectator_mode);
    Game();
    Game(xml_node<>* Root_Node);
    void Deserialize(xml_node<>* Root_Node);
    void Test();
    bool Save_Game(string path);
    tuple<bool, Game*> Load_Game(string path);
    int Get_Turn();
    int End_Player_Turn(Magic_Thread_Communicator* Thread_Portal);
    void Confirm_Pass_To_Game_Window();
    vector<Civ> Get_Players();
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
    vector<array<int, 2>> Get_Tiles_To_Update();
    void Recruit_Unit(string u, int x, int y);
    int Get_Total_Cost_Of_Technology_By_Name(string name);
    Upgrade Get_Upgrade_Of_Currently_Moving_Player(string upg_name);
    void Plunder_Tile(int x, int y);
    void Detonate_Atomic_Bomb(int x, int y);
    void Build_Upgrade(string name, int x, int y, int player_id);
    int Get_Only_Living_Player_Id();
    string Get_Texture_Path_For_Cultured_Upgrade(int x, int y, string upg_name);
};
