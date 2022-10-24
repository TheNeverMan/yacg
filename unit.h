#pragma once
#include<string>
#include<iostream>
#include<vector>

#include "help_object.h"
#include "xml_serializable.h"
#include "texture_owner.h"
#include "technology_requirements_owner.h"
#include "traits_owner.h"

using namespace std;

class Unit : public Help_Object, public XML_Serializable, public Texture_Owner, public Technology_Requirements_Owner, public Traits_Owner
{
  private:
    int attack_power;
    int defense_power;
    int movement_points;
    int current_movement_points;
    int hp;
    int cost;
    int maintenance;
    vector<string> Allowed_Tiles;
    string obsolete_unit_name;
  public:
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Unit(xml_node<>* Root_Node);
    bool Is_Naval();
    bool Can_Move_On_Tile_By_Name(string name);
    int Get_Maitenance();
    int Get_Cost();
    int Get_HP();
    int Get_Current_Actions();
    int Get_Max_Actions();
    int Get_Attack_Power();
    int Get_Defense_Power();
    void Remove_All_Movement();
    string Get_Obsolete_Unit_Name();
    bool Has_Full_HP();
    void Heal(int howmuch);
    void Set_HP(int new_hp);
    void Decrease_Movement(int val);
    void Refresh_Movement_Points();
    int Get_Manpower();
    void Reduce_Maitenance_By_One();
    void Increase_Attack_By_One();
    void Increase_Movement_By_One();
    void Increase_Current_Movement(int val);
    vector<string> Get_Allowed_Tiles();
    void Allow_Moving_On_Tile_By_Name(string name);
    Unit(string n, int c, int a, int d, int m, int ma, string i, string r, string t_p, vector<string> a_t, vector<string> t_s, string o);
    Unit();
};
