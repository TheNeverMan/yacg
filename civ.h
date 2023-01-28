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
#include "upgrade.h"
#include "gov.h"
#include "audio_owner.h"
#include "texture_owner.h"
#include "logger.h"
#include "city.h"

using std::string;
using std::vector;
using std::array;
using std::map;
using std::string_view;

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

class Civ : public Help_Object, public Traits_Owner, public Texture_Owner, public Audio_Owner
{
  public:
    Civ(string n, vector<string> l, string c_i, vector<string> c_n, vector<Tech> t_t, vector<Unit> u_t, int r, int g, int b, vector<string> t, vector<Gov> go, map<string, vector<string>> g_n_r, string p, vector<Upgrade> us, string t_p, string a, string c, vector<string> r_n);
    string Get_Leader_Name();
    string Get_Full_Name();
    void Assign_Id(int i);
    int Get_Id() const;
    bool Build_City_On_Map(int x, int y, string_view fallback_city_name, string_view founding_date);
    void Conquer_City(City New_City);
    string_view Get_City_Name_By_Coordinates(int x, int y) const;
    City& Get_City_By_Coordinates(array<int, 2> Coords);
    Unit Get_Unit_On_Tile(int x, int y) const;
    Unit& Get_Unit_On_Tile_Pointer(int x, int y);
    array<int, 2> Get_Capital_Location() const;
    string_view Get_Capital_Name() const;
    int Get_Score();
    int Get_Gold() const;
    int Get_Current_Actions() const;
    int Get_Max_Actions() const;
    Tech& Get_Currently_Researched_Tech();
    int Get_Research_Percent() const;
    void End_Turn(vector<int> income);
    void Start_Turn();
    vector<Tech> Get_Possible_Research_Techs();
    vector<Gov> Get_Possible_Goverments() const;
    void Set_Research_Tech_By_Name(string_view tech_name);
    void Set_Research_Funds_Percentage(int new_val);
    bool Has_Tech_Been_Researched_By_Name(string_view tech_name) const;
    bool Has_Tech_Been_Researched_By_Trait(string_view trait_name) const;
    void Build_Upgrade(string_view upg_name);
    string_view Get_Active_Goverment_Name() const;
    int Get_Upgrade_Border_Radius() const;
    int Get_Number_Of_Stability_Techs() const;
    int Get_Number_Of_Assimilation_Techs() const;
    int32_t Get_Civ_Color() const;
    bool Has_Researched_Border_Expand_Tech_Recently();
    string_view Get_Raw_Name() const;
    string_view Get_Raw_Leader_Name() const;
    bool Change_Goverment_By_Name(string_view new_gov_name, string_view fallback_leader_name);
    vector<Unit> Get_Units() const;
    vector<Unit_On_Map>& Get_Owned_Units();
    void Recruit_Unit_By_Name(string_view name, int x, int y);
    void Remove_Unit_By_Coords(int x, int y);
    bool Has_Unit_On_Tile(int x, int y) const;
    void Disband_First_Unit();
    void Move_Unit_To_By_Coords(int unit_x, int unit_y, int dest_x, int dest_y, int cost);
    int Get_Unit_Maitenance() const;
    void Refresh_Unit_Movement_Points();
    City Lose_City_By_Coords(int x, int y);
    int Get_Number_Of_Cities_Owned() const;
    int Get_Population() const;
    int Get_Army_Manpower() const;
    vector<City>& Get_Owned_Cities();
    vector<City> Get_Owned_Cities_Not_Pointer() const;
    string_view Get_Personality() const;
    int Get_Number_Of_Researched_Techs() const;
    int Get_Number_Of_Naval_Units() const;
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Civ(xml_node<>* Root_Node);
    void Set_Research_Tech_By_Trait(string_view trait);
    bool Has_Enough_Gold_To_Build_Upgrade(string_view upg_name) const;
    Upgrade Find_Upgrade_By_Name(string_view upg_name) const;
    vector<Upgrade>& Get_Upgrades();
    int Get_Upgrade_Production_By_Name(string_view upg_name) const;
    int Get_Upgrade_Maitenance_By_Name(string_view upg_name) const;
    int Get_Upgrade_Buff_By_Name(string_view name) const;
    double Get_Defense_Bonus_For_Upgrade(string_view upg_name) const;
    void Give_One_Gold();
    bool Is_Unit_Obsolete(string_view unit_name) const;
    string_view Get_Culture_Name() const;
    bool Change_Leader_Name(string_view fallback_leader_name);
    array<string, 2> Get_Rebellion_Name_And_Flag_Path();
    vector<Tech> Get_Tech_Tree() const;
    vector<string> Get_All_Upgrade_Names_By_Trait(string_view trait_name) const;
    vector<Upgrade> Get_All_Upgrades_By_Trait(string_view trait_name) const;
    const Unit& Get_Unit_On_Tile_Pointer(int x, int y) const;
    void Lose_Gold_And_One_Action(int reduction);
  private:
    map<string, vector<string>> Goverment_Name_Replacements;
    vector<string> Rebellion_Names;
    bool recent_expand = false;
    int research_percent;
    int city_name_index;
    int points;
    int points_from_technologies;
    string leader;
    string personality;
    string culture;
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
    vector<City> Cities;
    vector<Tech> Tech_Tree;
    vector<Unit> Unit_Templates;
    vector<Unit_On_Map> Units_Owned;
    vector<Upgrade> Upgrades;
    string_view Get_State_Name();
    string_view Get_Leader_Title();
    void Calculate_Score();
    int Calculate_Unit_Maitenance() const;
    void Do_Traits();
    void Do_Trait(string_view trait_name);
    bool Is_Unit_Unlocked(string_view unit_name) const;
    void Do_Traits_Of_Researched_Tech();
};
