#pragma once
#include<vector>
#include<string>
#include<algorithm>

#include "xml_serializable.h"
#include "trait.h"

using std::string;
using std::vector;
using std::string_view;

class Traits_Owner : public XML_Serializable
{
  private:
    vector<Trait> Traits;
  public:
    bool Has_Trait(string_view trait_name) const;
    Traits_Owner(vector<string> t);
    Traits_Owner(xml_node<>* Root_Node);
    xml_node<>* Serialize_Traits(memory_pool<>* doc);
    vector<string> Get_Trait_Names();
    int How_Many_Times_Has_Trait(string_view trait_name) const;
    vector<string> Get_All_Arguments_For_Trait(string_view trait_name) const;
    void Give_Trait(string_view raw_trait);
};
