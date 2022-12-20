#include "gov.h"

Gov::Gov(string n, string l_t, string s_n, string t_r, string i, string t_p, vector<string> t, int m_s, double pa, double a) : Traits_Owner(t), Help_Object(n, i), Texture_Owner(t_p), Technology_Requirements_Owner(t_r)
{
  leader_title = l_t;
  state_name = s_n;
  max_stability = m_s;
  passive_stability = pa;
  army_stability = a;
}

Gov::Gov() : Traits_Owner({" "}), Help_Object(" ", " "), Texture_Owner(), Technology_Requirements_Owner(" ")
{

}

Gov::~Gov()
{
  //scanf("%s\n", );
}

string Gov::Get_Leader_Title()
{
  return leader_title;
}

string Gov::Get_State_Name()
{
  return state_name;
}

int Gov::Get_Max_Stability()
{
  return max_stability;
}

double Gov::Get_Passive_Stability()
{
  return passive_stability;
}

double Gov::Get_Army_Stability()
{
  return army_stability;
}

Gov::Gov(xml_node<>* Root_Node) : Traits_Owner(Root_Node), Help_Object(Root_Node), Texture_Owner(Root_Node), Technology_Requirements_Owner(Root_Node)
{
  Deserialize(Root_Node);
}

void Gov::Deserialize(xml_node<>* Root_Node)
{
  state_name = Root_Node->first_attribute("state_name")->value();
  leader_title = Root_Node->first_attribute("leader_title")->value();
  max_stability = stoi(Root_Node->first_attribute("max_stability")->value());
  passive_stability = stod(Root_Node->first_attribute("passive_stability")->value());
  army_stability = stod(Root_Node->first_attribute("army_stability")->value());
}

xml_node<>* Gov::Serialize(memory_pool<>* doc)
{
  xml_node<> *Root_Node = doc->allocate_node(node_element, "goverment");
  xml_attribute<> *State_Name = doc->allocate_attribute("state_name", state_name.c_str());
  Root_Node->append_attribute(State_Name);
  xml_attribute<> *Leader_Name = doc->allocate_attribute("leader_title", leader_title.c_str());
  Root_Node->append_attribute(Leader_Name);
  Root_Node->append_attribute(doc->allocate_attribute("max_stability", to_string(max_stability).c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("passive_stability", to_string(passive_stability).c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("army_stability", to_string(army_stability).c_str()));
  Root_Node->append_node(Serialize_Help(doc));
  Root_Node->append_node(Serialize_Textures(doc));
  Root_Node->append_node(Serialize_Technologies(doc));
  Root_Node->append_node(Serialize_Traits(doc));
  return Root_Node;
}
