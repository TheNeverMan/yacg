#pragma once
#include<string>
#include<vector>

#include "xml_serializable.h"
#include "logger.h"

using std::string;
using std::vector;
using std::string_view;

class Trait : public XML_Serializable
{
  private:
    string trait_name;
    vector<string> Arguments;
  public:
    Trait(string raw_trait);
    Trait(xml_node<>* Root_Node);
    xml_node<>* Serialize_Trait(memory_pool<>* doc);
    string_view Get_Trait_Name() const;
    string_view Get_First_Argument() const;
    vector<string> Get_All_Arguments() const;
};
