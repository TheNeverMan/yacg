#include "city.h"

City::City(string n, string f_n, string f_d, int s, array<int, 2> c)
{
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
}

City::City(xml_node<>* Root_Node)
{
  Deserialize(Root_Node);
}

void City::Change_Owner(string new_owner)
{
  owner_name = new_owner;
  stability = stability - 20;
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
  if(stability < 30)
    Status = City_Status::Anarchy;
  if(stability > 50)
    Status = City_Status::Fine;
}

void City::Update_Nationality()
{
  if(nationality != owner_name)
  {
    if(rand() % 100 < 5)
      nationality == owner_name;
  }
}

void City::Process_Passive_Changes(array<int, 2> Capital_Location, bool has_unit, bool is_connected, int stability_techs, int assimilation_techs)
{
  Update_Nationality();
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
  Root_Node->append_attribute(doc->allocate_attribute("name", name.c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("founder_name", founder_name.c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("nationality", nationality.c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("owner_name", owner_name.c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("founding_date", founding_date.c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("stability", to_string(stability).c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("turns_without_stability_changes", to_string(turns_without_stability_changes).c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("turns_without_positive_stability_changes", to_string(turns_without_positive_stability_changes).c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("x", to_string(Coords[0]).c_str()));
  Root_Node->append_attribute(doc->allocate_attribute("y", to_string(Coords[1]).c_str()));
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
