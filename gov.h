#pragma once
#include<string>

#include "help_object.h"
#include "xml_serializable.h"
#include "texture_owner.h"
#include "technology_requirements_owner.h"
#include "traits_owner.h"

using std::string;
using std::stod;

class Gov : public Help_Object, public XML_Serializable, public Texture_Owner, public Technology_Requirements_Owner, public Traits_Owner
{
  public:
    Gov(string n, string l_t, string s_n, string t_r, string i, string t_p, vector<string> t, int m_s, double pa, double a);
    Gov();
    string Get_Leader_Title();
    string Get_State_Name();
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Gov(xml_node<>* Root_Node);
    ~Gov();
    int Get_Max_Stability();
    double Get_Passive_Stability();
    double Get_Army_Stability();
  private:
    string leader_title;
    string state_name;
    int max_stability;
    double passive_stability;
    double army_stability;
};
