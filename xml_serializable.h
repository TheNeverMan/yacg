#pragma once
#include<algorithm>
#include<string>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;
using std::stoi;

class XML_Serializable
{
  public:
    xml_node<>* Serialize(memory_pool<>* doc);
  protected:
    void Deserialize(xml_node<>* Root_Node);
};
