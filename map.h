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

using namespace std;

class Map : public XML_Serializable
{
  private:
    vector<vector<Owned_Tile>> Game_Map;
    vector<Tile> Tiles;
    vector<Upgrade> Upgrades;
    Upgrade Find_Upgrade_By_Name_In_Vector(string n, vector<Upgrade> u);
    array<int, 2> Find_Closest_Tile_Owned_By_One_Direction(int owner, int x, int y, int depth, int x_dir, int y_dir, Unit u);
    array<int, 2> Find_In_One_Direction_To_Enemy_City_Or_Unit(int owner, int x, int y, int depth, int x_dir, int y_dir, Unit u);
    int Calculate_Distance_Between_Points(int p_x, int p_y, int g_x, int g_y);
    array<int, 2> Get_Closest_Point(int x, int y, vector<array<int ,2>> points);
    bool Is_Tile_Out_Of_Bounds(int x, int y);
  public:
    xml_node<>* Serialize(memory_pool<>* doc);
    Map(int x, int y, vector<Tile> t, vector<Upgrade> u);
    Map();
    Map(xml_node<>* Root_Node);
    void Deserialize(xml_node<>* Root_Node);
    //string Info();
    int Get_X_Size();
    int Get_Y_Size();
    void Change_Tile_Owner(int x, int y, int id);
    void Upgrade_Tile(int x, int y, Upgrade t_u);
    void Change_Tile_Type(int x, int y, Tile t);
    void Print_Map_In_ASCII();
    Tile Get_Tile(int x, int y);
    bool Is_Tile_Neutral(int x, int y);
    void Build_City(int x, int y, int owner, int radius);
    int Get_Owner(int x, int y);
    vector<int> Get_Netto_Income_For_Player_By_Id(int id, Civ player);
    bool Is_Upgrade_In_Radius_By_Name(string upg_name, int x, int y);
    void Build_Upgrade(Upgrade upg, int x, int y,int owner ,int radius);
    void Retake_Owner_In_Radius(int x, int y, int owner, int radius);
    void Retake_Tile(int x, int y, int owner);
    double Get_Defense_Bonus_For_Tile(int x, int y);
    void Claim_Tile(int x, int y, int owner);
    void Claim_Tiles_In_Radius(int x, int y, int owner, int radius);
    void Recalculate_Borders_For_Player_By_Id(int owner, int radius);
    Tile* Get_Tile_Pointer(int x, int y);
    void Unclaim_All_Player_Tiles(int player);
    vector<int> Check_If_Path_For_Unit_Exists(int unit_x, int unit_y, int dest_x, int dest_y, Unit u);
    vector<int> Find_Owned_Tile_For_Upgrade(int owner_id, string upg_name);
    int Count_Tiles_Owned_By_Player(int owner, string tile_name);
    array<int, 2> Find_Closest_Tile_Owned_By(int owner, int x, int y, int depth, Unit u);
    vector<int> Find_Direction_Away_From_Borders(int owner, int x, int y, int movement_points, Unit u);
    vector<int> Find_Direction_To_Enemy_City_Or_Unit(int unit_owner_id, int x, int y, int movement_points, Unit u);
};
