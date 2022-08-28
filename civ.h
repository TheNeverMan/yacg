#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<map>
#include<random>
#include<ctime>
#include<array>

#include "help_object.h"
#include "xml_serializable.h"
#include "traits_owner.h"
#include "tech.h"
#include "unit.h"
#include "gov.h"
#include "logger.h"

using namespace std;

struct Coords
{
  int x,y;
};

struct Unit_On_Map
{
  Unit Self;
  Coords Coordinates;
  Unit_On_Map(Unit u)
  {
    Self = u;
  }
  Unit_On_Map(xml_node<>* Root_Node) : Self(Root_Node->first_node("unit"))
  {

  }
};

//to be changed ???
struct Owned_City //????
{
  Coords Coordinates;
  string name;
  bool is_expanded = false;
};

class Civ : public Help_Object, public Traits_Owner
{
  public:
    Civ(string n, vector<string> l, string c_i, vector<string> c_n, vector<Tech> t_t, vector<Unit> u_t, int r, int g, int b, vector<string> t, vector<Gov> go, map<string, vector<string>> g_n_r, string p);
    string Get_Leader_Name();
    string Get_Full_Name();
    void Assign_Id(int i);
    int Get_Id();
    void Build_City_On_Map(int x, int y);
    void Build_City_On_Map_With_Name(int x, int y, string name);
    string Get_City_Name_By_Coordinates(int x, int y);
    Unit Get_Unit_On_Tile(int x, int y);
    Unit* Get_Unit_On_Tile_Pointer(int x, int y);
    array<int, 2> Get_Capital_Location();
    string Get_Capital_Name();
    int Get_Score();
    int Get_Gold();
    int Get_Current_Actions();
    int Get_Max_Actions();
    Tech *Get_Currently_Researched_Tech();
    int Get_Research_Percent();
    void End_Turn(vector<int> income);
    void Start_Turn();
    vector<Tech> Get_Possible_Research_Techs();
    vector<Gov> Get_Possible_Goverments();
    void Set_Research_Tech_By_Name(string tech_name);
    void Set_Research_Funds_Percentage(int new_val);
    bool Has_Tech_Been_Researched_By_Name(string tech_name);
    bool Has_Tech_Been_Researched_By_Trait(string trait_name);
    void Build_Upgrade(int cost);
    string Get_Active_Goverment_Name();
    int Get_Upgrade_Border_Radius();
    int32_t Get_Civ_Color();
    bool Has_Researched_Border_Expand_Tech_Recently();
    string Get_Raw_Name();
    string Get_Raw_Leader_Name();
    void Change_Goverment_By_Name(string new_gov_name);
    vector<Unit> Get_Units();
    vector<Unit_On_Map> *Get_Owned_Units();
    void Recruit_Unit_By_Name(string name, int x, int y);
    void Remove_Unit_By_Coords(int x, int y);
    bool Has_Unit_On_Tile(int x, int y);
    void Disband_First_Unit();
    void Move_Unit_To_By_Coords(int unit_x, int unit_y, int dest_x, int dest_y, int cost);
    int Get_Unit_Maitenance();
    void Refresh_Unit_Movement_Points();
    string Lose_City_By_Coords(int x, int y);
    int Get_Number_Of_Cities_Owned();
    int Get_Population();
    int Get_Army_Manpower();
    vector<Owned_City> Get_Owned_Cities();
    string Get_Personality();
    int Get_Number_Of_Researched_Techs();
    int Get_Number_Of_Naval_Units();
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Civ(xml_node<>* Root_Node);
    void Set_Research_Tech_By_Trait(string trait);
  private:
    map<string, vector<string>> Goverment_Name_Replacements;
    bool recent_expand = false;
    int research_percent;
    int city_name_index;
    int points;
    string leader;
    string personality;
    string tech_in_research;
    Gov Active_Goverment;
    vector<Gov> Goverments;
    vector<string> City_Names;
    int gold;
    int id;
    int max_actions;
    int current_actions;
    double tech_money_modifier = 1.0;
    int32_t color;
    vector<string> Leaders;
    vector<Owned_City> Cities;
    vector<Tech> Tech_Tree;
    vector<Unit> Unit_Templates;
    vector<Unit_On_Map> Units_Owned;
    string Get_State_Name();
    string Get_Leader_Title();
    void Calculate_Score();
    int Calculate_Unit_Maitenance();
    void Do_Traits();
    void Do_Trait(string trait_name);
};
