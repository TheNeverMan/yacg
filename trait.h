#pragma once
#include<string>
#include<vector>

#include "xml_serializable.h"
#include "logger.h"

using namespace std;

class Trait
{
  private:
    string trait_name;
    vector<string> Arguments;
  public:
    Trait(string raw_trait);
    Trait(xml_node<>* Root_Node);
    xml_node<>* Serialize_Trait(memory_pool<>* doc);
    string Get_Trait_Name();
    string Get_First_Argument();
    vector<string> Get_All_Arguments();
};
