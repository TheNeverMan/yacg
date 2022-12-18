#pragma once
#include<string>
#include<map>
#include<cctype>
#include<random>
#include<vector>
#include<chrono>
#include<algorithm>

#include "xml_serializable.h"
#include "help_object.h"
#include "logger.h"

using std::map;
using std::string;
using std::vector;
using std::shuffle;
using std::remove;

class Culture : public Help_Object
{
  private:
    vector<string> Fallback_City_Names;
    vector<string> Fallback_Leader_Names;
    string last_given_city_name;
    string last_give_leader_name;
    void Shuffle_Fallback_Lists();
  public:
    Culture(string n, vector<string> c, vector<string> l);
    Culture();
    string Get_Texture_For_Upgrade(string upgrade_name);
    string Get_Random_City_Name();
    string Get_Random_Leader_Name();
    void Remove_Last_City_Name();
    void Remove_Last_Leader_Name();
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    Culture(xml_node<>* Root_Node);
    ~Culture();
};
