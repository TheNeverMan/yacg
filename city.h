#pragma once
#include<string>
#include<array>

#include "xml_serializable.h"

using std::string;
using std::array;
using std::to_string;

class City : public XML_Serializable
{
  private:
    string name;
    string founder_name;
    string nationality;
    string owner_name;
    string founding_date;
    int stability;
    int turns_without_stability_changes;
    int turns_without_positive_stability_changes;
    array<int, 2> Coords;
    enum class City_Status
    {
      Fine = 5,
      Anarchy = 4,
      Riots = 3,
      Epidemy = 2,
      Drought = 1,
      Flood = 0,
    };
    City_Status Status;
    void Update_Status();
    void Update_Nationality();
  public:
    City(string n, string f_n, string f_d, int s, array<int, 2> c);
    City(xml_node<>* Root_Node);
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    void Change_Owner(string new_owner);
    void Process_Passive_Changes(array<int, 2> Capital_Location, bool has_unit, bool is_connected, int stability_techs, int assimilation_techs);
    string Get_Name();
    string Get_Founder_Name();
    string Get_Nationality();
    string Get_Owner_Name();
    string Get_Founding_Date();
    int Get_Stability();
    string Get_Status();
    void Change_Stability(int stability_change, bool has_unit);
    array<int, 2> Get_Coords();
};
