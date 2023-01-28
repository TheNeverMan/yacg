#pragma once
#include<map>
#include<vector>
#include<iostream>
#include<cmath>
#include<array>

#include "help_object.h"
#include "tile.h"
#include "owned_tile.h"
#include "upgrade.h"
#include "unit.h"
#include "xml_serializable.h"
#include "logger.h"
#include "civ.h"
#include "radius_generator.h"

using std::string_view;
using std::map;
using std::find;
using std::vector;
using std::array;
using std::pow;

class Map : public XML_Serializable
{
  private:
    Radius_Generator Main_Radius_Generator;
    vector<vector<Owned_Tile>> Game_Map;
    vector<Tile> Tiles;
    vector<Upgrade> Upgrades;
    Upgrade Find_Upgrade_By_Name_In_Vector(string_view n, vector<Upgrade> u) const;
    vector<array<int, 2>> Find_Closest_Tile_Owned_By_One_Direction(int owner, int x, int y, int x_dir, int y_dir, Unit u, string_view tile_type) const;
    vector<array<int, 2>> Find_In_One_Direction_To_Enemy_City_Or_Unit(int owner, int x, int y, int x_dir, int y_dir, Unit u);
    int Calculate_Distance_Between_Points(int p_x, int p_y, int g_x, int g_y) const;
    array<int, 2> Get_Closest_Point(int x, int y, vector<array<int ,2>> points) const;
    bool Is_Tile_Out_Of_Bounds(int x, int y) const;
    int Calculate_Buff_For_Tile(int x, int y, int owner, Civ player) const;
    int Get_Buff_From_Tile(int x, int y, int owner, Civ player) const;
    array<int, 2> Calculate_Production_For_Tile(int x, int y, int id, Civ player) const;
    void Buff_Tiles_In_Radius(int x, int y, int radius);
    void Debuff_Tiles_In_Radius(int x, int y, int radius);
    vector<array<int,2>> Find_All_Tiles_In_Radius(array<int, 2> Coords, vector<string> Allowed_Tiles, int radius) const;
  public:
    const Tile& Get_Tile_Pointer(int x, int y) const;
    xml_node<>* Serialize(memory_pool<>* doc);
    Map(vector<Tile> t, vector<Upgrade> u);
    void Set_Size(int x, int y);
    Map();
    Map(xml_node<>* Root_Node);
    void Deserialize(xml_node<>* Root_Node);
    //string_view Info();
    int Get_X_Size() const;
    int Get_Y_Size() const;
    void Change_Tile_Owner(int x, int y, int id);
    void Upgrade_Tile(int x, int y, string_view t_u);
    void Change_Tile_Type(int x, int y, Tile t);
    Tile Get_Tile(int x, int y) const;
    bool Is_Tile_Neutral(int x, int y) const;
    bool Is_Tile_Upgraded(int x, int y) const;
    void Build_City(int x, int y, int owner, int radius);
    string_view Get_Upgrade(int x, int y) const;
    int Get_Owner(int x, int y) const;
    bool Can_Tile_Plundered(int x, int y) const;
    void Plunder_Tile(int x, int y);
    vector<int> Get_Netto_Income_For_Player_By_Id(int id, Civ& player) const;
    bool Is_Upgrade_In_Radius_By_Name(string_view upg_name, int x, int y) const;
    void Build_Upgrade(Upgrade upg, int x, int y,int owner ,int radius);
    void Retake_Owner_In_Radius(int x, int y, int owner, int radius);
    void Retake_Owner_In_Radius_From(int x, int y, int owner, int radius, int former_owner_id);
    void Retake_Tile(int x, int y, int owner);
    void Retake_Tile_From(int x, int y, int owner, int former_owner_id);
    double Get_Defense_Bonus_For_Tile(int x, int y) const;
    void Claim_Tile(int x, int y, int owner);
    void Claim_Tiles_In_Radius(int x, int y, int owner, int radius);
    vector<array<int,2>> Recalculate_Borders_For_Player_By_Id(int owner, int radius, Civ player);
    Tile& Get_Tile_Pointer(int x, int y);
    vector<array<int, 2>> Unclaim_All_Player_Tiles(int player);
    vector<int> Check_If_Path_For_Unit_Exists(int unit_x, int unit_y, int dest_x, int dest_y, Unit u) const;
    vector<int> Find_Owned_Tile_For_Upgrade(int owner_id, string_view upg_name) const;
    int Count_Tiles_Owned_By_Player(int owner, string_view tile_name) const;
    array<int, 2> Find_Closest_Tile_Owned_By(int owner, int x, int y, Unit u, string_view tile_type) const;
    vector<int> Find_Direction_Away_From_Borders(int owner, int x, int y, int movement_points, Unit u) const;
    vector<int> Find_Direction_To_Enemy_City_Or_Unit(int unit_owner_id, int x, int y, int movement_points, Unit u);
    vector<array<int, 2>> Find_All_Upgrade_Locations(int owner, string_view upg_name) const;
    vector<int> Find_Direction_To_Settle_City(int owner, int x, int y, Unit u);
    array<int, 2> Find_Closest_Upgrade_By_Name(array<int,2> Coords, int owner, string_view name) const;
    vector<array<int, 2>> Find_All_Upgrade_Locations_In_Radius(array<int, 2> Coords, int owner, int radius, string_view upg_name) const;
    vector<array<int, 2>> Get_Path_Tiles(array<int, 2> Start_Coords, array<int, 2> End_Coords, vector<string> Allowed_Tiles);
};
