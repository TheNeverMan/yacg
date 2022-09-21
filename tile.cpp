#include "tile.h"

#include<iostream>

Tile::Tile(int m_c, string n, string t_p, vector<string> t) : Traits_Owner(t), Help_Object(n, " "), Texture_Owner(t_p)
{
  has_unit = false;
  unit_owner_id = 0;
  movement_cost = m_c;
  upgrade = "none";
  is_plundered = false;
}

void Tile::Upgrade_Tile(string t_u)
{
  upgrade = t_u;
}

Tile::Tile() : Traits_Owner({" "}), Help_Object(" ", " "), Texture_Owner(" ")
{
  vector<string> t;
  has_unit = false;
  upgrade = "none";
  unit_owner_id = 0;
  is_plundered = false;
}

string Tile::Get_Upgrade()
{
  if(is_plundered)
    return "plundered";
  return upgrade;
}

void Tile::Plunder()
{
  is_plundered = true;
}

void Tile::Fix()
{
  is_plundered = false;
}

bool Tile::Has_Unit()
{
  return has_unit;
}

int Tile::Get_Unit_Owner_Id()
{
  return unit_owner_id;
}

void Tile::Buff_Tile()
{
  is_buffed = true;
}

bool Tile::Is_Buffed()
{
  return is_buffed;
}

void Tile::Remove_Unit_From_Tile()
{
  has_unit = false;
  unit_owner_id = 0;
}

void Tile::Put_Unit_On_Tile(int owner)
{
  has_unit = true;
  unit_owner_id = owner;
}

int Tile::Get_Movement_Cost()
{
  return movement_cost;
}

Tile::Tile(xml_node<>* Root_Node) : Traits_Owner(Root_Node), Help_Object(Root_Node), Texture_Owner(Root_Node)
{
  Deserialize(Root_Node);
}

void Tile::Deserialize(xml_node<>* Root_Node)
{
  movement_cost = stoi(Root_Node->first_attribute("cost")->value());
  has_unit = (bool) stoi(Root_Node->first_attribute("has_unit")->value());
  upgrade = Root_Node->first_attribute("upgrade")->value();
  unit_owner_id = stoi(Root_Node->first_attribute("unit_owner_id")->value());
  is_plundered = (bool) stoi(Root_Node->first_attribute("is_plundered")->value());
}

xml_node<>* Tile::Serialize(memory_pool<>* doc)
{
  xml_node<> *Root_Node = doc->allocate_node(node_element, "tile");
  xml_attribute<> *Cost = doc->allocate_attribute("cost", doc->allocate_string(to_string(movement_cost).c_str()));
  Root_Node->append_attribute(Cost);
  xml_attribute<> *Has_Unit = doc->allocate_attribute("has_unit", doc->allocate_string(to_string(has_unit).c_str()));
  Root_Node->append_attribute(Has_Unit);
  xml_attribute<> *Is_Plundered = doc->allocate_attribute("is_plundered", doc->allocate_string(to_string(is_plundered).c_str()));
  Root_Node->append_attribute(Is_Plundered);
  xml_attribute<> *Upgrade_Name = doc->allocate_attribute("upgrade", doc->allocate_string(upgrade.c_str()));
  Root_Node->append_attribute(Upgrade_Name);
  xml_attribute<> *Unit_Owner = doc->allocate_attribute("unit_owner_id", doc->allocate_string(to_string(unit_owner_id).c_str()));
  Root_Node->append_attribute(Unit_Owner);
  Root_Node->append_node(Serialize_Traits(doc));
  Root_Node->append_node(Serialize_Textures(doc));
  Root_Node->append_node(Serialize_Help(doc));
  return Root_Node;
}
