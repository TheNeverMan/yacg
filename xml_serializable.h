#pragma once
#include<algorithm>
#include<string>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;
using std::stoi;
using std::string;

class XML_Serializable
{
  public:
    xml_node<>* Serialize(memory_pool<>* doc);
  protected:
    void Deserialize(xml_node<>* Root_Node);
    xml_node<>* Get_Subnode(xml_node<>* Root_Node, string node_name);
    xml_attribute<>* Get_Attribute(xml_node<>* XML_Node, string attribute_name);
    string Get_Value_From_Attribute(xml_node<>* XML_Node, string attribute_name);
    int Get_Int_Value_From_Attribute(xml_node<>* XML_Node, string attribute_name);
};
