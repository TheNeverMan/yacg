#pragma once
#include<string>

#include "xml_serializable.h"

using std::string;

class Help_Object
{
  private:
    string name;
    string help_text;
  public:
    string Info();
    string Get_Name();
    xml_node<>* Serialize_Help(memory_pool<>* doc);
    Help_Object(xml_node<>* Root_Node);
    Help_Object(string n, string h_t);
};
