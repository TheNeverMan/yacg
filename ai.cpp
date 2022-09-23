#include "ai.h"

AI::AI(Game *m_g)
{
  Main_Game = m_g;
}

bool AI::Is_Income_For_Currently_Moving_Player_Is_Negative()
{
  vector<int> income = Main_Game->Get_Map()->Get_Netto_Income_For_Player_By_Id(Main_Game->Get_Currently_Moving_Player_Id(), *Main_Game->Get_Currently_Moving_Player());
  int plus = income[0];
  int minus = income[1];
  int unit_minus = Main_Game->Get_Currently_Moving_Player()->Get_Unit_Maitenance();
  plus = plus - minus;
  plus = plus - unit_minus;
  if(plus > 0)
    return false;
  return true;
}






bool AI::Build_Random_Producing_Upgrade()
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
            return false;
          Main_Game->Build_Upgrade(upg.Get_Name(), out[0], out[1], Main_Game->Get_Currently_Moving_Player_Id());
          return true;
        }
      }
    }
  }
  return false;
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
    if(!(Main_Game->Get_Map()->Is_Upgrade_In_Radius_By_Name("City", var.Coordinates.x, var.Coordinates.y)) && Main_Game->Get_Upgrade_By_Name("City").Is_Tile_Allowed_By_Name(Main_Game->Get_Map()->Get_Tile(var.Coordinates.x, var.Coordinates.y).Get_Name()) && Main_Game->Get_Map()->Get_Owner(var.Coordinates.x, var.Coordinates.y) == 0)
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

  vector<int> out = Main_Game->Get_Map()->Find_Direction_Away_From_Borders(Main_Game->Get_Currently_Moving_Player_Id(), unit.Coordinates.x, unit.Coordinates.y, unit.Self.Get_Current_Actions(), unit.Self);
  if(out[0] == 1)
    Main_Game->Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(unit.Coordinates.x, unit.Coordinates.y, out[2], out[3], out[1], (bool) out[4], out[5], out[6]);

}

array<int ,2> AI::Get_Random_Player_Capital_Location()
{
  bool loop = true;
  while(loop)
  {
    int random = rand() % Main_Game->Get_Amount_Of_Players();
    if(random == 0)
      continue;
    if(!Main_Game->Is_Player_Eliminated(random))
    {
      return Main_Game->Get_Player_By_Id(random)->Get_Capital_Location();
    }
  }
  return {0,0};
}

void AI::Move_Unit_Towards_Enemy(int x, int y, Unit u)
{
  vector<int> out = Main_Game->Get_Map()->Find_Direction_To_Enemy_City_Or_Unit(Main_Game->Get_Currently_Moving_Player_Id(), x, y, u.Get_Current_Actions(), u);
  array<int, 2> capital = Get_Random_Player_Capital_Location();
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
  vector<Owned_City> cities = Main_Game->Get_Currently_Moving_Player()->Get_Owned_Cities();
  reverse(units.begin(), units.end());
  for(auto &city : cities)
  {
    if(Main_Game->Get_Map()->Get_Tile(city.Coordinates.x, city.Coordinates.y).Has_Unit() && Main_Game->Get_Map()->Get_Tile(city.Coordinates.x, city.Coordinates.y).Get_Unit_Owner_Id() == Main_Game->Get_Currently_Moving_Player_Id())
    {
      Move_Unit_Towards_Enemy(city.Coordinates.x, city.Coordinates.y, Main_Game->Get_Currently_Moving_Player()->Get_Unit_On_Tile(city.Coordinates.x, city.Coordinates.y));
      if(Main_Game->Get_Map()->Get_Tile(city.Coordinates.x, city.Coordinates.y).Has_Unit())
        continue;
    }
    for(auto &unit : units)
    {
      if(Main_Game->Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Name(unit.Get_First_Requirement()) && unit.Can_Move_On_Tile_By_Name(Main_Game->Get_Map()->Get_Tile(city.Coordinates.x, city.Coordinates.y).Get_Name()))
      {
        if(Main_Game->Get_Currently_Moving_Player()->Get_Gold() >= unit.Get_Cost())
        {
          Main_Game->Recruit_Unit(unit.Get_Name(), city.Coordinates.x, city.Coordinates.y);
          return true;
        }
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

AI_Data AI::Process_Turn(AI_Data Data)
{
  Logger::Log_Info("Starting AI Turn of " + Main_Game->Get_Currently_Moving_Player()->Get_Name());
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
  int economy_goal = 15; //gold per city
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

  if(Is_Income_For_Currently_Moving_Player_Is_Negative())
  {
    while(! Is_Income_For_Currently_Moving_Player_Is_Negative() && Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
    {
      if(!Build_Random_Producing_Upgrade())
        Main_Game->Get_Currently_Moving_Player()->Disband_First_Unit();
    }
  }

  economy_parameter = economy_goal - Main_Game->Get_Map()->Get_Netto_Income_For_Player_By_Id(Main_Game->Get_Currently_Moving_Player_Id(), *Main_Game->Get_Currently_Moving_Player())[0] / city_count;
  technologic_parameter = tech_goal - Main_Game->Get_Currently_Moving_Player()->Get_Number_Of_Researched_Techs() / city_count;
  technologic_parameter = technologic_parameter * 5;
  military_parameter = military_goal - Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units()->size() / city_count;
  military_parameter = military_parameter * 3;
  naval_parameter = Main_Game->Get_Map()->Count_Tiles_Owned_By_Player(Main_Game->Get_Currently_Moving_Player_Id(), "Sea") / (Main_Game->Get_Currently_Moving_Player()->Get_Number_Of_Naval_Units() + 1);
  int tech_class = Find_Biggest_Parameter({economy_parameter, military_parameter, naval_parameter, expanse_parameter});
  Change_Goverment_If_Necessary();
  technologic_parameter = Change_Technology_Goal(technologic_parameter, tech_class);
  while(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
  {
    //cout << "Turn" << endl;
    int index = Find_Biggest_Parameter({economy_parameter, military_parameter, naval_parameter, expanse_parameter});
    if(economy_parameter <= 0 && military_parameter <= 0 && expanse_parameter <= 0 && naval_parameter <= 0)
      break;
    switch(index)
    {
      case 0:
      {
        economy_parameter = 0;
        bool loop = true;
        while(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left() && !(economy_goal == Main_Game->Get_Map()->Get_Netto_Income_For_Player_By_Id(Main_Game->Get_Currently_Moving_Player_Id(), *Main_Game->Get_Currently_Moving_Player())[0] / city_count) && loop)
        {
          loop = Build_Random_Producing_Upgrade();
        }
        break;
      }
      case 1:
      {
        military_parameter = 0;
        bool loop = true;
        while(Main_Game->Has_Currently_Moving_Player_Any_Actions_Left() && !(military_goal == static_cast<int>(Main_Game->Get_Currently_Moving_Player()->Get_Owned_Units()->size() / city_count)) && loop)
        {
          loop = Recruit_Unit_In_City();
        }
        break;
      }
      case 2:
      {
        naval_parameter = 0;
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

  Main_Game->Get_Currently_Moving_Player()->Set_Research_Funds_Percentage((double) technologic_parameter * 7.0);
  return Data;
}
