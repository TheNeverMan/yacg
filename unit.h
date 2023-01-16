#pragma once
#include<string>
#include<iostream>
#include<vector>

#include "help_object.h"
#include "xml_serializable.h"
#include "texture_owner.h"
#include "technology_requirements_owner.h"
#include "traits_owner.h"
#include "audio_owner.h"

using std::string;
using std::vector;
using std::string_view;

class Unit : public Help_Object, public Texture_Owner, public Technology_Requirements_Owner, public Traits_Owner, public Audio_Owner
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
    bool Is_Naval() const;
    bool Can_Move_On_Tile_By_Name(string_view name) const;
    int Get_Maitenance() const;
    int Get_Cost() const;
    int Get_HP() const;
    int Get_Current_Actions() const;
    int Get_Max_Actions() const;
    int Get_Attack_Power() const;
    int Get_Defense_Power() const;
    void Remove_All_Movement();
    string_view Get_Obsolete_Unit_Name() const;
    bool Has_Full_HP() const;
    void Heal(int howmuch);
    void Set_HP(int new_hp);
    void Decrease_Movement(int val);
    void Refresh_Movement_Points();
    int Get_Manpower() const;
    void Reduce_Maitenance_By_One();
    void Increase_Attack_By_One();
    void Increase_Movement_By_One();
    void Increase_Current_Movement(int val);
    vector<string> Get_Allowed_Tiles() const;
    void Allow_Moving_On_Tile_By_Name(string_view name);
    Unit(string n, int c, int a, int d, int m, int ma, string i, string r, string t_p, vector<string> a_t, vector<string> t_s, string o, string s_p);
    Unit();
};
