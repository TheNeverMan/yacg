#pragma once
#include<string>

#include "xml_serializable.h"

using std::string;
using std::string_view;

class Help_Object
{
  private:
    string name;
    string help_text;
  public:
    string_view Info() const;
    string_view Get_Name() const;
    xml_node<>* Serialize_Help(memory_pool<>* doc);
    Help_Object(xml_node<>* Root_Node);
    Help_Object(string n, string h_t);
};
