#pragma once
#include<vector>
#include<string>
#include<algorithm>

#include "xml_serializable.h"
#include "trait.h"

using namespace std;

class Traits_Owner
{
  private:
    vector<Trait> Traits;
  public:
    bool Has_Trait(string trait_name);
    Traits_Owner(vector<string> t);
    Traits_Owner(xml_node<>* Root_Node);
    xml_node<>* Serialize_Traits(memory_pool<>* doc);
    vector<string> Get_Trait_Names();
    int How_Many_Times_Has_Trait(string trait_name);
    vector<string> Get_All_Arguments_For_Trait(string trait_name);
    void Give_Trait(string raw_trait);
};
