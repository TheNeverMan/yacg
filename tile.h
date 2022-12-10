#pragma once
#include<string>
#include<vector>
#include<algorithm>

#include "help_object.h"
#include "unit.h"
#include "xml_serializable.h"
#include "traits_owner.h"
#include "texture_owner.h"

using std::string;
using std::vector;

class Tile : public Help_Object, public XML_Serializable, public Traits_Owner, public Texture_Owner
{
  private:
    int movement_cost;
    string name;
    string upgrade;
    bool has_unit;
    int is_buffed = 0;
    bool is_plundered;
    int unit_owner_id;
  public:
    Tile(int m_c,string n, string t_p, vector<string> t);
    Tile();
    Tile(xml_node<>* Root_Node);
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    void Upgrade_Tile(string t_u);
    int Get_Unit_Owner_Id();
    string Get_Upgrade();
    bool Has_Unit();
    void Buff_Tile();
    void Debuff_Tile();
    bool Is_Buffed();
    void Put_Unit_On_Tile(int owner);
    void Remove_Unit_From_Tile();
    int Get_Movement_Cost();
    void Plunder();
    void Fix();
};
