#pragma once
#include<string>
#include<vector>

#include "help_object.h"
#include "xml_serializable.h"
#include "traits_owner.h"
#include "texture_owner.h"
#include "technology_requirements_owner.h"

using std::string;
using std::vector;
using std::string_view;

class Tech : public Help_Object, public Traits_Owner, public Texture_Owner, public Technology_Requirements_Owner
{
  public:
    void Research_Tech(int val);
    bool Is_Reseached() const;
    int Get_Cost() const;
    int Get_Current_Cost() const;
    Tech(string n, int c, vector<string> p, string h_t, string t_p, vector<string> t);
    Tech(xml_node<>* Root_Node);
    bool Is_Researched_And_Has_Trait(string_view trait_name) const;
    int Is_Researched_And_How_Many_Times_Has_Trait_Name(string_view trait_name) const;
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
  private:
    int cost;
    int current_cost;
    bool is_researched;
};
