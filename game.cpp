#include "game.h"

int Game::Get_Amount_Of_Players()
{
  return Players.size();
}

void Game::XML_Load_Data()
{
  XML_Data_Loader Loader(" ");
  Tiles = Loader.Load_Tiles();
  Technologies = Loader.Load_Techs();
  Units = Loader.Load_Units();
  Goverments = Loader.Load_Govs();
  Civs = Loader.Load_Civs();
  Upgrades = Loader.Load_Upgrades();
}


void Game::Generate_Map(Map_Generator_Data User_Data, bool load_starting_positions)
{
  Logger::Log_Info("Generating map...");
  Map_Generator Gen(User_Data, Tiles, Upgrades);
  Game_Map = Gen.Generate_Map_Using_User_Data();
  map<string, array<int, 2>> starting_positions = Gen.Fetch_Starting_Positions_From_Map_Data();
  if(starting_positions.empty() || !(load_starting_positions))
  {
    Logger::Log_Info("Assigning random starting positions...");
    Assign_Random_Starting_Positions();
  }
  else
  {
    Logger::Log_Info("Loading starting positions...");
    Assign_Starting_Positions_From_Data(starting_positions);
  }
  //Game_Map.Print_Map_In_ASCII();
}

void Game::Build_Upgrade(string name, int x, int y, int player_id)
{
  Upgrade u = Get_Player_By_Id(player_id)->Find_Upgrade_By_Name(name);
  int radius = Get_Player_By_Id(player_id)->Get_Upgrade_Border_Radius();
  if(name == "City")
    Game_Map.Build_City(x,y,player_id,radius);
  else
    Game_Map.Build_Upgrade(u, x, y, player_id, radius);
  if(u.How_Many_Times_Has_Trait("borderexpand") == 0)
    radius = 0;
  vector<array<int, 2>> tmp = Main_Radius_Generator.Get_Radius_For_Coords(x,y,radius);
  Tiles_To_Update.insert(Tiles_To_Update.end(), tmp.begin(), tmp.end());
}

void Game::Assing_Starting_Position_For_Player(int player_id, int x, int y)
{
  Get_Player_By_Id(player_id)->Assign_Id(player_id);
  Logger::Log_Info("Player " + Get_Player_By_Id(player_id)->Get_Leader_Name() + " will start at position " + to_string(x) + " " + to_string(y) );
  Build_City(x,y,player_id,Get_Player_By_Id(player_id)->Get_Upgrade_Border_Radius());
}

void Game::Asign_Random_Starting_Position_For_Player(int player_id)
{
    int x = 0;
    int y = 0;
    bool loop = true;
    while(loop)
    {
      x = rand() % Game_Map.Get_X_Size();
      y = rand() % Game_Map.Get_Y_Size();
      if(Game_Map.Is_Tile_Neutral(x,y) && Game_Map.Get_Tile(x,y).Get_Name() == "Land")
      {
        Assing_Starting_Position_For_Player(player_id, x, y);
        loop = false;
      }
    }
}

void Game::Assign_Starting_Positions_From_Data(map<string, array<int, 2>> starting_positions)
{
  int player_id = 1;
  for(auto &player : Players)
  {
    if(starting_positions.count(player.Get_Raw_Name()))
    {
      int x = starting_positions[player.Get_Raw_Name()][0];
      int y = starting_positions[player.Get_Raw_Name()][1];
      Assing_Starting_Position_For_Player(player_id, x ,y);
    }
    else
    {
      Logger::Log_Warning("Could not find starting position for " + player.Get_Raw_Name() + " assuming random...");
      Asign_Random_Starting_Position_For_Player(player_id);
    }
    player_id++;
  }
}

void Game::Assign_Random_Starting_Positions()
{
  int player_id = 1;
  for(auto &player : Players)
  {
    Asign_Random_Starting_Position_For_Player(player_id);
    player_id++;
  }
}

void Game::Add_Player_By_Name(string name, bool ai)
{
  Is_Player_AI_List.push_back(ai);
  for(auto& civ : Civs)
  {
    if(civ.Get_Raw_Name() == name)
    {
      Players.push_back(civ);
      return;
    }
  }
  Logger::Log_Error("Civilization not found!");
  Logger::Log_Warning("Adding random!");
  Players.push_back(Civs[0]);
}

void Game::Add_Players(vector<tuple<string, bool>> players)
{
  currently_moving_player = 1;
  Is_Player_AI_List.push_back(0); //neutrals
  Logger::Log_Info("Adding " + to_string(players.size()) + " players!");
  for(auto& player : players)
  {
    Add_Player_By_Name(get<0>(player), get<1>(player));
  }

  Logger::Log_Info("Players added!" );
}

Game::Game(bool a, Map_Generator_Data Map_Data, vector<tuple<string, bool>> players, bool load_starting_positions) : Main_Radius_Generator(Map_Data.size_x, Map_Data.size_y)
{
  srand(time(0));
  turn_counter = 1;
  Logger::Log_Info("Starting XML Data Loading...");
  Logger::Log_Warning("TO DO: Safety, If code crashes here check XML!" );
  XML_Load_Data();
  Logger::Log_Info("XML Data Loaded!" );
  Add_Players(players);
  Generate_Map(Map_Data, load_starting_positions);

  autosave = a;
  for(auto &player : Players)
  {
    Player_Border_Colors.push_back(player.Get_Civ_Color());
  }
}

Game::Game() : Main_Radius_Generator(0,0)
{
  //in gond we truts
}

void Game::Test()
{
  Logger::Log_Info("Massive Magic" );
}

void Game::Confirm_Pass_To_Game_Window()
{
  Logger::Log_Info("Game Object Passed to Game Window!" );
}

Civ *Game::Get_Player_By_Id(int id)
{
  return &Players[id-1];
}

int Game::Get_Currently_Moving_Player_Id()

{
  return currently_moving_player;
}

Map *Game::Get_Map()
{
  return &Game_Map;
}

uint32_t Game::Get_Border_Color_By_Player_Id(int id)
{
  if(id == 0)
    return 0;
  return Player_Border_Colors[id-1];
}

int Game::Get_Turn()
{
  return turn_counter;
}

void Game::Check_For_Dead_Players()
{
  int index = 1;
  for(auto &player : Players)
  {
    if(player.Get_Number_Of_Cities_Owned() == 0 && find(Eliminated_Players_List.begin(),Eliminated_Players_List.end(), index) == Eliminated_Players_List.end())
    {
      //remove player
      Main_Newspaper.Add_News(Get_Current_Turn_By_Years(), "Goverment of " + Get_Player_By_Id(index)->Get_Full_Name() + " has fallen!");
      Get_Map()->Unclaim_All_Player_Tiles(index);
      vector<Unit_On_Map> *units = Get_Player_By_Id(index)->Get_Owned_Units();
      for(auto &unit : *units)
      {
        Get_Map()->Get_Tile_Pointer(unit.Coordinates.x,unit.Coordinates.y)->Remove_Unit_From_Tile();
        Get_Player_By_Id(index)->Remove_Unit_By_Coords(unit.Coordinates.x,unit.Coordinates.y);
      }
      Eliminated_Players_List.push_back(index);
    }
    index++;
  }
}

bool Game::Is_Player_AI(int player_id)
{
  return Is_Player_AI_List[player_id];

}

bool Game::Is_Currently_Moving_Player_AI()
{
  return Is_Player_AI(Get_Currently_Moving_Player_Id());
}

vector<array<int, 2>> Game::Get_Tiles_To_Update()
{
  vector<array<int, 2>> out = Tiles_To_Update;
  Tiles_To_Update.clear();
  return out;
}

void Game::Recruit_Unit(string u, int x, int y)
{
  Get_Map()->Get_Tile_Pointer(x,y)->Put_Unit_On_Tile(Get_Currently_Moving_Player_Id());
  Get_Currently_Moving_Player()->Recruit_Unit_By_Name(u,x,y);
}



void Game::Do_AI_Actions_For_Currently_Moving_Player()
{
  AI AI_Player(this);
  AI_Data_For_Players[Get_Currently_Moving_Player_Id()] = AI_Player.Process_Turn(AI_Data_For_Players[Get_Currently_Moving_Player_Id()]);
  End_Player_Turn();
}

void Game::Start_Turn_Of_Currently_Moving_Player()
{
  Get_Player_By_Id(currently_moving_player)->Start_Turn();
  if(Is_Currently_Moving_Player_AI())
  {
    Do_AI_Actions_For_Currently_Moving_Player();
  }
}

bool Game::Is_Map_Update_Required()
{
  bool out = false;
  for_each(Players.begin(), Players.end(), [&out](Civ& player)
{
  out = out || player.Has_Researched_Border_Expand_Tech_Recently();
});
  return out;
}

int Game::First_Not_Eliminated_Player_Id()
{
  int index = 1; //fucking neutrals
  while(index <= static_cast<int>(Players.size()))
  {
    if(! Is_Player_Eliminated(index))
      return index;
    index++;
  }
  Logger::Log_Error("All players are eliminated!");
  //fuck what?!
  throw;
}

bool Game::Is_Player_Eliminated(int player_id)
{
  if(find(Eliminated_Players_List.begin(), Eliminated_Players_List.end(), player_id) != Eliminated_Players_List.end())
    return true;
  return false;
}

bool Game::Is_Currently_Moving_Player_Eliminated()
{
  return Is_Player_Eliminated(Get_Currently_Moving_Player_Id());
}

bool Game::All_Humans_Are_Eliminated()
{
  Logger::Log_Warning("Checking for alive humans! Turn off when AI only");
  bool out = true;
  int index = 1; //fucking neutrals
  while(index <= static_cast<int>(Players.size()))
  {
    if((! Is_Player_Eliminated(index)) && (! Is_Player_AI(index)))
      out = false;
    index++;
  }
  return out;
}

void Game::Set_Autosave(bool a)
{
  autosave = a;
}

void Game::Remove_All_Missle_Units()
{
  for(auto& player : Players)
  {
    for(auto& unit : *player.Get_Owned_Units())
    {
      if(unit.Self.Has_Trait("missle"))
      {
        Get_Player_By_Id(Get_Map()->Get_Tile(unit.Coordinates.x,unit.Coordinates.y).Get_Unit_Owner_Id())->Remove_Unit_By_Coords(unit.Coordinates.x,unit.Coordinates.y);
        Get_Map()->Get_Tile_Pointer(unit.Coordinates.x,unit.Coordinates.y)->Remove_Unit_From_Tile();
      }
    }
  }
}

bool Game::End_Player_Turn()
{
  Remove_All_Missle_Units();
  vector<int> income = Get_Map()->Get_Netto_Income_For_Player_By_Id(Get_Currently_Moving_Player_Id(), *Get_Currently_Moving_Player());
  Get_Currently_Moving_Player()->End_Turn(income);
  if(Get_Currently_Moving_Player()->Has_Researched_Border_Expand_Tech_Recently())
  {
    Get_Map()->Recalculate_Borders_For_Player_By_Id(Get_Currently_Moving_Player_Id(), Get_Currently_Moving_Player()->Get_Upgrade_Border_Radius(), *Get_Currently_Moving_Player());
  }

  Check_For_Dead_Players();
  do
  {
    currently_moving_player++;
    if(currently_moving_player > (int)Players.size())
      currently_moving_player = 1;
  }
  while(Is_Currently_Moving_Player_Eliminated());

  if(All_Humans_Are_Eliminated())
    return false;

  Get_Currently_Moving_Player()->Refresh_Unit_Movement_Points();
  vector<Unit_On_Map> *u = Get_Currently_Moving_Player()->Get_Owned_Units();
  for(auto &var : *u)
  {
    var.Self.Increase_Current_Movement(Get_Currently_Moving_Player()->Find_Upgrade_By_Name(Get_Map()->Get_Tile(var.Coordinates.x, var.Coordinates.y).Get_Upgrade()).How_Many_Times_Has_Trait("movementbonus"));
    if(var.Self.Get_All_Arguments_For_Trait("class")[0] == "flying" && Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(var.Coordinates.x, var.Coordinates.y)).Has_Trait("renewflyingmovement"))
      var.Self.Increase_Current_Movement(999);
  }

  if(currently_moving_player == First_Not_Eliminated_Player_Id())
    turn_counter++;
  if(autosave && !Is_Currently_Moving_Player_AI())
    Save_Game("autosave.sav");
  Start_Turn_Of_Currently_Moving_Player();
  return true;
}

Civ* Game::Get_Currently_Moving_Player()
{
  return &Players[currently_moving_player-1];
}

vector<Upgrade> Game::Get_Upgrades()
{
  return Upgrades;
}

vector<Unit> Game::Get_Units()
{
  return Units;
}

Upgrade Game::Get_Upgrade_By_Name(string name)
{
  for(auto &upg : Upgrades)
  {
    if(upg.Get_Name() == name)
      return upg;
  }
  throw;
}

double Game::Get_Defense_Bonus_For_Tile_And_Player(int x, int y, int player_id)
{
  return Get_Map()->Get_Defense_Bonus_For_Tile(x, y) + (Get_Player_By_Id(player_id)->Get_Defense_Bonus_For_Upgrade(Get_Map()->Get_Tile(x,y).Get_Upgrade()) - 1.0);
}

array<int, 3> Game::Get_Units_Stats_For_Battle(int unit_x, int unit_y)
{
  Get_Player_By_Id(Get_Map()->Get_Tile(unit_x,unit_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile_Pointer(unit_x, unit_y)->Remove_All_Movement();
  Unit Stat_Unit = Get_Player_By_Id(Get_Map()->Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_x, unit_y);
  array<int, 3> out;
  out[0] = Stat_Unit.Get_Attack_Power();
  out[1] = Stat_Unit.Get_Defense_Power();
  out[2] = Stat_Unit.Get_HP();
  double unit_def_mod = 0.0;
  if(!Stat_Unit.Has_Trait("doesnotrecievedefensivebonus"))
    unit_def_mod = Get_Defense_Bonus_For_Tile_And_Player(unit_x, unit_y, Get_Map()->Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id());

  out[1] = static_cast<double>(out[1]) * unit_def_mod;

  if(Get_Player_By_Id(Get_Map()->Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id())->Get_Active_Goverment_Name() == "Fundamentalism")
  {
    out[1] = static_cast<double>(out[1]) * 0.9;
    out[0] = static_cast<double>(out[0]) * 0.9;
  }
  Logger::Log_Info("Unit Debug");
  Logger::Log_Info("Unit name: " + Stat_Unit.Get_Name());
  Logger::Log_Info("Unit HP: " + to_string(out[2]));
  Logger::Log_Info("Unit ATK: " + to_string(out[0]));
  Logger::Log_Info("Unit DEF: " + to_string(out[1]) + " DEF Bonus " + to_string(unit_def_mod));
  return out;
}


Unit Game::Get_Unit_By_Tile(int x, int y)
{
  return Get_Player_By_Id(Get_Map()->Get_Tile(x,y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(x,y);
}

void Game::Plunder_Tile(int x, int y)
{
  Get_Currently_Moving_Player()->Get_Unit_On_Tile_Pointer(x,y)->Increase_Current_Movement(-1);
  Get_Map()->Plunder_Tile(x,y);
}

void Game::Battle_Units(int unit_1_x, int unit_1_y, int unit_2_x, int unit_2_y)
{
  Logger::Log_Info("Battle Debug");
  array<int, 3> unit_1_stats = Get_Units_Stats_For_Battle(unit_1_x, unit_1_y);
  array<int, 3> unit_2_stats = Get_Units_Stats_For_Battle(unit_2_x, unit_2_y);

  if(Get_Unit_By_Tile(unit_1_x,unit_1_y).Get_All_Arguments_For_Trait("class")[0] == "flying")
    unit_2_stats[0] = unit_2_stats[0] * Get_Unit_By_Tile(unit_2_x, unit_2_x).How_Many_Times_Has_Trait("fightflying");

  if(Get_Unit_By_Tile(unit_2_x,unit_2_y).Get_All_Arguments_For_Trait("class")[0] == "flying")
    unit_2_stats[0] = unit_2_stats[0] * Get_Unit_By_Tile(unit_1_x, unit_1_x).How_Many_Times_Has_Trait("fightflying");

  unit_1_stats[2] = unit_1_stats[2] - ((double) 30 * ((double) unit_2_stats[0] / (double) unit_1_stats[1]));
  unit_2_stats[2] = unit_2_stats[2] - ((double) 30 * ((double) unit_1_stats[0] / (double) unit_2_stats[1]));
  Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile_Pointer(unit_1_x, unit_1_y)->Set_HP(unit_1_stats[2]);
  Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile_Pointer(unit_2_x, unit_2_y)->Set_HP(unit_2_stats[2]);
  if(unit_1_stats[2] <= 0 || Get_Unit_By_Tile(unit_1_x,unit_1_y).Get_All_Arguments_For_Trait("class")[0] == "missle")
  {
    Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Remove_Unit_By_Coords(unit_1_x,unit_1_y);
    Get_Map()->Get_Tile_Pointer(unit_1_x,unit_1_y)->Remove_Unit_From_Tile();
    if(Get_Unit_By_Tile(unit_2_x, unit_2_y).Has_Trait("steal") && unit_2_stats[2] > 0)
      Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Give_One_Gold();
  }
  if(unit_2_stats[2] <= 0 || Get_Unit_By_Tile(unit_2_x,unit_2_y).Get_All_Arguments_For_Trait("class")[0] == "missle")
  {
    Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Remove_Unit_By_Coords(unit_2_x,unit_2_y);
    Get_Map()->Get_Tile_Pointer(unit_2_x,unit_2_y)->Remove_Unit_From_Tile();
    if(Get_Unit_By_Tile(unit_1_x, unit_1_y).Has_Trait("steal") && unit_1_stats[2] > 0)
      Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Give_One_Gold();
  }
}

void Game::Move_Unit(int unit_x, int unit_y, int dest_x, int dest_y, int cost)
{
  int player_id = Get_Map()->Get_Tile(unit_x,unit_y).Get_Unit_Owner_Id();
  Get_Player_By_Id(player_id)->Move_Unit_To_By_Coords(unit_x, unit_y, dest_x, dest_y, cost);
  Get_Map()->Get_Tile_Pointer(unit_x, unit_y)->Remove_Unit_From_Tile();
  Get_Map()->Get_Tile_Pointer(dest_x, dest_y)->Put_Unit_On_Tile(player_id);
}

bool Game::Has_Currently_Moving_Player_Any_Actions_Left()
{
  return (bool) Get_Currently_Moving_Player()->Get_Current_Actions();
}

bool Game::Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(int unit_x, int unit_y, int dest_x, int dest_y, int movement_cost, bool combat, int enemy_unit_x, int enemy_unit_y)
{
  int unit_owner_id = Get_Map()->Get_Tile(unit_x,unit_y).Get_Unit_Owner_Id();
  int tile_owner_id = Get_Map()->Get_Owner(dest_x, dest_y);
  Tiles_To_Update.push_back({unit_x, unit_y});
  Tiles_To_Update.push_back({dest_x, dest_y});
  Tiles_To_Update.push_back({enemy_unit_x, enemy_unit_y});
  bool is_map_update_necesary = false;
  Move_Unit(unit_x, unit_y, dest_x, dest_y, movement_cost);
  if(unit_owner_id != tile_owner_id)
  {
    if(Get_Map()->Get_Owner(dest_x, dest_y) != 0 && Get_Player_By_Id(Get_Map()->Get_Owner(dest_x, dest_y))->Find_Upgrade_By_Name(Get_Map()->Get_Tile(dest_x, dest_y).Get_Upgrade()).Has_Trait("borderexpand"))
    {
      is_map_update_necesary = true;
      Get_Map()->Change_Tile_Owner(dest_x, dest_y, unit_owner_id);
      Get_Map()->Retake_Owner_In_Radius_From(dest_x, dest_y, unit_owner_id, Get_Player_By_Id(unit_owner_id)->Get_Upgrade_Border_Radius(), tile_owner_id);
      if(Get_Map()->Get_Tile(dest_x, dest_y).Get_Upgrade() == "City")
      {
        string city_name = Get_Player_By_Id(tile_owner_id)->Lose_City_By_Coords(dest_x, dest_y);
        Get_Player_By_Id(unit_owner_id)->Build_City_On_Map_With_Name(dest_x, dest_y, city_name);
        string message = Get_Player_By_Id(unit_owner_id)->Get_Full_Name() + " has conquered " + city_name;
        Main_Newspaper.Add_News(Get_Current_Turn_By_Years(), message);
        Logger::Log_Info(message + " X: " + to_string(dest_x) + " Y: " + to_string(dest_y));
      }
    }
  }
  if(combat)
  {
    Battle_Units(dest_x, dest_y, enemy_unit_x, enemy_unit_y);
  }
  return is_map_update_necesary;
}

vector<Civ> Game::Get_All_Civs()
{
  return Civs;
}

void Game::Detonate_Atomic_Bomb(int x, int y)
{
  Main_Newspaper.Add_News(Get_Current_Turn_By_Years(), Get_Currently_Moving_Player()->Get_Full_Name() + " has dropped atomic bomb on tile X: " + to_string(x) + " Y: " + to_string(y) + "!");
  Disband_Unit(x,y);
  if(Get_Map()->Get_Tile(x+1,y).Has_Unit())
    Disband_Unit(x+1,y);
  if(Get_Map()->Get_Tile(x+1,y-1).Has_Unit())
    Disband_Unit(x+1,y-1);
  if(Get_Map()->Get_Tile(x+1,y+1).Has_Unit())
    Disband_Unit(x+1,y+1);
  if(Get_Map()->Get_Tile(x-1,y).Has_Unit())
    Disband_Unit(x-1,y);
  if(Get_Map()->Get_Tile(x-1,y+1).Has_Unit())
    Disband_Unit(x-1,y+1);
  if(Get_Map()->Get_Tile(x-1,y-1).Has_Unit())
    Disband_Unit(x-1,y-1);
  if(Get_Map()->Get_Tile(x,y+1).Has_Unit())
    Disband_Unit(x,y+1);
  if(Get_Map()->Get_Tile(x,y-1).Has_Unit())
    Disband_Unit(x,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x, y)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x,y);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x+1, y)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x+1,y);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x+1, y-1)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x+1,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x+1, y+1)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x+1,y+1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x-1, y)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x-1,y);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x-1, y-1)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x-1,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x-1, y+1)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x-1,y+1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x, y-1)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map()->Get_Upgrade(x, y+1)).Has_Trait("cannotbeplundered"))
    Get_Map()->Plunder_Tile(x,y+1);
}

string Game::Get_Current_Turn_By_Years()
{
  string out;
  int starting_year = -4000;
  int current_year = starting_year + (turn_counter * 25);
  out = out + to_string(current_year);
  if(current_year < 0)
    out = out + " BC";
  else
    out = out + " AD";
  return out;
}

Game::Game(xml_node<>* Root_Node) : Game_Map(Root_Node->first_node("map")), Main_Radius_Generator(Get_Map()->Get_X_Size(), Get_Map()->Get_Y_Size())
{
  Logger::Log_Info("Deserializing Game...");
  Deserialize(Root_Node);
}

void Game::Deserialize(xml_node<>* Root_Node)
{
  currently_moving_player = stoi(Root_Node->first_attribute("currently_moving_player")->value());
  turn_counter = stoi(Root_Node->first_attribute("turn")->value());

  Logger::Log_Info("Deserializing Tiles...");
  xml_node<>* Tiles_Node = Root_Node->first_node("tiles");
  for(xml_node<> *Tile_Node = Tiles_Node->first_node("tile"); Tile_Node; Tile_Node = Tile_Node->next_sibling("tile"))
  {
    Tile tmp(Tile_Node);
    Tiles.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Upgrades...");
  xml_node<>* Upgrades_Node = Root_Node->first_node("upgrades");
  for(xml_node<> *Upgrade_Node = Upgrades_Node->first_node("upgrade"); Upgrade_Node; Upgrade_Node = Upgrade_Node->next_sibling("upgrade"))
  {
    Upgrade tmp(Upgrade_Node);
    Upgrades.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Technologies...");
  xml_node<>* Technologies_Node = Root_Node->first_node("techs");
  for(xml_node<> *Technology_Node = Technologies_Node->first_node("tech"); Technology_Node; Technology_Node = Technology_Node->next_sibling("tech"))
  {
    Tech tmp(Technology_Node);
    Technologies.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Units...");
  xml_node<>* Units_Node = Root_Node->first_node("units");
  for(xml_node<> *Unit_Node = Units_Node->first_node("unit"); Unit_Node; Unit_Node = Unit_Node->next_sibling("unit"))
  {
    Unit tmp(Unit_Node);
    Units.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Goverments...");
  xml_node<>* Goverments_Node = Root_Node->first_node("goverments");
  for(xml_node<> *Goverment_Node = Goverments_Node->first_node("goverment"); Goverment_Node; Goverment_Node = Goverment_Node->next_sibling("goverment"))
  {
    Gov tmp(Goverment_Node);
    Goverments.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Player Border Colors...");
  xml_node<>* Player_Border_Colors_Node = Root_Node->first_node("player_border_colors");
  for(xml_node<> *Color_Node = Player_Border_Colors_Node->first_node("color"); Color_Node; Color_Node = Color_Node->next_sibling("color"))
  {
    uint32_t tmp = static_cast<uint32_t>(stoul(Color_Node->first_attribute("value")->value()));
    Logger::Log_Warning(Color_Node->first_attribute("value")->value());
    Player_Border_Colors.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Civs...");
  xml_node<>* Civs_Node = Root_Node->first_node("civs");
  for(xml_node<> *Civ_Node = Civs_Node->first_node("civ"); Civ_Node; Civ_Node = Civ_Node->next_sibling("civ"))
  {
    Civ tmp(Civ_Node);
    Civs.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Players...");
  xml_node<>* Players_Node = Root_Node->first_node("players");
  for(xml_node<> *Player_Node = Players_Node->first_node("civ"); Player_Node; Player_Node = Player_Node->next_sibling("civ"))
  {
    Civ tmp(Player_Node);
    Players.push_back(tmp);
  }

  xml_node<>* AI_Node = Root_Node->first_node("ai_list");
  for(xml_node<> *Player_Node = AI_Node->first_node("ai"); Player_Node; Player_Node = Player_Node->next_sibling("ai"))
  {
    Is_Player_AI_List.push_back((bool) stoi(Player_Node->value()));
  }

  xml_node<>* Eliminated_Players_List_Node = Root_Node->first_node("eliminated_list");
  for(xml_node<> *Eliminated_Player_Node = Eliminated_Players_List_Node->first_node("player_id"); Eliminated_Player_Node; Eliminated_Player_Node = Eliminated_Player_Node->next_sibling("player_id"))
  {
    Eliminated_Players_List.push_back(stoi(Eliminated_Player_Node->value()));
  }

  xml_node<>* AI_Data_Node = Root_Node->first_node("ai_data_list");
  for(xml_node<> *Player_Node = AI_Data_Node->first_node("ai_data"); Player_Node; Player_Node = Player_Node->next_sibling("ai_data"))
  {
    AI_Data tmp;
    tmp.expanse_parameter = stoi(Player_Node->first_attribute("exspanse")->value());
    tmp.is_at_war = (bool) stoi(Player_Node->first_attribute("is_at_war")->value());
    tmp.war_target_id = stoi(Player_Node->first_attribute("war_target_id")->value());
    AI_Data_For_Players[stoi(Player_Node->first_attribute("player_id")->value())] = tmp;
  }
}

xml_node<>*  Game::Serialize(memory_pool<>* doc)
{
  Logger::Log_Info("Saving Game...");
  xml_node<> *Root_Node = doc->allocate_node(node_element, "miniyacg");
  xml_node<> *Game_Node = doc->allocate_node(node_element, "game");

  xml_attribute<> *Turn_Counter = doc->allocate_attribute("turn", doc->allocate_string(to_string(turn_counter).c_str()));
  Game_Node->append_attribute(Turn_Counter);
  xml_attribute<> *Currently_Moving_Player_Counter = doc->allocate_attribute("currently_moving_player", doc->allocate_string(to_string(currently_moving_player).c_str()));

  xml_node<> *Tiles_Node = doc->allocate_node(node_element, "tiles");
  xml_node<> *Upgrades_Node = doc->allocate_node(node_element, "upgrades");
  xml_node<> *Technologies_Node = doc->allocate_node(node_element, "techs");
  xml_node<> *Units_Node = doc->allocate_node(node_element, "units");
  xml_node<> *Govs_Node = doc->allocate_node(node_element, "goverments");
  xml_node<> *Player_Border_Colors_Node = doc->allocate_node(node_element, "player_border_colors");
  xml_node<> *Civs_Node = doc->allocate_node(node_element, "civs");
  xml_node<> *Players_Node = doc->allocate_node(node_element, "players");

  for_each(Tiles.begin(), Tiles.end(), [&](Tile& iterated_tile)
  {
    Logger::Log_Info("Serializing " + iterated_tile.Get_Name() );
    Tiles_Node->append_node(iterated_tile.Serialize(doc));
  });

  for_each(Upgrades.begin(), Upgrades.end(), [&](Upgrade& iterated_upgrade)
  {
    Logger::Log_Info("Serializing " + iterated_upgrade.Get_Name() );
    Upgrades_Node->append_node(iterated_upgrade.Serialize(doc));
  });

  for_each(Technologies.begin(), Technologies.end(), [&](Tech& iterated_tech)
  {
    Logger::Log_Info("Serializing " + iterated_tech.Get_Name() );
    Technologies_Node->append_node(iterated_tech.Serialize(doc));
  });

  for_each(Units.begin(), Units.end(), [&](Unit& iterated_unit)
  {
    Logger::Log_Info("Serializing " + iterated_unit.Get_Name() );
    Units_Node->append_node(iterated_unit.Serialize(doc));
  });

  for_each(Goverments.begin(), Goverments.end(), [&](Gov& iterated_gov)
  {
    Logger::Log_Info("Serializing " + iterated_gov.Get_Name() );
    Govs_Node->append_node(iterated_gov.Serialize(doc));
  });

  for_each(Players.begin(), Players.end(), [&](Civ& iterated_civ)
  {
    Logger::Log_Info("Serializing " + iterated_civ.Get_Name() );
    Players_Node->append_node(iterated_civ.Serialize(doc));
  });

  for_each(Civs.begin(), Civs.end(), [&](Civ& iterated_civ)
  {
    Logger::Log_Info("Serializing " + iterated_civ.Get_Name() );
    Civs_Node->append_node(iterated_civ.Serialize(doc));
  });

  xml_node<>* AI_Node = doc->allocate_node(node_element, "ai_list");

  for_each(Is_Player_AI_List.begin(), Is_Player_AI_List.end(), [&](bool iterated_player)
  {
    xml_node<> *ai = doc->allocate_node(node_element, "ai", doc->allocate_string(to_string(iterated_player).c_str()));
    AI_Node->append_node(ai);
  });

  xml_node<>* Eliminated_Players_List_Node = doc->allocate_node(node_element, "eliminated_list");

  for_each(Eliminated_Players_List.begin(), Eliminated_Players_List.end(), [&](int iterated_player)
  {
    xml_node<> *ai = doc->allocate_node(node_element, "player_id", doc->allocate_string(to_string(iterated_player).c_str()));
    Eliminated_Players_List_Node->append_node(ai);
  });


  xml_node<>* AI_Data_Node = doc->allocate_node(node_element, "ai_data_list");

  for(auto &ai_data_var : AI_Data_For_Players)
  {
    xml_node<> *player_node = doc->allocate_node(node_element, "ai_data");
    xml_attribute<> *player_name = doc->allocate_attribute("player_id", doc->allocate_string(to_string(ai_data_var.first).c_str()));
    player_node->append_attribute(player_name);
    xml_attribute<> *expanse = doc->allocate_attribute("exspanse", doc->allocate_string(to_string(ai_data_var.second.expanse_parameter).c_str()));
    player_node->append_attribute(expanse);
    xml_attribute<> *war_target_id = doc->allocate_attribute("war_target_id", doc->allocate_string(to_string(ai_data_var.second.war_target_id).c_str()));
    player_node->append_attribute(war_target_id);
    xml_attribute<> *is_at_war = doc->allocate_attribute("is_at_war", doc->allocate_string(to_string(ai_data_var.second.is_at_war).c_str()));
    player_node->append_attribute(is_at_war);
    AI_Data_Node->append_node(player_node);
  }

  for_each(Player_Border_Colors.begin(), Player_Border_Colors.end(), [&](uint32_t& iterated_color)
  {
    xml_node<> *Color_Node = doc->allocate_node(node_element, "color");
    xml_attribute<> *Color = doc->allocate_attribute("value", doc->allocate_string(to_string(iterated_color).c_str()));
    Color_Node->append_attribute(Color);
    Player_Border_Colors_Node->append_node(Color_Node);
  });

  Game_Node->append_node(Game_Map.Serialize(doc));
  Game_Node->append_attribute(Currently_Moving_Player_Counter);
  Game_Node->append_node(Tiles_Node);
  Game_Node->append_node(Units_Node);
  Game_Node->append_node(Govs_Node);
  Game_Node->append_node(Civs_Node);
  Game_Node->append_node(AI_Data_Node);
  Game_Node->append_node(Eliminated_Players_List_Node);
  Game_Node->append_node(AI_Node);
  Game_Node->append_node(Players_Node);
  Game_Node->append_node(Player_Border_Colors_Node);
  Game_Node->append_node(Upgrades_Node);
  Game_Node->append_node(Technologies_Node);
  Root_Node->append_node(Game_Node);
  //xml_document<> Document;
  //Document.append_node(Root_Node);
  return Root_Node;
}

bool Game::Save_Game(string path)
{
  Game_Saver Saver(this);
  return Saver.Save_Game(path);
}

tuple<bool, Game*> Game::Load_Game(string path)
{
  Game_Loader Loader;
  return Loader.Load_Game(path);
}

vector<string> Game::Get_Newspaper_Events()
{
  return Main_Newspaper.Get_News();
}

void Game::Change_Goverment_For_Currently_Moving_Player_By_Name(string name)
{
  if(Get_Currently_Moving_Player()->Get_Current_Actions() == Get_Currently_Moving_Player()->Get_Max_Actions())
  {
    string message = "Riots in " + Get_Currently_Moving_Player()->Get_Capital_Name() + "!";
    message = message + " " + Get_Currently_Moving_Player()->Get_Leader_Name() + " has been killed.";
    message = message + " " + Get_Currently_Moving_Player()->Get_Raw_Name() + " has been proclaimed ";
    Get_Currently_Moving_Player()->Change_Goverment_By_Name(name);
    message = message + Get_Currently_Moving_Player()->Get_Full_Name();
    Main_Newspaper.Add_News(Get_Current_Turn_By_Years(), message);
  }
}

void Game::Build_City(int x, int y, int owner, int radius)
{
  string name = "City";
  Build_Upgrade(name, x, y, owner);
  Get_Player_By_Id(owner)->Build_City_On_Map(x,y);
  string message = Get_Player_By_Id(owner)->Get_Full_Name() + " has settled new city of " + Get_Player_By_Id(owner)->Get_City_Name_By_Coordinates(x,y);
  Main_Newspaper.Add_News(Get_Current_Turn_By_Years(), message);
}

int Game::Get_Total_Cost_Of_Technology_By_Name(string name)
{
  auto it = find_if(Technologies.begin(), Technologies.end(), [&name](Tech& t){
    return t.Get_Name() == name;
  });
  if(it != Technologies.end())
  {
    Tech tmp = *it;
    return tmp.Get_Cost();
  }
  Logger::Log_Error("Technology not found!");
  throw;
}

Unit Game::Get_Unit_By_Name(string name)
{
  auto it = find_if(Units.begin(), Units.end(), [&name](Unit& u){
    return u.Get_Name() == name;
  });
  Unit out = *it;
  return out;
}

void Game::Disband_Unit(int x, int y)
{
  Get_Map()->Get_Tile_Pointer(x,y)->Remove_Unit_From_Tile();
  Get_Currently_Moving_Player()->Remove_Unit_By_Coords(x,y);
}

vector<Civ> Game::Get_Players()
{
  return Players;
}

Upgrade Game::Get_Upgrade_Of_Currently_Moving_Player(string upg_name)
{
  return Get_Currently_Moving_Player()->Find_Upgrade_By_Name(upg_name);
}
