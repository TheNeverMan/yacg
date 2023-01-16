#include "xml_serializable.h"

/*
XML_Serializable::~XML_Serializable()
{

}
*/

xml_node<>*  XML_Serializable::Serialize(memory_pool<>* doc)
{
  throw;
}

void XML_Serializable::Deserialize(xml_node<>* Root_Node)
{
  throw;
}
xml_node<>* XML_Serializable::Get_Subnode(xml_node<>* Root_Node, string node_name)
{
  xml_node<>* out = Root_Node->first_node(node_name.c_str());
  if(out == nullptr)
    throw nullptr;
  return out;
}

xml_attribute<>* XML_Serializable::Get_Attribute(xml_node<>* XML_Node, string attribute_name)
{
  xml_attribute<>* out = XML_Node->first_attribute(attribute_name.c_str());
  if(out == nullptr)
    throw nullptr;
  return out;
}

string XML_Serializable::Get_Value_From_Attribute(xml_node<>* XML_Node, string attribute_name)
{
  string out = "";
  try
  {
    out = Get_Attribute(XML_Node, attribute_name)->value();
  }
  catch(...)
  {
    throw nullptr;
  }
  return out;
}

int XML_Serializable::Get_Int_Value_From_Attribute(xml_node<>* XML_Node, string attribute_name)
{
  int out = 0;
  try
  {
    out = stoi(Get_Attribute(XML_Node, attribute_name)->value());
  }
  catch(...)
  {
    throw nullptr;
  }
  return out;
}
