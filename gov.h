#pragma once
#include<string>

#include "help_object.h"
#include "xml_serializable.h"
#include "texture_owner.h"
#include "technology_requirements_owner.h"
#include "traits_owner.h"

using namespace std;

class Gov : public Help_Object, public XML_Serializable, public Texture_Owner, public Technology_Requirements_Owner, public Traits_Owner
{
  public:
    Gov(string n, string l_t, string s_n, string t_r, string i, string t_p, vector<string> t);
    Gov();
    string Get_Leader_Title();
    string Get_State_Name();
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Gov(xml_node<>* Root_Node);
    ~Gov();
  private:
    string leader_title;
    string state_name;
};
