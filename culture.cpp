#include "culture.h"

Culture::Culture() : Help_Object(" ", " ")
{

}

void Culture::Shuffle_Fallback_Lists()
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(Fallback_City_Names.begin(), Fallback_City_Names.end(), std::default_random_engine(seed));
  shuffle(Fallback_Leader_Names.begin(), Fallback_Leader_Names.end(), std::default_random_engine(seed));

}

Culture::Culture(string n, std::vector<string> c, vector<string> l) : Help_Object(n, " "), Fallback_City_Names(c), Fallback_Leader_Names(l)
{
    Shuffle_Fallback_Lists();
    last_given_city_name = "";
    last_give_leader_name = "";
}

Culture::~Culture()
{

}

Culture::Culture(xml_node<>* Root_Node) : Help_Object(Root_Node)
{
  Deserialize(Root_Node);
}

string_view Culture::Get_Random_City_Name()
{
  if(!Fallback_City_Names.size())
    return "City Name";
  last_given_city_name = Fallback_City_Names[0];
  return last_given_city_name;
}

string_view Culture::Get_Random_Leader_Name()
{
  if(!Fallback_Leader_Names.size())
    return "Leader Name";
  last_give_leader_name = Fallback_Leader_Names[0];
  return last_give_leader_name;
}

void Culture::Remove_Last_City_Name()
{
  remove(Fallback_City_Names.begin(), Fallback_City_Names.end(), last_given_city_name);
}

void Culture::Remove_Last_Leader_Name()
{
  remove(Fallback_Leader_Names.begin(), Fallback_Leader_Names.end(), last_give_leader_name);
}

xml_node<>* Culture::Serialize(memory_pool<>* doc)
{
  xml_node<>* Root_Node = doc->allocate_node(node_element, "culture");
  xml_node<>* Fallback_City_Names_Node = doc->allocate_node(node_element, "cities");
  for_each(Fallback_City_Names.begin(), Fallback_City_Names.end(), [&](auto& city_name)
  {
      xml_node<>* City_Node = doc->allocate_node(node_element, "city", doc->allocate_string(city_name.c_str()));
      Fallback_City_Names_Node->append_node(City_Node);
  });
  xml_node<>* Fallback_Leader_Names_Node = doc->allocate_node(node_element, "leaders");
  for_each(Fallback_Leader_Names.begin(), Fallback_Leader_Names.end(), [&](auto& leader_name)
  {
      xml_node<>* Leader_Node = doc->allocate_node(node_element, "leader", doc->allocate_string(leader_name.c_str()));
      Fallback_Leader_Names_Node->append_node(Leader_Node);
  });
  Root_Node->append_node(Fallback_City_Names_Node);
  Root_Node->append_node(Fallback_Leader_Names_Node);
  Root_Node->append_node(Serialize_Help(doc));
  return Root_Node;
}

void Culture::Deserialize(xml_node<>* Root_Node)
{
  xml_node<> *Fallback_City_Names_Node = Get_Subnode(Root_Node, "cities");
  for(xml_node<> *City_Node = Fallback_City_Names_Node->first_node("city"); City_Node; City_Node = City_Node->next_sibling("city"))
  {
    Fallback_City_Names.push_back(City_Node->value());
  }
  xml_node<> *Fallback_Leader_Names_Node = Get_Subnode(Root_Node, "leaders");
  for(xml_node<> *Leader_Node = Fallback_Leader_Names_Node->first_node("leader"); Leader_Node; Leader_Node = Leader_Node->next_sibling("leader"))
  {
    Fallback_Leader_Names.push_back(Leader_Node->value());
  }
  Shuffle_Fallback_Lists();
}

string Culture::Get_Texture_For_Upgrade(string upgrade_name) const
{
  std::transform(upgrade_name.begin(), upgrade_name.end(), upgrade_name.begin(), ::tolower);
  return "assets/textures/upgrades/" + upgrade_name + "/" + Get_Name().data() + "-" + upgrade_name + "-upgrade-texture.svg";
}
