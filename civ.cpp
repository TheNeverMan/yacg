#include "civ.h"

Civ::Civ(string n, vector<string> l, string c_i, vector<string> c_n, vector<Tech> t_t, vector<Unit> u_t, int r, int g, int b, vector<string> t, vector<Gov> go, map<string, vector<string>> g_n_r, string p, vector<Upgrade> us, string t_p, string a) : Traits_Owner(t), Help_Object(n,c_i), Texture_Owner(t_p), Audio_Owner(a)
{
  srand(time(NULL));
  Goverment_Name_Replacements = g_n_r;
  Goverments = go;
  Active_Goverment = Goverments[0];
  city_name_index = 0;
  id = 0;
  gold = 45;
  research_percent = 50;
  Leaders = l;
  int leader_id = rand() % Leaders.size();
  leader = Leaders[leader_id];
  Leaders.erase(Leaders.begin() + leader_id);
  max_actions = 0;
  Upgrades = us;
  current_actions = 2;
  points_from_technologies = 0;
  City_Names = c_n;
  Tech_Tree = t_t;
  ////cout << "cos" << endl;
  tech_in_research = "Agriculture";
  Unit_Templates = u_t;
  int32_t red = r;
  int32_t green = g;
  int32_t blue = b;
  int32_t alpha = 255;
  alpha = alpha | (red << 24);
  alpha = alpha | (green << 16);
  alpha = alpha | (blue << 8);
  color = alpha;
  personality = p;
  Do_Traits();
  //start game with Agriculture researched
  for(auto &val : Tech_Tree)
  {
    if(val.Get_Name() == tech_in_research)
      val.Research_Tech(9999);
  }
}

string Civ::Get_Leader_Title()
{
  if(Goverment_Name_Replacements.count(Active_Goverment.Get_Name()))
    return Goverment_Name_Replacements[Active_Goverment.Get_Name()][0];
  else
    return Active_Goverment.Get_Leader_Title();
}

string Civ::Get_State_Name()
{
  if(Goverment_Name_Replacements.count(Active_Goverment.Get_Name()))
    return Goverment_Name_Replacements[Active_Goverment.Get_Name()][1];
  else
    return Active_Goverment.Get_State_Name();
}

string Civ::Get_Full_Name()
{
  string out = Get_State_Name() + " " + Get_Name();
  return out;
}

string Civ::Get_Leader_Name()
{
  string out = Get_Leader_Title() + " " + leader;
  return out;
}

string Civ::Get_Personality()
{
  return personality;
}

void Civ::Assign_Id(int i)
{
  id = i;
}

void Civ::Give_One_Gold()
{
  gold++;
}

int Civ::Get_Id()
{
  return id;
}

void Civ::Build_City_On_Map(int x, int y)
{
  Owned_City tmp;
  //max_actions++;
  tmp.Coordinates.x = x;
  tmp.Coordinates.y = y;
  tmp.name = City_Names[city_name_index % City_Names.size()];
  tmp.is_expanded = false;
  Cities.push_back(tmp);
  city_name_index++;
}

void Civ::Build_City_On_Map_With_Name(int x, int y, string name)
{
  Owned_City tmp;
  max_actions++;
  tmp.Coordinates.x = x;
  tmp.Coordinates.y = y;
  tmp.name = name;
  Cities.push_back(tmp);
}

string Civ::Get_City_Name_By_Coordinates(int x, int y)
{
  string out = " ";
  for(auto &city : Cities)
  {
    if(city.Coordinates.x == x && city.Coordinates.y == y)
      out = city.name;
  }
  return out;
}

array<int, 2> Civ::Get_Capital_Location()
{
  array<int, 2> out;
  out[0] = Cities[0].Coordinates.x;
  out[1] = Cities[0].Coordinates.y;
  return out;
}

Upgrade Civ::Find_Upgrade_By_Name(string upg_name)
{
  auto it = find_if(Upgrades.begin(), Upgrades.end(), [&upg_name](Upgrade& upg)
  {
    if(upg.Get_Name() == upg_name)
      return true;
    return false;
  });
  if(it != Upgrades.end())
    return *it;
  else
    Logger::Log_Error("Upgrade not found! " + upg_name);
  throw;
}

void Civ::Build_Upgrade(string upg_name)
{
  int cost = Find_Upgrade_By_Name(upg_name).Get_Cost();
  max_actions = max_actions + Find_Upgrade_By_Name(upg_name).How_Many_Times_Has_Trait("giveaction");
  cost = cost - How_Many_Times_Has_Trait("E");
  if(Active_Goverment.Get_Name() == "Republic")
    cost--;
  if(Active_Goverment.Get_Name() == "Communism")
    cost++;
  if(upg_name == "City" && Get_Active_Goverment_Name() == "Theocracy")
  {
    cost = cost - 10;
  }
  if(upg_name == "Farm" && Get_Active_Goverment_Name() == "Monarchy")
  {
    cost = cost + 2;
  }
  gold = gold - cost;
  current_actions--;
}

Unit Civ::Get_Unit_On_Tile(int x, int y)
{
  for(auto &unit : Units_Owned)
  {
    if(unit.Coordinates.x == x && unit.Coordinates.y == y)
      return unit.Self;
  }
  Logger::Log_Error("Unit not found!");
  throw;
}

Unit* Civ::Get_Unit_On_Tile_Pointer(int x, int y)
{
  for(auto &unit : Units_Owned)
  {
    if(unit.Coordinates.x == x && unit.Coordinates.y == y)
      return &unit.Self;
  }
  Logger::Log_Error("Unit not found!");
  throw;
}


bool Civ::Has_Unit_On_Tile(int x, int y)
{
  for(auto &unit : Units_Owned)
  {
    if(unit.Coordinates.x == x && unit.Coordinates.y == y)
      return true;
  }
  return false;
}

int Civ::Get_Number_Of_Cities_Owned()
{
  return Cities.size();
}

void Civ::Remove_Unit_By_Coords(int x, int y)
{
  int index = 0;
  for(auto &unit : Units_Owned)
  {
    if(unit.Coordinates.x == x && unit.Coordinates.y == y)
      Units_Owned.erase(Units_Owned.begin() + index);
    index++;
  }
  //fuuuck;
}

int Civ::Get_Upgrade_Production_By_Name(string upg_name)
{
  int out = Find_Upgrade_By_Name(upg_name).Get_Production();
  if(upg_name == "Farm")
  {
    out = out + How_Many_Times_Has_Trait("A");
    if(Get_Active_Goverment_Name() == "Monarchy")
      out++;
  }
  return out;
}

int Civ::Get_Upgrade_Maitenance_By_Name(string upg_name)
{
  int out = Find_Upgrade_By_Name(upg_name).Get_Maitenance();
  if(upg_name == "Farm")
  {
    if(upg_name == "City" && Get_Active_Goverment_Name() == "Theocracy")
    {
      out = out + 2;
    }
  }
  return out;
}

int Civ::Get_Upgrade_Buff_By_Name(string upg_name)
{
  int out = Find_Upgrade_By_Name(upg_name).How_Many_Times_Has_Trait("economybonus");
  out = out * (How_Many_Times_Has_Trait("O") + 1);
  if(Get_Active_Goverment_Name() == "Democracy")
  {
    out = out * 2;
  }
  return out;
}

double Civ::Get_Defense_Bonus_For_Upgrade(string upg_name)
{
  double out = 1.0;
  out = out + (static_cast<double>(Find_Upgrade_By_Name(upg_name).How_Many_Times_Has_Trait("minordefbonus")) / 10.0);
  out = out + (static_cast<double>(Find_Upgrade_By_Name(upg_name).How_Many_Times_Has_Trait("minordefbonus") * 2) / 10.0);
  out = out - (static_cast<double>(Find_Upgrade_By_Name(upg_name).How_Many_Times_Has_Trait("fightdebuff") * 3) / 10.0);
  return out;
}

void Civ::Calculate_Score()
{
  points = 0;
  points = points + points_from_technologies;
  points = Cities.size() * 10;
  points = points + Calculate_Unit_Maitenance();
  for(auto &tech : Tech_Tree)
  {
    if(tech.Is_Reseached())
      points = points + 5;
  }
}

int Civ::Get_Score()
{
  Calculate_Score();
  return points;
}

int Civ::Get_Gold()
{
  return gold;
}

int Civ::Get_Max_Actions()
{
  return max_actions;
}

int Civ::Get_Current_Actions()
{
  return current_actions;
}

Tech* Civ::Get_Currently_Researched_Tech()
{
  //Set_Research_Tech_By_Name("s");
  ////cout << "tech ine reseasrh " << tech_in_research << endl;
  for(auto &tech : Tech_Tree)
  {
    if(tech.Get_Name() == tech_in_research)
      return &tech;
  }
  Logger::Log_Error("No Tech in research!");
  throw;
}

int Civ::Calculate_Unit_Maitenance()
{
  int out = 0;
  for(auto &unit : Units_Owned)
  {
    out = out + unit.Self.Get_Maitenance();
  }
  if(Active_Goverment.Get_Name() == "Dictatorship")
    out = out * 0.75;
  if(Active_Goverment.Get_Name() == "Democracy" || Get_Active_Goverment_Name() == "Parliamentary Monarchy")
    out = out * 1.25;
  return out;
}

int Civ::Get_Research_Percent()
{
  return research_percent;
}

void Civ::Do_Traits_Of_Researched_Tech()
{
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increaseborder"))
    recent_expand = true;
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("embarkment"))
  {
    for(auto &unit : Unit_Templates)
    {
      if(!unit.Can_Move_On_Tile_By_Name("Sea"))
        unit.Allow_Moving_On_Tile_By_Name("Sea");
    }
    for(auto &unit : Units_Owned)
    {
      if(!unit.Self.Can_Move_On_Tile_By_Name("Sea"))
        unit.Self.Allow_Moving_On_Tile_By_Name("Sea");
    }
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increaseresearchfunds"))
    tech_money_modifier = tech_money_modifier + 0.05;
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("givepoints"))
    points_from_technologies = points_from_technologies + stoi(Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("givepoints")[0]);
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("reduceallupgradescost"))
    for_each(Upgrades.begin(), Upgrades.end(), [](Upgrade& u){u.Reduce_Cost(1);});
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("giveallunits"))
  {
    string trait = Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("giveallunits")[0];
    for_each(Unit_Templates.begin(), Unit_Templates.end(), [&trait](Unit& u){u.Give_Trait(trait);});
    for_each(Units_Owned.begin(), Units_Owned.end(), [&trait](Unit_On_Map& u){u.Self.Give_Trait(trait);});
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("reduceallunitsmaitenancecost"))
  {
    for_each(Unit_Templates.begin(), Unit_Templates.end(), [](Unit& u){u.Reduce_Maitenance_By_One();});
    for_each(Units_Owned.begin(), Units_Owned.end(), [](Unit_On_Map& u){u.Self.Reduce_Maitenance_By_One();});
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increaseallunitsmovementspeed"))
  {
    for_each(Unit_Templates.begin(), Unit_Templates.end(), [](Unit& u){u.Increase_Movement_By_One();});
    for_each(Units_Owned.begin(), Units_Owned.end(), [](Unit_On_Map& u){u.Self.Increase_Movement_By_One();});
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increasenavalunitsmovement"))
  {
    std::vector<Unit>::iterator iter = Unit_Templates.begin();
    while ((iter = std::find_if(iter, Unit_Templates.end(), [](Unit& u){return u.Get_All_Arguments_For_Trait("class")[0] == "naval";})) != Unit_Templates.end())
    {
      iter->Increase_Movement_By_One();
      iter->Increase_Movement_By_One();
      iter++;
    }
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("reduceupgradebuildcost"))
  {
    string name = Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("reduceupgradebuildcost")[0];
    find_if(Upgrades.begin(), Upgrades.end(), [&name](Upgrade& u){return name == u.Get_Name();})->Reduce_Cost(Get_Currently_Researched_Tech()->How_Many_Times_Has_Trait("reduceupgradebuildcost"));
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("reduceupgrademaitenancecost"))
  {
    string name = Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("reduceupgrademaitenancecost")[0];
    find_if(Upgrades.begin(), Upgrades.end(), [&name](Upgrade& u){return name == u.Get_Name();})->Reduce_Maitenance(Get_Currently_Researched_Tech()->How_Many_Times_Has_Trait("reduceupgrademaitenancecost"));
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increaseupgradeproduction"))
  {
    string name = Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("increaseupgradeproduction")[0];
    find_if(Upgrades.begin(), Upgrades.end(), [&name](Upgrade& u){return name == u.Get_Name();})->Increase_Production_By_One();
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increaseunitmovementspeed"))
  {
    for_each(Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("increaseunitmovementspeed").begin(), Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("increaseunitmovementspeed").end(), [&](string& unit_name)
    {
      find_if(Unit_Templates.begin(), Unit_Templates.end(), [unit_name](Unit& u){return u.Get_Name() == unit_name;})->Increase_Movement_By_One();
      find_if(Units_Owned.begin(), Units_Owned.end(), [unit_name](Unit_On_Map& u){return u.Self.Get_Name() == unit_name;})->Self.Increase_Movement_By_One();
    });
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("giveupgradetrait"))
  {
    string name = Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("giveupgradetrait")[0];
    string trait = Get_Currently_Researched_Tech()->Get_All_Arguments_For_Trait("giveupgradetrait")[1];
    find_if(Upgrades.begin(), Upgrades.end(), [&name](Upgrade& u){return u.Get_Name() == name;})->Give_Trait(trait);
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increasenavalhealrate"))
  {
    std::vector<Unit>::iterator iter = Unit_Templates.begin();
    while ((iter = std::find_if(iter, Unit_Templates.end(), [](Unit& u){return u.Get_All_Arguments_For_Trait("class")[0] == "naval";})) != Unit_Templates.end())
    {
      iter->Give_Trait("healincrease");
      iter++;
    }
  }
  if(Get_Currently_Researched_Tech()->Is_Researched_And_Has_Trait("increaselandhealrate"))
  {
    std::vector<Unit>::iterator iter = Unit_Templates.begin();
    while ((iter = std::find_if(iter, Unit_Templates.end(), [](Unit& u){return u.Get_All_Arguments_For_Trait("class")[0] != "naval";})) != Unit_Templates.end())
    {
      iter->Give_Trait("healincrease");
      iter++;
    }
  }
}

void Civ::End_Turn(vector<int> income)
{
  int plus = income[0];
  plus = plus - Calculate_Unit_Maitenance();
  int minus = income[1];
  if(Active_Goverment.Get_Name() == "Communism")
  {
    minus = minus * 0.5;
  }
  plus = plus - minus;
  if(plus > 0)
  {
    double research_fund = 0;
    double research_double_percent = 0;
    research_double_percent = (double)research_percent / 100;
    research_fund = (double)plus * research_double_percent;
    if(Active_Goverment.Get_Name() == "Dictatorship")
      research_fund = (double) research_fund * (double) 0.80;
    plus = plus - research_fund;
    Get_Currently_Researched_Tech()->Research_Tech((int)research_fund * tech_money_modifier);
    Do_Traits_Of_Researched_Tech();
    gold = gold + plus;
  }
  else
  {
    gold = gold + plus;
  }
}

int Civ::Get_Unit_Maitenance()
{
  return Calculate_Unit_Maitenance();
}

void Civ::Refresh_Unit_Movement_Points()
{
  for(auto &unit : Units_Owned)
  {
    unit.Self.Refresh_Movement_Points();
    if(Get_Active_Goverment_Name() == "Horder")
      unit.Self.Increase_Current_Movement(1);
  }
}

void Civ::Start_Turn()
{
  current_actions = max_actions;
}

bool Civ::Is_Unit_Unlocked(string unit_name)
{
  if(unit_name == " ")
    return false;
  Unit u = *find_if(Unit_Templates.begin(), Unit_Templates.end(), [&unit_name](Unit& i_u){return unit_name == i_u.Get_Name();});
  return Has_Tech_Been_Researched_By_Name(u.Get_First_Requirement());
}

vector<Upgrade>* Civ::Get_Upgrades()
{
  return &Upgrades;
}

bool Civ::Is_Unit_Obsolete(string unit_name)
{
  Unit u = *find_if(Unit_Templates.begin(), Unit_Templates.end(), [&unit_name](Unit& i_u){return unit_name == i_u.Get_Name();});
  return Is_Unit_Unlocked(u.Get_Obsolete_Unit_Name());
}

bool Civ::Has_Tech_Been_Researched_By_Name(string tech_name)
{
  bool out = false;
  for(auto &tech : Tech_Tree)
  {
    if(tech.Get_Name() == tech_name && tech.Is_Reseached())
      out = true;
  }
  return out;
}

vector<Tech> Civ::Get_Possible_Research_Techs()
{
  vector<Tech> out;
  for(auto &tech : Tech_Tree)
  {
    bool has_all_requirements = true;
    vector<string> requirements = tech.Get_All_Requirements();
    for(string &tech : requirements)
    {
      if(!Has_Tech_Been_Researched_By_Name(tech))
        has_all_requirements = false;
    }
    if(has_all_requirements && !Has_Tech_Been_Researched_By_Name(tech.Get_Name()))
    {
      out.push_back(tech);
    }
  }
  return out;
}

vector<Gov> Civ::Get_Possible_Goverments()
{
  vector<Gov> out;
  for(auto &var : Goverments)
  {
    if(Has_Tech_Been_Researched_By_Name(var.Get_First_Requirement()))
      out.push_back(var);
  }
  return out;
}

void Civ::Set_Research_Tech_By_Name(string tech_name)
{
  tech_in_research = tech_name;
  if(Active_Goverment.Get_Name() == "Republic")
    current_actions--;
//  //cout << "tech: " << tech_in_research << endl;
}

void Civ::Set_Research_Funds_Percentage(int new_val)
{
  research_percent = new_val;
}

int32_t Civ::Get_Civ_Color()
{
  return color;
}

void Civ::Do_Trait(string trait_name)
{
  if(trait_name == "M")
  {
    for(auto &var : Unit_Templates)
    {
      var.Increase_Attack_By_One();
    }
  }

  if(trait_name == "N")
  {
    for(auto &var : Unit_Templates)
    {
      if(var.Is_Naval())
      {
        var.Increase_Movement_By_One();
        var.Increase_Movement_By_One();
      }
    }
  }
  //E done in place

  if(trait_name == "S")
  {
    tech_money_modifier = tech_money_modifier + 0.10;
  }
  if(trait_name == "R")
  {
    current_actions++;
    max_actions++;
  }

  if(trait_name == "D")
  {
    for(auto &var : Unit_Templates)
      var.Increase_Movement_By_One();
  }

  if(trait_name == "V")
  {
    for(auto& var : Unit_Templates)
    {
      if(var.Get_All_Arguments_For_Trait("class")[0] == "infantry")
      {
        var.Allow_Moving_On_Tile_By_Name("Ice");
        var.Allow_Moving_On_Tile_By_Name("Desert");
      }
    }
  }

}

bool Civ::Has_Enough_Gold_To_Build_Upgrade(string upg_name)
{
  for(auto& var : Upgrades)
  {
    if(var.Get_Name() == upg_name)
    {
      if(var.Get_Cost() <= gold)
        return true;
      return false;
    }
  }
  Logger::Log_Error("Upgrade not found! " + upg_name);
  return false;
}

void Civ::Do_Traits()
{
  for(auto &var : Get_Trait_Names())
  {
    Do_Trait(var);
  }
}

int Civ::Get_Upgrade_Border_Radius()
{
  int out = 2;
  for(auto &var : Tech_Tree)
  {
    out = out + var.Is_Researched_And_How_Many_Times_Has_Trait_Name("increaseborder");
  }
  out = out + How_Many_Times_Has_Trait("X");
  if(Get_Active_Goverment_Name() == "Horde")
    out--;
  return out;
}

bool Civ::Has_Researched_Border_Expand_Tech_Recently()
{
  bool out = recent_expand;
  recent_expand = false;
  return out;
}

string Civ::Get_Capital_Name()
{
  if(Cities.size() == 0)
    return "exile";
  return Cities[0].name;
}

string Civ::Get_Raw_Name()
{
  return Get_Name();
}

string Civ::Get_Raw_Leader_Name()
{
  return leader;
}

void Civ::Change_Goverment_By_Name(string new_gov_name)
{
  if(Active_Goverment.Get_Name() == "Despotism")
    gold = 0;
  for(auto &var : Goverments)
  {
    if(var.Get_Name() == new_gov_name)
      Active_Goverment = var;
  }
  if(! Has_Trait("C"))
    current_actions = 0;
  if(Leaders.size() != 1)
  {
    int leader_id = rand() % Leaders.size();
    leader = Leaders[leader_id];
    Leaders.erase(Leaders.begin() + leader_id);
  }
}

bool Civ::Has_Tech_Been_Researched_By_Trait(string trait_name)
{
  for(auto &var : Tech_Tree)
  {
    if(var.Is_Researched_And_Has_Trait(trait_name))
      return true;
  }
  return false;
}

vector<Unit> Civ::Get_Units()
{
  return Unit_Templates;
}

void Civ::Recruit_Unit_By_Name(string name, int x, int y)
{
  for(auto &var : Unit_Templates)
  {
    if(var.Get_Name() == name)
    {
      Unit_On_Map tmp(var);
      //tmp.Self = var;
      tmp.Coordinates.x = x;
      tmp.Coordinates.y = y;
      Units_Owned.push_back(tmp);
      if(Active_Goverment.Get_Name() != "Fundamentalism")
        current_actions--;
      gold = gold - var.Get_Cost();
      if(Active_Goverment.Get_Name() == "Despotism")
        gold++;
    }
  }
}

vector<Owned_City> Civ::Get_Owned_Cities()
{
  return Cities;
}

void Civ::Move_Unit_To_By_Coords(int unit_x, int unit_y, int dest_x, int dest_y, int cost)
{
  for(auto &unit : Units_Owned)
  {
    if(unit.Coordinates.x == unit_x && unit.Coordinates.y == unit_y)
    {
      unit.Coordinates.x = dest_x;
      unit.Coordinates.y = dest_y;
      unit.Self.Decrease_Movement(cost);
    }
  }
}

vector<Unit_On_Map>* Civ::Get_Owned_Units()
{
  return &Units_Owned;
}

string Civ::Lose_City_By_Coords(int x, int y)
{
  max_actions--;
  int index = 0;
  string out = " ";
  for(auto &city : Cities)
  {
    if(city.Coordinates.x == x && city.Coordinates.y == y)
    {
      out = city.name;
      Cities.erase(Cities.begin() + index);
      return out;
    }
    index++;
  }
  Logger::Log_Error("City Not Found");
  return out;
}

string Civ::Get_Active_Goverment_Name()
{
  return Active_Goverment.Get_Name();
}

int Civ::Get_Army_Manpower()
{
  int out = 0;
  for(auto &unit : Units_Owned)
  {
    out = out + unit.Self.Get_Manpower();
  }
  return out;
}

int Civ::Get_Population()
{
  int out = 0;
  int per_city_multiplier = 10000;
  int radius = Get_Upgrade_Border_Radius();
  if(radius > 2)
    per_city_multiplier = 100000;
  if(radius > 3)
    per_city_multiplier = 500000;
  out = out + (Get_Number_Of_Cities_Owned() * per_city_multiplier);
  out = out + Get_Army_Manpower();
  double entropy = rand() % 10;
  if(rand() % 2 == 0)
    entropy = 1.0 + (entropy / 100);
  else
    entropy = 1.0 - (entropy / 100);
  out = out * entropy;
  return out;
}

Civ::Civ(xml_node<>* Root_Node) : Traits_Owner(Root_Node), Help_Object(Root_Node), Texture_Owner(Root_Node), Audio_Owner(Root_Node)
{
  Deserialize(Root_Node);
}

void Civ::Deserialize(xml_node<>* Root_Node)
{
  recent_expand = (bool) stoi(Root_Node->first_attribute("recent_expand")->value());
  research_percent = stoi(Root_Node->first_attribute("research_percent")->value());
  city_name_index = stoi(Root_Node->first_attribute("city_name_index")->value());
  points = stoi(Root_Node->first_attribute("points")->value());
  points_from_technologies = stoi(Root_Node->first_attribute("points_from_tech")->value());
  leader = Root_Node->first_attribute("leader")->value();
  personality = Root_Node->first_attribute("personality")->value();
  tech_in_research = Root_Node->first_attribute("tech_in_research")->value();
  gold = stoi(Root_Node->first_attribute("gold")->value());
  id = stoi(Root_Node->first_attribute("id")->value());
  max_actions = stoi(Root_Node->first_attribute("max_actions")->value());
  current_actions = stoi(Root_Node->first_attribute("current_actions")->value());
  tech_money_modifier = atof(Root_Node->first_attribute("tech_money_modifier")->value());
  color = (int32_t) stoi(Root_Node->first_attribute("color")->value());
  Gov tmp(Root_Node->first_node("goverment"));
  Active_Goverment = tmp;
  xml_node<>* City_Names_Node = Root_Node->first_node("city_names");
  for(xml_node<> *City_Name_Node = City_Names_Node->first_node("city"); City_Name_Node; City_Name_Node = City_Name_Node->next_sibling("city"))
  {
    City_Names.push_back(City_Name_Node->value());
  }

  xml_node<>* Leaders_Node = Root_Node->first_node("leaders");
  for(xml_node<> *Leader_Node = Leaders_Node->first_node("leader"); Leader_Node; Leader_Node = Leader_Node->next_sibling("leader"))
  {
    Leaders.push_back(Leader_Node->value());
  }

  xml_node<>* Goverments_Node = Root_Node->first_node("goverments");
  for(xml_node<> *Goverment_Node = Goverments_Node->first_node("goverment"); Goverment_Node; Goverment_Node = Goverment_Node->next_sibling("goverment"))
  {
    Gov tmp(Goverment_Node);
    Goverments.push_back(tmp);
  }

  xml_node<>* Units_Node = Root_Node->first_node("units");
  for(xml_node<> *Unit_Node = Units_Node->first_node("unit"); Unit_Node; Unit_Node = Unit_Node->next_sibling("unit"))
  {
    Unit tmp(Unit_Node);
    Unit_Templates.push_back(tmp);
  }

  xml_node<>* Technologies_Node = Root_Node->first_node("techs");
  for(xml_node<> *Technology_Node = Technologies_Node->first_node("tech"); Technology_Node; Technology_Node = Technology_Node->next_sibling("tech"))
  {
    Tech tmp(Technology_Node);
    Tech_Tree.push_back(tmp);
  }

  xml_node<>* Upgrades_Node = Root_Node->first_node("upgrades");
  for(xml_node<> *Upgrade_Node = Upgrades_Node->first_node("upgrade"); Upgrade_Node; Upgrade_Node = Upgrade_Node->next_sibling("upgrade"))
  {
    Upgrade tmp(Upgrade_Node);
    Upgrades.push_back(tmp);
  }

  xml_node<>* Owned_Cities_Node = Root_Node->first_node("owned_cities");
  for(xml_node<> *Owned_City_Node = Owned_Cities_Node->first_node("owned_city"); Owned_City_Node; Owned_City_Node = Owned_City_Node->next_sibling("owned_city"))
  {
    Owned_City tmp;
    tmp.Coordinates.x = stoi(Owned_City_Node->first_attribute("x")->value());
    tmp.Coordinates.y = stoi(Owned_City_Node->first_attribute("y")->value());
    tmp.name = Owned_City_Node->first_attribute("city_name")->value();
    Cities.push_back(tmp);
  }

  xml_node<>* Owned_Units_Node = Root_Node->first_node("owned_units");
  for(xml_node<> *Owned_Unit_Node = Owned_Units_Node->first_node("owned_unit"); Owned_Unit_Node; Owned_Unit_Node = Owned_Unit_Node->next_sibling("owned_unit"))
  {
    Unit_On_Map tmp(Owned_Unit_Node);
    tmp.Coordinates.x = stoi(Owned_Unit_Node->first_attribute("x")->value());
    tmp.Coordinates.y = stoi(Owned_Unit_Node->first_attribute("y")->value());
    Unit tmp_unit(Owned_Unit_Node->first_node("unit"));
    tmp.Self = tmp_unit;
    Units_Owned.push_back(tmp);
  }

  xml_node<> *Goverment_Name_Replacements_Node = Root_Node->first_node("goverment_name_replacements");
  for(xml_node<> *Goverment_Name_Replacement_Node = Goverment_Name_Replacements_Node->first_node("replacement"); Goverment_Name_Replacement_Node; Goverment_Name_Replacement_Node = Goverment_Name_Replacement_Node->next_sibling("replacement"))
  {
    vector<string> tmp;
    for(xml_node<> *Name_Node = Goverment_Name_Replacement_Node->first_node("name"); Name_Node; Name_Node = Name_Node->next_sibling("name"))
    {
      tmp.push_back(Name_Node->value());
    }
    Goverment_Name_Replacements[Goverment_Name_Replacement_Node->first_attribute("goverment_name")->value()] = tmp;
  }
}

xml_node<>* Civ::Serialize(memory_pool<>* doc)
{
  Logger::Log_Info( "Serializing " + Get_Raw_Name());
  xml_node<> *Root_Node = doc->allocate_node(node_element, "civ");
  xml_attribute<> *Recent_Expand = doc->allocate_attribute("recent_expand", doc->allocate_string(to_string(recent_expand).c_str()));
  Root_Node->append_attribute(Recent_Expand);
  xml_attribute<> *Research_Percent = doc->allocate_attribute("research_percent", doc->allocate_string(to_string(research_percent).c_str()));
  Root_Node->append_attribute(Research_Percent);
  xml_attribute<> *City_Name = doc->allocate_attribute("city_name_index", doc->allocate_string(to_string(city_name_index).c_str()));
  Root_Node->append_attribute(City_Name);
  xml_attribute<> *Points = doc->allocate_attribute("points_from_tech", doc->allocate_string(to_string(points_from_technologies).c_str()));
  Root_Node->append_attribute(Points);
  xml_attribute<> *Points_From_Tech = doc->allocate_attribute("points", doc->allocate_string(to_string(points).c_str()));
  Root_Node->append_attribute(Points_From_Tech);
  xml_attribute<> *Tech_In_Research = doc->allocate_attribute("tech_in_research", doc->allocate_string(tech_in_research.c_str()));
  Root_Node->append_attribute(Tech_In_Research);
  xml_attribute<> *Gold = doc->allocate_attribute("gold", doc->allocate_string(to_string(gold).c_str()));
  Root_Node->append_attribute(Gold);
  xml_attribute<> *Id = doc->allocate_attribute("id", doc->allocate_string(to_string(id).c_str()));
  Root_Node->append_attribute(Id);
  xml_attribute<> *Max_Actions = doc->allocate_attribute("max_actions", doc->allocate_string(to_string(max_actions).c_str()));
  Root_Node->append_attribute(Max_Actions);
  xml_attribute<> *Current_Actions = doc->allocate_attribute("current_actions", doc->allocate_string(to_string(current_actions).c_str()));
  Root_Node->append_attribute(Current_Actions);
  xml_attribute<> *Color = doc->allocate_attribute("color", doc->allocate_string(to_string(color).c_str()));
  Root_Node->append_attribute(Color);
  xml_attribute<> *Tech_Money = doc->allocate_attribute("tech_money_modifier", doc->allocate_string(to_string(tech_money_modifier).c_str()));
  Root_Node->append_attribute(Tech_Money);
  xml_attribute<> *Leader = doc->allocate_attribute("leader", doc->allocate_string(leader.c_str()));
  Root_Node->append_attribute(Leader);
  xml_attribute<> *Personality = doc->allocate_attribute("personality", doc->allocate_string(personality.c_str()));
  Root_Node->append_attribute(Personality);

  xml_node<> *Leaders_Node = doc->allocate_node(node_element, "leaders");

  for_each(Leaders.begin(), Leaders.end(), [&](string leader_name)
  {
    xml_node<>* leader_node = doc->allocate_node(node_element, "leader", doc->allocate_string(leader_name.c_str()));
    Leaders_Node->append_node(leader_node);
  } );


  xml_node<> *City_Names_Node = doc->allocate_node(node_element, "city_names");

  for_each(City_Names.begin(), City_Names.end(), [&](string city_name)
  {
    xml_node<>* city_node = doc->allocate_node(node_element, "city", doc->allocate_string(city_name.c_str()));
    City_Names_Node->append_node(city_node);
  } );

  xml_node<> *Govs_Node = doc->allocate_node(node_element, "goverments");

  for_each(Goverments.begin(), Goverments.end(), [&](Gov& iterated_gov)
  {
    Govs_Node->append_node(iterated_gov.Serialize(doc));
  });

  xml_node<> *Upgrades_Node = doc->allocate_node(node_element, "upgrades");

  for_each(Upgrades.begin(), Upgrades.end(), [&](Upgrade& iterated_upgrade)
  {
    Upgrades_Node->append_node(iterated_upgrade.Serialize(doc));
  });

  xml_node<> *Technologies_Node = doc->allocate_node(node_element, "techs");
  xml_node<> *Units_Node = doc->allocate_node(node_element, "units");

  for_each(Tech_Tree.begin(), Tech_Tree.end(), [&](Tech& iterated_tech)
  {
    Technologies_Node->append_node(iterated_tech.Serialize(doc));
  });

  for_each(Unit_Templates.begin(), Unit_Templates.end(), [&](Unit& iterated_unit)
  {
    Units_Node->append_node(iterated_unit.Serialize(doc));
  });

  xml_node<> *Owned_Cities_Node = doc->allocate_node(node_element, "owned_cities");

  for(Owned_City& iterated_city : Cities)
  {
    xml_node<> *owned_city_node = doc->allocate_node(node_element, "owned_city");
    xml_attribute<> *x_coord = doc->allocate_attribute("x", doc->allocate_string(to_string(iterated_city.Coordinates.x).c_str()));
    owned_city_node->append_attribute(x_coord);
    xml_attribute<> *y_coord = doc->allocate_attribute("y", doc->allocate_string(to_string(iterated_city.Coordinates.y).c_str()));
    owned_city_node->append_attribute(y_coord);
    xml_attribute<> *city_name = doc->allocate_attribute("city_name", doc->allocate_string(iterated_city.name.c_str()));
    owned_city_node->append_attribute(city_name);
    Owned_Cities_Node->append_node(owned_city_node);
  };

  xml_node<> *Owned_Units_Node = doc->allocate_node(node_element, "owned_units");

  for_each(Units_Owned.begin(), Units_Owned.end(), [&](Unit_On_Map& iterated_unit)
  {
    xml_node<> *owned_unit_node = doc->allocate_node(node_element, "owned_unit");
    xml_attribute<> *x_coord = doc->allocate_attribute("x", doc->allocate_string(to_string(iterated_unit.Coordinates.x).c_str()));
    owned_unit_node->append_attribute(x_coord);
    xml_attribute<> *y_coord = doc->allocate_attribute("y", doc->allocate_string(to_string(iterated_unit.Coordinates.y).c_str()));
    owned_unit_node->append_attribute(y_coord);
    owned_unit_node->append_node(iterated_unit.Self.Serialize(doc));
    Owned_Units_Node->append_node(owned_unit_node);
  });

  xml_node<> *Goverment_Name_Replacements_Node = doc->allocate_node(node_element, "goverment_name_replacements");

  for(auto &replacement : Goverment_Name_Replacements)
  {
    xml_node<> *replacement_node = doc->allocate_node(node_element, "replacement");
    xml_attribute<> *gov_name = doc->allocate_attribute("goverment_name", doc->allocate_string(replacement.first.c_str()));
    replacement_node->append_attribute(gov_name);

    for_each(replacement.second.begin(), replacement.second.end(), [&](string name)
  {
    xml_node<>* name_node = doc->allocate_node(node_element, "name", doc->allocate_string(name.c_str()));
    replacement_node->append_node(name_node);
  });

    Goverment_Name_Replacements_Node->append_node(replacement_node);
  }

  Root_Node->append_node(Active_Goverment.Serialize(doc));
  Root_Node->append_node(Serialize_Help(doc));
  Root_Node->append_node(City_Names_Node);
  Root_Node->append_node(Serialize_Traits(doc));
  Root_Node->append_node(Serialize_Textures(doc));
  Root_Node->append_node(Serialize_Audio(doc));
  Root_Node->append_node(Owned_Units_Node);
  Root_Node->append_node(Upgrades_Node);
  Root_Node->append_node(Leaders_Node);
  Root_Node->append_node(Goverment_Name_Replacements_Node);
  Root_Node->append_node(Technologies_Node);
  Root_Node->append_node(Owned_Cities_Node);
  Root_Node->append_node(Units_Node);
  Root_Node->append_node(Govs_Node);

  return Root_Node;
}

int Civ::Get_Number_Of_Researched_Techs()
{
  return count_if(Tech_Tree.begin(), Tech_Tree.end(), [](Tech &tech){return tech.Is_Reseached();});
}

void Civ::Disband_First_Unit()
{
  if(Units_Owned.size() != 0)
  {
    Remove_Unit_By_Coords(Units_Owned[0].Coordinates.x, Units_Owned[0].Coordinates.y);
    Units_Owned.erase(Units_Owned.begin());
    current_actions--;
  }
}

int Civ::Get_Number_Of_Naval_Units()
{
  return count_if(Units_Owned.begin(), Units_Owned.end(), [](Unit_On_Map& unit){return unit.Self.Is_Naval();});
}

void Civ::Set_Research_Tech_By_Trait(string trait)
{
  vector<Tech> Possible_Techs = Get_Possible_Research_Techs();
  vector<Tech> Possible_Techs_With_Trait;
  for(auto &tech : Possible_Techs)
  {
    if(tech.Has_Trait(trait))
      Possible_Techs_With_Trait.push_back(tech);
  }
  if(Possible_Techs_With_Trait.size() > 0)
    Set_Research_Tech_By_Name(Possible_Techs_With_Trait[0].Get_Name());
  else
    Set_Research_Tech_By_Name(Possible_Techs[0].Get_Name());
}
