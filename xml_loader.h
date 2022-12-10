#pragma once
#include<string>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace rapidxml;
using std::string;

class XML_Loader
{
  public:
    static xml_node<>* Get_Subnode(xml_node<>* Root_Node, string node_name)
    {
      xml_node<>* out = Root_Node->first_node(node_name.c_str());
      if(out == nullptr)
        throw nullptr;
      return out;
    }
};
