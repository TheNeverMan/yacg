#pragma once
#include<string>
#include<map>
#include<cctype>

#include "xml_serializable.h"
#include "help_object.h"
#include "logger.h"

using std::map;
using std::string;

class Culture : public Help_Object
{
  private:
    map<string, string> Upgrade_Texture_Replacements;
  public:
    Culture(string n);
    Culture();
    string Get_Texture_For_Upgrade(string upgrade_name);
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Culture(xml_node<>* Root_Node);
    ~Culture();
};
