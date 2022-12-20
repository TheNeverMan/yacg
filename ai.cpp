#include "ai.h"

AI::AI(Game *m_g)
{
  Main_Game = m_g;
}

bool AI::Is_Income_For_Currently_Moving_Player_Is_Negative()
{
  if(Get_Currently_Moving_Player_Finances() <= 0)
    return true;
  return false;
}

int AI::Get_Currently_Moving_Player_Finances()
{
  vector<int> income = Main_Game->Get_Map()->Get_Netto_Income_For_Player_By_Id(Main_Game->Get_Currently_Moving_Player_Id(), *Main_Game->Get_Currently_Moving_Player());
  int plus = income[0];
  int minus = income[1];
  int unit_minus = Main_Game->Get_Currently_Moving_Player()->Get_Unit_Maitenance();
  plus = plus - minus;
  plus = plus - unit_minus;
  return plus;
}


int AI::Calculate_Distance_Between_Points(int p_x, int p_y, int g_x, int g_y)
{
  int out = sqrt((abs(pow(p_x - g_x,2))) + abs((pow(p_y - g_y,2))));
  return out;
}

array<int ,2> AI::Get_Closest_Point(int x, int y, vector<array<int ,2>> points)
{
  map<int, array<int, 2>> out;
  for_each(points.begin(), points.end(), [&](array<int, 2> point)
  {
    out[Calculate_Distance_Between_Points(x,y,point[0],point[1])] = point;
  });
  for(auto &var : out)
  {
    return var.second;
  }
  Logger::Log_Error("No points in Get_Closest_Point");
  return {0,0};
}



int AI::Build_Random_Producing_Upgrade()
{
  for(auto &upg : Main_Game->Get_Upgrades())
  {
    if(upg.Get_Production() != 0)
    {
      if(Main_Game->Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Name(upg.Get_First_Requirement()))
      {
        if(Main_Game->Get_Currently_Moving_Player()->Get_Gold() >= upg.Get_Cost())
        {
          vector<int> out = Main_Game->Get_Map()->Find_Owned_Tile_For_Upgrade(Main_Game->Get_Currently_Moving_Player_Id(), upg.Get_Name());
          if(out[0] == -1)
            continue;
          Main_Game->Build_Upgrade(upg.Get_Name(), out[0], out[1], Main_Game->Get_Currently_Moving_Player_Id());
          return upg.Get_Production();
        }
      }
    }
  }
  return 0;
}

void AI::Build_Naval_Producing_Upgrades()
{
  for(auto &upg : Main_Game->Get_Upgrades())
  {
    if(upg.Get_Production() != 0 && upg.Is_Tile_Allowed_By_Name("Sea"))
    {
      if(Main_Game->Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Name(upg.Get_First_Requirement()))
      {
        if(Main_Game->Get_Currently_Moving_Player()->Get_Gold() >= upg.Get_Cost())
        {
          vector<int> out = Main_Game->Get_Map()->Find_Owned_Tile_For_Upgrade(Main_Game->Get_Currently_Moving_Player_Id(), upg.Get_Name());
          if(out[0] == -1)
            return;
          Main_Game->Build_Upgrade(upg.Get_Name(), out[0], out[1], Main_Game->Get_Currently_Moving_Player_Id());
          return;
        }
      }
    }
  }
}

void AI::Build_Naval_Recruitment_Upgrades()
{
  for(auto &upg : Main_Game->Get_Upgrades())
  {
    if(upg.Is_Tile_Allowed_By_Name("Sea") && upg.Has_Trait("recruit"))
    {
      if(Main_Game->Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Name(upg.Get_First_Requirement()))
      {
        if(Main_Game->Get_Currently_Moving_Player()->Get_Gold() >= upg.Get_Cost())
        {
          vector<int> out = Main_Game->Get_Map()->Find_Owned_Tile_For_Upgrade(Main_Game->Get_Currently_Moving_Player_Id(), upg.Get_Name());
          if(out[0] == -1)
            return;
          Main_Game->Build_Upgrade(upg.Get_Name(), out[0], out[1], Main_Game->Get_Currently_Moving_Player_Id());
          return;
        }
      }
    }
  }
}

int AI::Find_Biggest_Parameter(std::vector<int> input)
{
  int out = 0;
  int index = 0;
  int biggest = -1;
  for(int var : input)
  {
    if(var > biggest)
    {
      biggest = var;
      out = index;
    }
    index++;
  }
  return out;
}


bool AI::Can_Settle_City_With_Unit()
{
  vector<Unit_On_Map> units = *Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units();
  for(auto &var : units)
  {
    if(!(Main_Game->Get_Map()->Is_Upgrade_In_Radius_By_Name("City", var.Coordinates.x, var.Coordinates.y)))
    {
      return true;
    }
  }
  return false;
}


void AI::Settle_City_With_Unit()
{
  vector<Unit_On_Map> units = *Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units();
  for(auto &var : units)
  {
    if(!(Main_Game->Get_Map()->Is_Upgrade_In_Radius_By_Name("City", var.Coordinates.x, var.Coordinates.y)) && Main_Game->Get_Upgrade_By_Name("City").Is_Tile_Allowed_By_Name(Main_Game->Get_Map()->Get_Tile(var.Coordinates.x, var.Coordinates.y).Get_Name()) && Main_Game->Get_Map()->Get_Owner(var.Coordinates.x, var.Coordinates.y) == 0 && Main_Game->Get_Currently_Moving_Player()->Get_Gold() > 25)
    {
      Main_Game->Build_City(var.Coordinates.x, var.Coordinates.y, Main_Game->Get_Currently_Moving_Player_Id(), Main_Game->Get_Currently_Moving_Player()->Get_Upgrade_Border_Radius());
    }
  }
}

void AI::Move_All_Units_Not_In_Cities_To_Enemy()
{
  vector<Unit_On_Map> units = *Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units();
  for(auto &unit : units)
  {
    if(Main_Game->Get_Map()->Get_Tile_Pointer(unit.Coordinates.x, unit.Coordinates.y)->Get_Upgrade() != "City")
      Move_Unit_Towards_Enemy(unit.Coordinates.x, unit.Coordinates.y, unit.Self);
  }
}

void AI::Move_All_Units_Not_In_Cities_Away_From_Borders()
{
  vector<Unit_On_Map> units = *Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units();
  for(auto &unit : units)
  {
    if(Main_Game->Get_Map()->Get_Tile_Pointer(unit.Coordinates.x, unit.Coordinates.y)->Get_Upgrade() != "City")
      Move_Unit_Away_From_Borders(unit);
  }
}

void AI::Move_Unit_Away_From_Borders(Unit_On_Map unit)
{

  vector<int> out = Main_Game->Get_Map()->Find_Direction_To_Settle_City(Main_Game->Get_Currently_Moving_Player_Id(), unit.Coordinates.x, unit.Coordinates.y, unit.Self);
  if(out[0] == 1)
    Main_Game->Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(unit.Coordinates.x, unit.Coordinates.y, out[2], out[3], out[1], (bool) out[4], out[5], out[6]);

}

array<int ,2> AI::Get_Closest_Player_Capital_Location()
{
  vector<array<int,2>> capitals;
  int index = 1;
  while(index <= Main_Game->Get_Amount_Of_Players())
  {
    if(index == Main_Game->Get_Currently_Moving_Player_Id())
    {
      index++;
      continue;
    }
    if(!Main_Game->Is_Player_Eliminated(index))
    {
      capitals.push_back(Main_Game->Get_Player_By_Id(index)->Get_Capital_Location());
    }
    index++;
  }
  array<int, 2> cap = Get_Closest_Point(static_cast<int>(Main_Game->Get_Currently_Moving_Player()->Get_Capital_Location()[0]), static_cast<int>(Main_Game->Get_Currently_Moving_Player()->Get_Capital_Location()[1]), capitals);
//  //cout << cap[0] << " " << cap[1];
  return cap;
}
void AI::Move_Unit_Towards_Enemy(int x, int y, Unit u)
{
  vector<int> out = Main_Game->Get_Map()->Find_Direction_To_Enemy_City_Or_Unit(Main_Game->Get_Currently_Moving_Player_Id(), x, y, u.Get_Current_Actions(), u);
  array<int, 2> capital = Get_Closest_Player_Capital_Location();
  vector<int> path_to_capital = Main_Game->Get_Map()->Check_If_Path_For_Unit_Exists(x, y, capital[0], capital[1], u);
  if(rand() % 2 == 0 && out[0] == 1)
  {
    Main_Game->Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(x, y, out[2], out[3], out[1], (bool) out[4], out[5], out[6]);
  }
  else if(path_to_capital[0] == 1)
  {
    Main_Game->Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(x, y, path_to_capital[2], path_to_capital[3], path_to_capital[1], (bool) path_to_capital[4], path_to_capital[5], path_to_capital[6]);
  }
}

bool AI::Recruit_Unit_In_City()
{
  vector<Unit> units = Main_Game->Get_Currently_Moving_Player()->Get_Units();
  vector<City> cities = Main_Game->Get_Currently_Moving_Player()->Get_Owned_Cities_Not_Pointer();
  reverse(units.begin(), units.end());
  for(auto &city : cities)
  {
    int x = city.Get_Coords()[0];
    int y = city.Get_Coords()[1];
    if(Main_Game->Get_Map()->Get_Tile(city.Get_Coords()[0], city.Get_Coords()[1]).Has_Unit() && Main_Game->Get_Map()->Get_Tile(city.Get_Coords()[0], city.Get_Coords()[1]).Get_Unit_Owner_Id() == Main_Game->Get_Currently_Moving_Player_Id())
    {
      Move_Unit_Towards_Enemy(x,y, Main_Game->Get_Currently_Moving_Player()->Get_Unit_On_Tile(city.Get_Coords()[0], city.Get_Coords()[1]));
      if(Main_Game->Get_Map()->Get_Tile(x,y).Has_Unit())
        continue;
    }
    for(auto &unit : units)
    {
      if(Main_Game->Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Name(unit.Get_First_Requirement()) && unit.Can_Move_On_Tile_By_Name(Main_Game->Get_Map()->Get_Tile(city.Get_Coords()[0], city.Get_Coords()[1]).Get_Name()))
      {
        if(Main_Game->Get_Currently_Moving_Player()->Get_Gold() >= unit.Get_Cost())
        {
          Main_Game->Recruit_Unit(unit.Get_Name(), x,y);
          return true;
        }
      }
    }
  }

  return false;
}

bool AI::Recruit_Unit_By_Class_And_Coords(int x, int y, string unit_class)
{
  if(!Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
    return false;
  vector<Unit> units = Main_Game->Get_Currently_Moving_Player()->Get_Units();
  reverse(units.begin(), units.end());
  std::vector<Unit>::iterator iter = units.begin();
  while ((iter = std::find_if(iter, units.end(), [unit_class](Unit& u){return u.Get_All_Arguments_For_Trait("class")[0] == unit_class;})) != units.end())
  {
    if(Main_Game->Get_Currently_Moving_Player()->Get_Gold() >= iter->Get_Cost())
    {
      Main_Game->Recruit_Unit(iter->Get_Name(), x, y);
      return true;
    }

    iter++;
  }
  return false;
}

bool AI::Recruit_Naval_Units()
{
  vector<Upgrade> upgrades = Main_Game->Get_Upgrades();

  for(auto& upg : upgrades)
  {
    if(upg.Has_Trait("recruit") && (upg.Get_All_Arguments_For_Trait("recruit")[0] == "naval" || upg.Get_All_Arguments_For_Trait("recruit")[0] == "weaknaval"))
    {
      vector<array<int, 2>> tiles_with_upgrade = Main_Game->Get_Map()->Find_All_Upgrade_Locations(Main_Game->Get_Currently_Moving_Player_Id(), upg.Get_Name());
      for(auto &tile : tiles_with_upgrade)
      {
        if(Main_Game->Get_Map()->Get_Tile(tile[0],tile[1]).Has_Unit())
          continue;
        bool out = Recruit_Unit_By_Class_And_Coords(tile[0], tile[1], upg.Get_All_Arguments_For_Trait("recruit")[0]);
        if(!out)
          break;
      }
    }
  }

  return false;
}

bool AI::Recruit_Non_Infantry_Unit()
{
  vector<Upgrade> upgrades = Main_Game->Get_Upgrades();

  for(auto& upg : upgrades)
  {
    if(upg.Has_Trait("recruit") && (upg.Get_All_Arguments_For_Trait("recruit")[0] != "naval" && upg.Get_All_Arguments_For_Trait("recruit")[0] != "weaknaval" && upg.Get_All_Arguments_For_Trait("recruit")[0] != "infantry"))
    {
      vector<array<int, 2>> tiles_with_upgrade = Main_Game->Get_Map()->Find_All_Upgrade_Locations(Main_Game->Get_Currently_Moving_Player_Id(), upg.Get_Name());
      for(auto &tile : tiles_with_upgrade)
      {
        if(Main_Game->Get_Map()->Get_Tile(tile[0],tile[1]).Has_Unit())
          continue;
        bool out = Recruit_Unit_By_Class_And_Coords(tile[0], tile[1], upg.Get_All_Arguments_For_Trait("recruit")[0]);
        if(!out)
          break;
      }
    }
  }

  return false;
}

void AI::Change_Goverment_To_More_Advanced_One()
{
  vector<Gov> govs = Main_Game->Get_Currently_Moving_Player()->Get_Possible_Goverments();
  int index = 0;
  int chosen_gov = 0;
  int tech_cost = 0;
  for(auto &gov : govs)
  {
    if(Main_Game->Get_Total_Cost_Of_Technology_By_Name(gov.Get_First_Requirement()) > tech_cost)
    {
      chosen_gov = index;
      tech_cost = Main_Game->Get_Total_Cost_Of_Technology_By_Name(gov.Get_First_Requirement());
    }
    index++;
  }
  if(Main_Game->Get_Currently_Moving_Player()->Get_Active_Goverment_Name() != govs[chosen_gov].Get_Name() && govs[chosen_gov].Get_Name() != "Tribe")
    Main_Game->Change_Goverment_For_Currently_Moving_Player_By_Name(govs[chosen_gov].Get_Name());
}

void AI::Change_Goverment_If_Necessary()
{
  if(Main_Game->Get_Currently_Moving_Player()->Get_Possible_Goverments().size() > 1)
  {
    if(Main_Game->Get_Currently_Moving_Player()->Get_Active_Goverment_Name() == "Tribe")
      Main_Game->Change_Goverment_For_Currently_Moving_Player_By_Name(Main_Game->Get_Currently_Moving_Player()->Get_Possible_Goverments()[Main_Game->Get_Currently_Moving_Player()->Get_Possible_Goverments().size() - 1].Get_Name());
    Change_Goverment_To_More_Advanced_One();
  }
}

double AI::Change_Technology_Goal(double technologic_parameter, int tech_class)
{
  if(! Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
  {
    return technologic_parameter;
  }
  bool are_all_techs_researched = false;
  if(Main_Game->Get_Currently_Moving_Player()->Get_Possible_Research_Techs().size() == 0)
  {
    are_all_techs_researched = true;
    technologic_parameter = 0.0;
  }
  if(!are_all_techs_researched)
  {
    switch(tech_class)
    {
      case 0: //economy
      {
        Main_Game->Get_Currently_Moving_Player()->Set_Research_Tech_By_Trait("economic");
        break;
      }
      case 1: //military
      {
        Main_Game->Get_Currently_Moving_Player()->Set_Research_Tech_By_Trait("military");
        break;
      }
      case 2: //naval
      {
        Main_Game->Get_Currently_Moving_Player()->Set_Research_Tech_By_Trait("naval");
        break;
      }
      case 3: //expanse
      {
        Main_Game->Get_Currently_Moving_Player()->Set_Research_Tech_By_Trait("expanse");
        break;
      }
      default:
      {
        Main_Game->Get_Currently_Moving_Player()->Set_Research_Tech_By_Trait("default");
        break;
      }
    }
  }
  return technologic_parameter;
}

void AI::Heal_Units_In_Cities()
{
  vector<City>* cities = Main_Game->Get_Currently_Moving_Player()->Get_Owned_Cities();
  for(auto &city : *cities)
  {
    if(Main_Game->Get_Map()->Get_Tile(city.Get_Coords()[0], city.Get_Coords()[1]).Has_Unit())
      Main_Game->Get_Currently_Moving_Player()->Get_Unit_On_Tile_Pointer(city.Get_Coords()[0], city.Get_Coords()[1])->Heal(0);
  }
}

AI_Data AI::Process_Turn(AI_Data Data)
{
  Logger::Log_Info("Starting AI Turn of " + Main_Game->Get_Currently_Moving_Player()->Get_Name());
  Logger::Log_Info("AI Debug");
  string personality = " ";
  personality = Main_Game->Get_Currently_Moving_Player()->Get_Personality();
  int economy_parameter = 0;
  int military_parameter = 0;
  int technologic_parameter = 0;
  int naval_parameter = 0;
  int expanse_increase = 1;
  if(personality == "Expansive")
  {
    expanse_increase = 3;
  }
  Data.expanse_parameter = Data.expanse_parameter + expanse_increase;
  int expanse_parameter = Data.expanse_parameter;
  int economy_goal = 18; //gold per city
  int military_goal = 2; //units per city
  int tech_goal = 2; //techs per city
  //int naval_goal = 1; //units per 5 water tiles
  int city_count = Main_Game->Get_Currently_Moving_Player()->Get_Number_Of_Cities_Owned();
  if(personality == "Authoritarian")
    military_goal = 4;
  if(personality == "Enterprising")
    economy_goal = 25;
  if(personality == "Enlightened")
    tech_goal = 4;

  Logger::Log_Info("Personality: " + personality);
  Logger::Log_Info("Gold: " + to_string(Main_Game->Get_Currently_Moving_Player()->Get_Gold()));
  int player_finances = Get_Currently_Moving_Player_Finances();
  if(Is_Income_For_Currently_Moving_Player_Is_Negative() || Main_Game->Get_Currently_Moving_Player()->Get_Gold() < 0)
  {
    while(!(player_finances <= 0) && Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
    {
      int upgrade_production = Build_Random_Producing_Upgrade();
      if(upgrade_production == 0)
        break;
//if(upgrade_production == 0)
        //Main_Game->Get_Currently_Moving_Player()->Disband_First_Unit();
      //else
      player_finances = player_finances + upgrade_production;
    }
  }
  int player_finances_only_income = Main_Game->Get_Map()->Get_Netto_Income_For_Player_By_Id(Main_Game->Get_Currently_Moving_Player_Id(), *Main_Game->Get_Currently_Moving_Player())[0];
  economy_parameter = economy_goal - player_finances_only_income / city_count;
  technologic_parameter = tech_goal - Main_Game->Get_Currently_Moving_Player()->Get_Number_Of_Researched_Techs() / city_count;
  technologic_parameter = technologic_parameter * 5;
  if(technologic_parameter < 0)
    technologic_parameter = 2;
  military_parameter = military_goal - Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units()->size() / city_count;
  military_parameter = military_parameter * 3;
  naval_parameter = Main_Game->Get_Map()->Count_Tiles_Owned_By_Player(Main_Game->Get_Currently_Moving_Player_Id(), "Sea") / (Main_Game->Get_Map()->Count_Tiles_Owned_By_Player(Main_Game->Get_Currently_Moving_Player_Id(), "Land") + 1);
  if(personality == "Exploring")
    naval_parameter *= 2;
  int tech_class = Find_Biggest_Parameter({economy_parameter, military_parameter, naval_parameter, expanse_parameter});
  Change_Goverment_If_Necessary();
  technologic_parameter = Change_Technology_Goal(technologic_parameter, tech_class);
  Logger::Log_Info("Economic Parameter: " + to_string(economy_parameter));
  Logger::Log_Info("Technologic Parameter: " + to_string(technologic_parameter));
  Logger::Log_Info("Military Parameter: " + to_string(military_parameter));
  Logger::Log_Info("Naval Parameter: " + to_string(naval_parameter));
  Logger::Log_Info("Expanse Parameter: " + to_string(expanse_parameter));
  Logger::Log_Info("Technology Researched: " + Main_Game->Get_Currently_Moving_Player()->Get_Currently_Researched_Tech()->Get_Name());
  Logger::Log_Info("Research Funds: " + to_string(Main_Game->Get_Currently_Moving_Player()->Get_Research_Percent()));
  while(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
  {
    ////cout << "Turn" << endl;
    int index = Find_Biggest_Parameter({economy_parameter, military_parameter, naval_parameter, expanse_parameter});
    if(economy_parameter <= 0 && military_parameter <= 0 && expanse_parameter <= 0 && naval_parameter <= 0)
      break;
    switch(index)
    {
      case 0:
      {
        economy_parameter = 0;
        bool loop = true;
        while(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left() && !(economy_goal <= player_finances_only_income / city_count) && loop)
        {
          int upgrade_production = Build_Random_Producing_Upgrade();
          loop = static_cast<bool>(upgrade_production);
          player_finances_only_income += upgrade_production;
        }
        break;
      }
      case 1:
      {
        military_parameter = 0;
        bool loop = true;
        bool recruit_non_inf = true;
        while(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left() && !(military_goal <= static_cast<int>(Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units()->size() / city_count)) && loop)
        {
          if(rand() % 2 == 0 && recruit_non_inf)
            recruit_non_inf = Recruit_Non_Infantry_Unit();
          else
            loop = Recruit_Unit_In_City();
        }
        break;
      }
      case 2:
      {
        naval_parameter = 0;
        bool loop = true;
        while(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left() && !(military_goal <= static_cast<int>(Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units()->size() / city_count)) && loop)
        {
          if(economy_goal <= player_finances_only_income / city_count)
            Build_Naval_Producing_Upgrades();
          else
            Build_Naval_Recruitment_Upgrades();
          loop = Recruit_Naval_Units();
        }
        break;
      }
      case 3:
      {
        expanse_parameter = 0;
        if(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
        {
          if(Can_Settle_City_With_Unit() && Main_Game->Get_Currently_Moving_Player()->Get_Gold() >= Main_Game->Get_Upgrade_By_Name("City").Get_Cost())
          {
            Settle_City_With_Unit();
            Data.expanse_parameter = 0;
          }
        }
        if(Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units()->size() > 0)
        {
          Move_All_Units_Not_In_Cities_Away_From_Borders();
        }
        break;
      }
      default:
      {
        Logger::Log_Error("Error in AI Processing");
        break;
      }

    }
  }
  if(personality == "Peaceful")
  {
    if(rand() % 3 != 0)
      Move_All_Units_Not_In_Cities_To_Enemy();
  }
  else
  {
    Move_All_Units_Not_In_Cities_To_Enemy();
  }

  Main_Game->Get_Currently_Moving_Player()->Set_Research_Funds_Percentage((double) technologic_parameter * 7.0 + 10);
  Heal_Units_In_Cities();
  if(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
    while(Build_Random_Producing_Upgrade()){}

  return Data;
}
