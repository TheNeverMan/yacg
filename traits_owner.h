#pragma once
#include<vector>
#include<string>
#include<algorithm>

#include "xml_serializable.h"

using namespace std;

class Traits_Owner
{
  private:
    vector<string> Traits;
  public:
    bool Has_Trait(string trait_name);
    Traits_Owner(vector<string> t);
    Traits_Owner(xml_node<>* Root_Node);
    xml_node<>* Serialize_Traits(memory_pool<>* doc);
    vector<string> Get_Traits();
    int How_Many_Times_Has_Trait(string trait_name);
};
