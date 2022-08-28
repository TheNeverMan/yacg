#pragma once
#include<string>
#include<vector>
#include<algorithm>

#include "help_object.h"
#include "upgrade.h"
#include "unit.h"
#include "xml_serializable.h"
#include "traits_owner.h"
#include "texture_owner.h"

using namespace std;

enum Tile_Upgrade
{
  Farm,Mine,Road,Boat,City,None
};

class Tile : public Help_Object, public XML_Serializable, public Traits_Owner, public Texture_Owner
{
  private:
    int movement_cost;
    string name;
    Upgrade Tile_Upgrade;
    bool has_unit;
    bool is_buffed = false;
    int unit_owner_id;
  public:
    Tile(int m_c,string n, string t_p, vector<string> t);
    Tile();
    Tile(xml_node<>* Root_Node);
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Upgrade Get_Upgrade();
    void Upgrade_Tile(Upgrade t_u);
    string Get_Upgrade_Texture_Path();
    vector<string> Get_Textures_Path();
    int Get_Unit_Owner_Id();
    bool Has_Unit();
    void Buff_Tile();
    bool Is_Buffed();
    void Put_Unit_On_Tile(int owner);
    void Remove_Unit_From_Tile();
    int Get_Movement_Cost();
};
