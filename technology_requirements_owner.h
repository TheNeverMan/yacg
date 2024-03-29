#pragma once
#include<vector>
#include<string>

#include "xml_serializable.h"

using std::string;
using std::vector;
using std::string_view;

class Technology_Requirements_Owner
{
  private:
    vector<string> Required_Technologies;
  public:
    Technology_Requirements_Owner(string t_r);
    Technology_Requirements_Owner(vector<string> t_r);
    Technology_Requirements_Owner(xml_node<>* Root_Node);
    xml_node<>* Serialize_Technologies(memory_pool<>* doc);
    vector<string> Get_All_Requirements() const;
    string_view Get_First_Requirement() const;
};
