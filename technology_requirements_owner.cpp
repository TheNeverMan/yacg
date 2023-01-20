#include "technology_requirements_owner.h"

Technology_Requirements_Owner::Technology_Requirements_Owner(string t_r)
{
  Required_Technologies.push_back(t_r);
}

Technology_Requirements_Owner::Technology_Requirements_Owner(vector<string> t_r)
{
  Required_Technologies = t_r;
}

Technology_Requirements_Owner::Technology_Requirements_Owner(xml_node<>* Root_Node)
{
  xml_node<>* Technology_Requirements_Node = Root_Node->first_node("technology_requirements");
  for(xml_node<> *Requirement_Node = Technology_Requirements_Node->first_node("tech"); Requirement_Node; Requirement_Node = Requirement_Node->next_sibling("tech"))
  {
    Required_Technologies.push_back(Requirement_Node->value());
  }
}

xml_node<>* Technology_Requirements_Owner::Serialize_Technologies(memory_pool<>* doc)
{
  xml_node<> *Technology_Requirements_Node = doc->allocate_node(node_element, "technology_requirements");

  for_each(Required_Technologies.begin(), Required_Technologies.end(), [&](string tech)
  {
    xml_node<>* req_node = doc->allocate_node(node_element, "tech", doc->allocate_string(tech.c_str()));
    Technology_Requirements_Node->append_node(req_node);
  } );
  return Technology_Requirements_Node;
}

vector<string> Technology_Requirements_Owner::Get_All_Requirements() const
{
  return Required_Technologies;
}

string_view Technology_Requirements_Owner::Get_First_Requirement() const
{
  return Required_Technologies[0];
}
