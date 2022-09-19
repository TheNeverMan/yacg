#include "tech.h"

Tech::Tech(string n, int c, vector<string> p, string h_t, string t_p, vector<string> t) : Traits_Owner(t), Help_Object(n, h_t), Texture_Owner(t_p), Technology_Requirements_Owner(p)
{
  cost = c;
  is_researched = false;
  current_cost = cost;
}

void Tech::Research_Tech(int val)
{
  current_cost = current_cost - val;
  if(current_cost <= 0)
    is_researched = true;
}

bool Tech::Is_Reseached()
{
  return is_researched;
}

int Tech::Get_Cost()
{
  return cost;
}

int Tech::Get_Current_Cost()
{
  return current_cost;
}

int Tech::Is_Researched_And_How_Many_Times_Has_Trait_Name(string trait_name)
{
  if(is_researched)
    return How_Many_Times_Has_Trait(trait_name);
  return 0;
}

bool Tech::Is_Researched_And_Has_Trait(string trait_name)
{
  bool out = is_researched && Has_Trait(trait_name);
  return out;
}

Tech::Tech(xml_node<>* Root_Node) : Traits_Owner(Root_Node), Help_Object(Root_Node), Texture_Owner(Root_Node), Technology_Requirements_Owner(Root_Node)
{
  Deserialize(Root_Node);
}

void Tech::Deserialize(xml_node<>* Root_Node)
{
  cost = stoi(Root_Node->first_attribute("cost")->value());
  current_cost = stoi(Root_Node->first_attribute("current_cost")->value());
  is_researched = (bool) stoi(Root_Node->first_attribute("is_researched")->value());
}

xml_node<>* Tech::Serialize(memory_pool<>* doc)
{
  xml_node<> *Root_Node = doc->allocate_node(node_element, "tech");
  xml_attribute<> *Cost = doc->allocate_attribute("cost", doc->allocate_string(to_string(cost).c_str()));
  Root_Node->append_attribute(Cost);
  xml_attribute<> *Current_cost = doc->allocate_attribute("current_cost", doc->allocate_string(to_string(current_cost).c_str()));
  Root_Node->append_attribute(Current_cost);
  xml_attribute<> *Is_Reseached = doc->allocate_attribute("is_researched", doc->allocate_string(to_string(is_researched).c_str()));
  Root_Node->append_attribute(Is_Reseached);
  Root_Node->append_node(Serialize_Help(doc));
  Root_Node->append_node(Serialize_Traits(doc));
  Root_Node->append_node(Serialize_Textures(doc));
  Root_Node->append_node(Serialize_Technologies(doc));
  return Root_Node;
}
