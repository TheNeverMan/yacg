#pragma once
#include<string>
#include<iostream>
#include<vector>

#include "help_object.h"
#include "xml_serializable.h"
#include "traits_owner.h"
#include "texture_owner.h"
#include "technology_requirements_owner.h"

using std::string;
using std::vector;

class Upgrade : public Help_Object, public Traits_Owner, public Texture_Owner, public Technology_Requirements_Owner
{
  public:
    Upgrade(int c, int p, int m, string h_t, string n, string r_t, vector<string> a_t, string texture_path, bool avoid, vector<string> t);
    Upgrade(int c, int p, int m, string h_t, string n, string r_t);
    Upgrade(xml_node<>* Root_Node);
    int Get_Production() const;
    int Get_Maitenance() const;
    int Get_Cost() const;
    void Reduce_Cost(int how_much);
    bool Does_Avoid_Same_Type_Upgrades() const;
    bool Is_Tile_Allowed_By_Name(string_view tile_name) const;
    void Decrease_Cost_By_One();
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    void Reduce_Maitenance(int how_much);
    void Increase_Production_By_One();
    vector<string> Get_Allowed_Tiles() const;
  private:
    int cost;
    int production;
    int maitenance;
    vector<string> Allowed_Tiles;
    bool does_avoid_same_type_upgrades;
};
