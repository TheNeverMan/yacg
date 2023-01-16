#pragma once
#include<string>
#include<array>
#include<cmath>

#include "xml_serializable.h"
#include "logger.h"
#include "help_object.h"

using std::string;
using std::array;
using std::to_string;
using std::string_view;
using std::sqrt;
using std::abs;
using std::pow;

class City : public XML_Serializable, public Help_Object
{
  private:
    string founder_name;
    string nationality;
    string owner_name;
    string founding_date;
    bool is_connected;
    int stability;
    int turns_without_stability_changes;
    int turns_without_positive_stability_changes;
    int turns_after_revolt;
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
    void Set_Catastrophe(City_Status Type);
  public:
    City(string n, string f_n, string f_d, int s, array<int, 2> c);
    City(xml_node<>* Root_Node);
    xml_node<>* Serialize(memory_pool<>* doc);
    void Deserialize(xml_node<>* Root_Node);
    void Change_Owner(string new_owner);
    void Process_Passive_Changes(array<int, 2> Capital_Location, bool has_unit, int stability_techs, int assimilation_techs, double base_growth, double army_multiplier, int max_stability);
    bool Does_Rebel() const;
    string_view Get_Founder_Name() const;
    string_view Get_Nationality() const;
    string_view Get_Owner_Name() const;
    string_view Get_Founding_Date() const;
    int Get_Stability() const;
    string_view Get_Status() const;
    void Change_Stability(int stability_change, bool has_unit);
    array<int, 2> Get_Coords() const;
    bool Does_Have_Increased_Maitenance();
    void Flood();
    void Drought();
    void Epidemy();
    void Rebel();
    void Connect_City();
    bool Is_Connected() const;
};
