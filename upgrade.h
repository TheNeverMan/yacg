#pragma once
#include<string>
#include<iostream>
#include<vector>

#include "help_object.h"
#include "xml_serializable.h"
#include "traits_owner.h"
#include "texture_owner.h"
#include "technology_requirements_owner.h"

using namespace std;

class Upgrade : public Help_Object, public XML_Serializable, public Traits_Owner, public Texture_Owner, public Technology_Requirements_Owner
{
  public:
    Upgrade(int c, int p, int m, string h_t, string n, string r_t, vector<string> a_t, string texture_path, bool avoid, vector<string> t);
    Upgrade(int c, int p, int m, string h_t, string n, string r_t);
    Upgrade(xml_node<>* Root_Node);
    int Get_Production();
    int Get_Maitenance();
    int Get_Cost();
    void Reduce_Cost(int how_much);
    bool Does_Avoid_Same_Type_Upgrades();
    bool Is_Tile_Allowed_By_Name(string tile_name);
    void Decrease_Cost_By_One();
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    void Reduce_Maitenance(int how_much);
    void Increase_Production_By_One();
  private:
    int cost;
    int production;
    int maitenance;
    vector<string> Allowed_Tiles;
    bool does_avoid_same_type_upgrades;
};
