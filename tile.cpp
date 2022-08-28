#include "tile.h"

#include<iostream>

Tile::Tile(int m_c, string n, string t_p, vector<string> t) : Tile_Upgrade(0,0,0," ", "none", "Dummy"), Traits_Owner(t), Help_Object(n, " "), Texture_Owner(t_p)
{
  has_unit = false;
  unit_owner_id = 0;
  movement_cost = m_c;
}

void Tile::Upgrade_Tile(Upgrade t_u)
{
  Tile_Upgrade = t_u;
}

Tile::Tile() : Tile_Upgrade(0,0,0," ", "none", "Dummy"), Traits_Owner({" "}), Help_Object(" ", " "), Texture_Owner(" ")
{
  vector<string> t;
  has_unit = false;
  unit_owner_id = 0;
}

Upgrade Tile::Get_Upgrade()
{
  return Tile_Upgrade;
}

string Tile::Get_Upgrade_Texture_Path()
{
  return Tile_Upgrade.Get_Texture_Path();
}

vector<string> Tile::Get_Textures_Path()
{
  vector<string> out;
  out.push_back(Get_Texture_Path());
  out.push_back(Get_Upgrade_Texture_Path());
  return out;
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

Tile::Tile(xml_node<>* Root_Node) : Tile_Upgrade(Root_Node->first_node("upgrade")), Traits_Owner(Root_Node), Help_Object(Root_Node), Texture_Owner(Root_Node)
{
  Deserialize(Root_Node);
}

void Tile::Deserialize(xml_node<>* Root_Node)
{
  movement_cost = stoi(Root_Node->first_attribute("cost")->value());
  has_unit = (bool) stoi(Root_Node->first_attribute("has_unit")->value());
  is_buffed = (bool) stoi(Root_Node->first_attribute("is_buffed")->value());
  unit_owner_id = stoi(Root_Node->first_attribute("unit_owner_id")->value());
}


xml_node<>* Tile::Serialize(memory_pool<>* doc)
{
  xml_node<> *Root_Node = doc->allocate_node(node_element, "tile");
  xml_attribute<> *Cost = doc->allocate_attribute("cost", doc->allocate_string(to_string(movement_cost).c_str()));
  Root_Node->append_attribute(Cost);
  xml_attribute<> *Has_Unit = doc->allocate_attribute("has_unit", doc->allocate_string(to_string(has_unit).c_str()));
  Root_Node->append_attribute(Has_Unit);
  xml_attribute<> *Is_Buffed = doc->allocate_attribute("is_buffed", doc->allocate_string(to_string(is_buffed).c_str()));
  Root_Node->append_attribute(Is_Buffed);
  xml_attribute<> *Unit_Owner = doc->allocate_attribute("unit_owner_id", doc->allocate_string(to_string(unit_owner_id).c_str()));
  Root_Node->append_attribute(Unit_Owner);

  xml_node<> *Upgrade_Node = Tile_Upgrade.Serialize(doc);

  Root_Node->append_node(Serialize_Traits(doc));
  Root_Node->append_node(Serialize_Textures(doc));
  Root_Node->append_node(Upgrade_Node);
  Root_Node->append_node(Serialize_Help(doc));
  return Root_Node;
}
