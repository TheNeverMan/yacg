#include "city.h"

City::City(string n, string f_n, string f_d, int s, array<int, 2> c)
{
  is_connected = false;
  name = n;
  founder_name = f_n;
  nationality = f_n;
  owner_name = f_n;
  founding_date = f_d;
  stability = s;
  Coords = c;
  Status = City_Status::Fine;
  turns_without_stability_changes = 0;
  turns_without_positive_stability_changes = 0;
  turns_after_revolt = 0;
}

City::City(xml_node<>* Root_Node)
{
  Deserialize(Root_Node);
  turns_after_revolt = 0;
}

void City::Change_Owner(string new_owner)
{
  owner_name = new_owner;
  stability = stability - 7;
  Update_Status();
}

void City::Update_Status()
{
  if(stability < 0)
    stability = 0;
  if(stability > 100)
    stability = 100;
  if(turns_without_positive_stability_changes)
    return;
  if(stability < 50)
    Status = City_Status::Riots;
  if(stability < 25)
    Status = City_Status::Anarchy;
  if(stability > 50)
    Status = City_Status::Fine;
}

void City::Update_Nationality()
{
  if(nationality != owner_name)
  {
    if(rand() % 100 < 15)
      nationality = owner_name;
  }
}

bool City::Does_Have_Increased_Maitenance()
{
  return stability < 50;
}

void City::Process_Passive_Changes(array<int, 2> Capital_Location, bool has_unit, int stability_techs, int assimilation_techs, double base_growth, double army_multiplier, int max_stability)
{
  Update_Nationality();
  double stab_growth = 0;
  double capital_distance = sqrt((abs(pow((Get_Coords()[0] - Capital_Location[0]),2))) + abs((pow(Get_Coords()[1] - Capital_Location[1],2))));
  if(nationality == owner_name)
  {
    stab_growth = (base_growth - (0.12*capital_distance)) * (1 + (0.5 * stability_techs));
    if(has_unit)
      stab_growth = stab_growth + army_multiplier;
    if(is_connected)
    {
      if(stab_growth > 0)
        stab_growth = stab_growth * 2 + 1;
      else
        stab_growth = stab_growth / 2 + 1;
    }
  }
  else
  {
    stab_growth = (base_growth - (0.12*capital_distance)) * ((0.5 * assimilation_techs));
    if(has_unit)
      stab_growth = stab_growth + army_multiplier;
  }
  if(turns_without_stability_changes)
  {
    stability = stability + 3;
    turns_without_stability_changes--;
  }
  if(turns_without_positive_stability_changes)
    turns_without_positive_stability_changes--;
  if(turns_after_revolt)
    turns_after_revolt--;
  if((turns_without_positive_stability_changes && stab_growth < 0))
    return;
  stability = stability + stab_growth;
  if(stability > max_stability)
    stability = max_stability;
  if(stability < 0)
    stability = 0;
}

bool City::Does_Rebel()
{
  if(stability < 25 && rand() % 100 <= 40 && !turns_after_revolt)
    return true;
  return false;
}

string City::Get_Name()
{
  return name;
}

string City::Get_Founder_Name()
{
  return founder_name;
}

string City::Get_Nationality()
{
  return nationality;
}

string City::Get_Owner_Name()
{
  return owner_name;
}

string City::Get_Founding_Date()
{
  return founding_date;
}

int City::Get_Stability()
{
  return stability;
}

string City::Get_Status()
{
  switch (Status)
  {
    case City_Status::Fine:
      return "Fine";
      break;
    case City_Status::Anarchy:
      return "Anarchy";
      break;
    case City_Status::Riots:
      return "Riots";
      break;
    case City_Status::Epidemy:
      return "Epidemy";
      break;
    case City_Status::Drought:
      return "Drought";
      break;
    case City_Status::Flood:
      return "Flood";
      break;
  }
  return "Fine";
}

void City::Change_Stability(int stability_change, bool has_unit)
{
  if(has_unit && stability_change < 0)
    stability_change = stability_change * 0.6;
  stability = stability + stability_change;
  Update_Status();
}

xml_node<>* City::Serialize(memory_pool<>* doc)
{
  xml_node<>* Root_Node = doc->allocate_node(node_element, "city");
  Root_Node->append_attribute(doc->allocate_attribute("name", doc->allocate_string(name.c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("founder_name", doc->allocate_string(founder_name.c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("nationality", doc->allocate_string(nationality.c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("owner_name", doc->allocate_string(owner_name.c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("founding_date", doc->allocate_string(founding_date.c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("stability", doc->allocate_string(to_string(stability).c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("is_connected", doc->allocate_string(to_string(is_connected).c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("turns_without_stability_changes", doc->allocate_string(to_string(turns_without_stability_changes).c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("turns_without_positive_stability_changes", doc->allocate_string(to_string(turns_without_positive_stability_changes).c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("x", doc->allocate_string(to_string(Coords[0]).c_str())));
  Root_Node->append_attribute(doc->allocate_attribute("y", doc->allocate_string(to_string(Coords[1]).c_str())));
  return Root_Node;
}

void City::Deserialize(xml_node<>* Root_Node)
{
  name = Root_Node->first_attribute("name")->value();
  founder_name = Root_Node->first_attribute("founder_name")->value();
  nationality = Root_Node->first_attribute("nationality")->value();
  owner_name = Root_Node->first_attribute("owner_name")->value();
  founding_date = Root_Node->first_attribute("founding_date")->value();
  stability = stoi(Root_Node->first_attribute("stability")->value());
  is_connected = stoi(Root_Node->first_attribute("is_connected")->value());
  turns_without_stability_changes = stoi(Root_Node->first_attribute("turns_without_stability_changes")->value());
  turns_without_positive_stability_changes = stoi(Root_Node->first_attribute("turns_without_positive_stability_changes")->value());
  Coords[0] = stoi(Root_Node->first_attribute("x")->value());
  Coords[1] = stoi(Root_Node->first_attribute("y")->value());
  Update_Status();
}

array<int, 2> City::Get_Coords()
{
  return Coords;
}

void City::Set_Catastrophe(City_Status Type)
{
  if(Status < static_cast<City_Status>(3)) //cant happen if already happening
    return;
  Status = Type;
  stability = stability - 20;
  turns_without_positive_stability_changes = 3;
  Update_Status();
}

void City::Flood()
{
  Set_Catastrophe(City_Status::Flood);
}

void City::Drought()
{
  Set_Catastrophe(City_Status::Drought);
}

void City::Epidemy()
{
  Set_Catastrophe(City_Status::Epidemy);
}

void City::Rebel()
{
  stability = stability + 100;
  turns_without_stability_changes = 5;
  turns_after_revolt = 12;
  Update_Status();
}

void City::Connect_City()
{
  is_connected = true;
}

bool City::Is_Connected()
{
  return is_connected;
}
