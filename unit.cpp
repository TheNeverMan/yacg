#include "unit.h"

Unit::Unit(string n, int c, int a, int d, int m, int ma, string i, string r, string t_p, vector<string> a_t, vector<string> t_s, string o) : Help_Object(n, i), Texture_Owner(t_p), Technology_Requirements_Owner(r), Traits_Owner(t_s)
{
  Allowed_Tiles = a_t;
  cost = c;
  attack_power = a;
  defense_power = d;
  maintenance = ma;
  hp = 100;
  movement_points = m;
  current_movement_points = movement_points;
  obsolete_unit_name = o;
}

Unit::Unit() : Help_Object(" ", " "), Texture_Owner(" "), Technology_Requirements_Owner(" "), Traits_Owner({"z"})
{
    //in god we trust
}

int Unit::Get_Maitenance()
{
  return maintenance;
}

void Unit::Reduce_Maitenance_By_One()
{
  maintenance--;
}

void Unit::Refresh_Movement_Points()
{
  if(Get_All_Arguments_For_Trait("class")[0] == "flying")
  {
    current_movement_points++;
    if(current_movement_points > movement_points)
      current_movement_points = movement_points;
  }
  else
    current_movement_points = movement_points;
}

bool Unit::Can_Move_On_Tile_By_Name(string name)
{
  for(auto &var : Allowed_Tiles)
  {
    if(var == name)
      return true;
  }
  return false;
}

bool Unit::Is_Naval()
{
  return Can_Move_On_Tile_By_Name("Sea");
}

void Unit::Increase_Attack_By_One()
{
  attack_power++;
}

void Unit::Increase_Movement_By_One()
{
  movement_points++;
  current_movement_points++;
}

void Unit::Increase_Current_Movement(int val)
{
  current_movement_points = current_movement_points + val;
}

int Unit::Get_Cost()
{
  return cost;
}

int Unit::Get_HP()
{
  return hp;
}

int Unit::Get_Current_Actions()
{
  return current_movement_points;
}

int Unit::Get_Max_Actions()
{
  return movement_points;
}

void Unit::Decrease_Movement(int val)
{
  current_movement_points = current_movement_points - val;
}

void Unit::Allow_Moving_On_Tile_By_Name(string name)
{
  Allowed_Tiles.push_back(name);
}

int Unit::Get_Attack_Power()
{
  return attack_power;
}

int Unit::Get_Defense_Power()
{
  return defense_power;
}

void Unit::Remove_All_Movement()
{
  current_movement_points = 0;
}

bool Unit::Has_Full_HP()
{
  if(hp == 100)
    return true;
  return false;
}

void Unit::Set_HP(int new_hp)
{
  hp = new_hp;
}

void Unit::Heal(int howmuch = 0)
{
  Decrease_Movement(2);
  hp = hp + 20;
  hp = hp + howmuch;
  hp = hp + How_Many_Times_Has_Trait("healincrease") * 20;
  if(hp > 100)
    hp = 100;
}

int Unit::Get_Manpower()
{
  int out;
  out = (attack_power + defense_power) / 2 * 100;
  out = out * hp / 100;
  return out;
}

Unit::Unit(xml_node<>* Root_Node) : Help_Object(Root_Node), Texture_Owner(Root_Node), Technology_Requirements_Owner(Root_Node), Traits_Owner(Root_Node)
{
  Deserialize(Root_Node);
}

void Unit::Deserialize(xml_node<>* Root_Node)
{
  cost = stoi(Root_Node->first_attribute("cost")->value());
  attack_power = stoi(Root_Node->first_attribute("attack_power")->value());
  defense_power = stoi(Root_Node->first_attribute("defense_power")->value());
  movement_points = stoi(Root_Node->first_attribute("movement")->value());
  current_movement_points = stoi(Root_Node->first_attribute("current_movement")->value());
  obsolete_unit_name = Root_Node->first_attribute("obsolete")->value();
  hp = stoi(Root_Node->first_attribute("hp")->value());
  maintenance = stoi(Root_Node->first_attribute("maitenance")->value());
  xml_node<>* Allowed_Tiles_Node = Root_Node->first_node("tiles");
  for(xml_node<> *Allowed_Tile_Node = Allowed_Tiles_Node->first_node("tile"); Allowed_Tile_Node; Allowed_Tile_Node = Allowed_Tile_Node->next_sibling("tile"))
  {
    Allowed_Tiles.push_back(Allowed_Tile_Node->value());
  }
}

string Unit::Get_Obsolete_Unit_Name()
{
  return obsolete_unit_name;
}

xml_node<>* Unit::Serialize(memory_pool<>* doc)
{
  xml_node<> *Root_Node = doc->allocate_node(node_element, "unit");
  xml_attribute<> *Cost = doc->allocate_attribute("cost", doc->allocate_string(to_string(cost).c_str()));
  Root_Node->append_attribute(Cost);
  xml_attribute<> *Obsolete = doc->allocate_attribute("obsolete", doc->allocate_string(obsolete_unit_name.c_str()));
  Root_Node->append_attribute(Obsolete);
  xml_attribute<> *Maitenance = doc->allocate_attribute("maitenance", doc->allocate_string(to_string(maintenance).c_str()));
  Root_Node->append_attribute(Maitenance);
  xml_attribute<> *Attack = doc->allocate_attribute("attack_power", doc->allocate_string(to_string(attack_power).c_str()));
  Root_Node->append_attribute(Attack);
  xml_attribute<> *Defense = doc->allocate_attribute("defense_power", doc->allocate_string(to_string(defense_power).c_str()));
  Root_Node->append_attribute(Defense);
  xml_attribute<> *Movement = doc->allocate_attribute("movement", doc->allocate_string(to_string(movement_points).c_str()));
  Root_Node->append_attribute(Movement);
  xml_attribute<> *Current_Movement = doc->allocate_attribute("current_movement", doc->allocate_string(to_string(current_movement_points).c_str()));
  Root_Node->append_attribute(Current_Movement);
  xml_attribute<> *HP = doc->allocate_attribute("hp", doc->allocate_string(to_string(hp).c_str()));
  Root_Node->append_attribute(HP);
  xml_node<> *Tiles_Node = doc->allocate_node(node_element, "tiles");

  for_each(Allowed_Tiles.begin(), Allowed_Tiles.end(), [&](string tile)
  {
    xml_node<>* ti_node = doc->allocate_node(node_element, "tile", doc->allocate_string(tile.c_str()));
    Tiles_Node->append_node(ti_node);
  } );


  Root_Node->append_node(Tiles_Node);
  Root_Node->append_node(Serialize_Help(doc));
  Root_Node->append_node(Serialize_Textures(doc));
  Root_Node->append_node(Serialize_Technologies(doc));
  Root_Node->append_node(Serialize_Traits(doc));
  return Root_Node;
}
