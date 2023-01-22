#include "game.h"

int Game::Get_Amount_Of_Players() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Players.size();
}

void Game::XML_Load_Data()
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  try
  {
    XML_Data_Loader Loader(" ");
    Tiles = Loader.Load_Tiles();
    Technologies = Loader.Load_Techs();
    Units = Loader.Load_Units();
    Goverments = Loader.Load_Govs();
    Civs = Loader.Load_Civs();
    Upgrades = Loader.Load_Upgrades();
    vector<Culture> Cultures_Vector = Loader.Load_Cultures();
    for_each(Cultures_Vector.begin(), Cultures_Vector.end(), [&](Culture& tmp){Cultures[tmp.Get_Name()] = tmp;});
  }
  catch(...)
  {
    Logger::Log_Error("Failed to load XML Data files!");
  }
}


void Game::Generate_Map(Map_Generator_Data User_Data, bool load_starting_positions)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
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

Culture& Game::Get_Culture_By_Player_Id(int player_id)
{
  if(!Cultures.count(Get_Player_By_Id(player_id).Get_Culture_Name()))
  {
    Logger::Log_Error("Culture " + string(Get_Player_By_Id(player_id).Get_Culture_Name()) + "not found");
    return Cultures["european"];
  }
  return Cultures[Get_Player_By_Id(player_id).Get_Culture_Name()];
}

Culture& Game::Get_Culture_By_Player_Id(int player_id) const
{
  if(!Cultures.count(Get_Player_By_Id(player_id).Get_Culture_Name()))
  {
    Logger::Log_Error("Culture " + string(Get_Player_By_Id(player_id).Get_Culture_Name()) + "not found");
    return Cultures["european"];
  }
  return Cultures[Get_Player_By_Id(player_id).Get_Culture_Name()];
}

string_view Game::Get_Texture_Path_For_Cultured_Upgrade(int x, int y, string_view upg_name) const
{
  return Get_Culture_By_Player_Id(Get_Map().Get_Owner(x,y)).Get_Texture_For_Upgrade(upg_name);
}

vector<array<int, 3>> Game::Search_For_Connections(array<int, 2> Coords, int player_id) const
{
  vector<string> City_Connection_Upgrades = Get_Player_By_Id(player_id).Get_All_Upgrade_Names_By_Trait("cityconnection");
  vector<array<int, 2>> Cities_To_Connect;
  vector<array<int, 2>> Tiles_To_Check;
  vector<array<int, 2>> Tiles_Checked;
  Tiles_To_Check.push_back(Coords);
  do
  {
    if(find(Tiles_Checked.begin(), Tiles_Checked.end(), Tiles_To_Check[0]) != Tiles_Checked.end())
    {
      Tiles_To_Check.erase(Tiles_To_Check.begin());
      continue;
    }
    for(auto& upg_name : City_Connection_Upgrades)
    {
      vector<array<int,2>> Tmp = Get_Map().Find_All_Upgrade_Locations_In_Radius(Tiles_To_Check[0], player_id, 2, upg_name);
      Tiles_To_Check.insert(Tiles_To_Check.end(), Tmp.begin(), Tmp.end());
    }
    vector<array<int,2>> Cities = Get_Map().Find_All_Upgrade_Locations_In_Radius(Tiles_To_Check[0], player_id, 2, "City");
    Cities_To_Connect.insert(Cities_To_Connect.end(), Cities.begin(), Cities.end());
    Tiles_To_Check.insert(Tiles_To_Check.end(), Cities.begin(), Cities.end());
    Tiles_Checked.push_back(Tiles_To_Check[0]);
    Tiles_To_Check.erase(Tiles_To_Check.begin());
  } while(Tiles_To_Check.size());
  vector<array<int, 3>> out;
  for(auto& City : Cities_To_Connect)
    out.push_back({City[0], City[1], player_id});
  return out;
}

void Game::Build_Upgrade(string name, int x, int y, int player_id)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  if(!is_in_thread)
  Sound_Manager::Play_Sound("assets/sounds/upgrade-audio.mp3");
  Upgrade u = Get_Player_By_Id(player_id).Find_Upgrade_By_Name(name);
  int radius = Get_Player_By_Id(player_id).Get_Upgrade_Border_Radius();
  if(name == "City")
    Game_Map.Build_City(x,y,player_id,radius);
  else
    Game_Map.Build_Upgrade(u, x, y, player_id, radius);
  if(u.How_Many_Times_Has_Trait("borderexpand") == 0)
    radius = 0;
  vector<array<int, 2>> tmp = Main_Radius_Generator.Get_Radius_For_Coords(x,y,radius);
  Get_Player_By_Id(player_id).Build_Upgrade(name);
  if(u.How_Many_Times_Has_Trait("cityconnection"))
  {
    vector<array<int, 3>> Cities_With_Owners_To_Connect = Search_For_Connections({x,y}, player_id);
    for(auto& City : Cities_With_Owners_To_Connect)
    {
      Get_Player_By_Id(City[2]).Get_City_By_Coordinates({City[0], City[1]}).Connect_City();
    }
  }
  Tiles_To_Update.insert(Tiles_To_Update.end(), tmp.begin(), tmp.end());
}

void Game::Assing_Starting_Position_For_Player(int player_id, int x, int y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Get_Player_By_Id(player_id).Assign_Id(player_id);
  Logger::Log_Info("Player " + Get_Player_By_Id(player_id).Get_Leader_Name() + " will start at position " + to_string(x) + " " + to_string(y) );
  Build_City(x,y,player_id,Get_Player_By_Id(player_id).Get_Upgrade_Border_Radius());
}

void Game::Asign_Random_Starting_Position_For_Player(int player_id)
{
    //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
    int x = 0;
    int y = 0;
    bool loop = true;
    if(player_id == 1 && spectator_mode)
    {
      Assing_Starting_Position_For_Player(player_id, 9999, 9999);
      return;
    }
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
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  int player_id = 1;
  if(player_id == 1 && spectator_mode)
  {
    Assing_Starting_Position_For_Player(player_id, 9999, 9999);
    //player_id++;
  }
  for(auto &player : Players)
  {
    if(spectator_mode && player_id == 1)
    {
      player_id++;
      continue;
    }
    if(starting_positions.count(player.Get_Raw_Name().data()))
    {
      int x = starting_positions[player.Get_Raw_Name().data()][0];
      int y = starting_positions[player.Get_Raw_Name().data()][1];
      Assing_Starting_Position_For_Player(player_id, x ,y);
    }
    else
    {
      Logger::Log_Warning("Could not find starting position for " + string(player.Get_Raw_Name()) + " assuming random...");
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
    //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
    Asign_Random_Starting_Position_For_Player(player_id);
    player_id++;
  }
}

void Game::Add_Player_By_Name(string_view name, bool ai)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
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
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  currently_moving_player = 1;
  Is_Player_AI_List.push_back(0); //neutrals
  Logger::Log_Info("Adding " + to_string(players.size()) + " players!");
  for(auto& player : players)
  {
    Add_Player_By_Name(get<0>(player), get<1>(player));
  }

  Logger::Log_Info("Players added!" );
}

Game::Game(bool a, Map_Generator_Data Map_Data, vector<tuple<string, bool>> players, bool load_starting_positions, bool spectator_mode) : Main_Radius_Generator(Map_Data.size_x, Map_Data.size_y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  srand(time(0));
  this->spectator_mode = spectator_mode;
  turn_counter = 1;
  Logger::Log_Info("Starting XML Data Loading...");
  Logger::Log_Warning("TO DO: Safety, If code crashes here check XML!" );
  XML_Load_Data();
  Logger::Log_Info("XML Data Loaded!" );
  Add_Players(players);
  Generate_Map(Map_Data, load_starting_positions);
  Main_Radius_Generator.Set_Size(Get_Map().Get_X_Size(), Get_Map().Get_Y_Size());
  autosave = a;
  for(auto &player : Players)
  {
    Player_Border_Colors.push_back(player.Get_Civ_Color());
  }
}

Game::Game() : Main_Radius_Generator(0,0)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  //in gond we truts
}

Civ& Game::Get_Player_By_Id(int id)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Players[id-1];
}

Civ& Game::Get_Player_By_Id(int id) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Players[id-1];
}

int Game::Get_Currently_Moving_Player_Id() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return currently_moving_player;
}

Map& Game::Get_Map()
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Game_Map;
}

Map& Game::Get_Map() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Game_Map;
}

uint32_t Game::Get_Border_Color_By_Player_Id(int id) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  if(id == 0)
    return 0;
  return Player_Border_Colors[id-1];
}

int Game::Get_Turn() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return turn_counter;
}

void Game::Check_For_Dead_Players()
{
  int index = 1;
  for(auto &player : Players)
  {
    //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
    if(player.Get_Number_Of_Cities_Owned() == 0 && find(Eliminated_Players_List.begin(),Eliminated_Players_List.end(), index) == Eliminated_Players_List.end())
    {
      //remove player
      Main_Newspaper.Add_Revolt(Get_Current_Turn_By_Years(), "Goverment of " + Get_Player_By_Id(index).Get_Full_Name() + " has fallen!", index);
      vector<array<int, 2>> tmp = Get_Map().Unclaim_All_Player_Tiles(index);
      Tiles_To_Update.insert(Tiles_To_Update.end(), tmp.begin(), tmp.end());
      auto units = Get_Player_By_Id(index).Get_Owned_Units();
      for(auto &unit : units)
      {
        Get_Map().Get_Tile_Pointer(unit.Coordinates.x,unit.Coordinates.y).Remove_Unit_From_Tile();
        Get_Player_By_Id(index).Remove_Unit_By_Coords(unit.Coordinates.x,unit.Coordinates.y);
        Tiles_To_Update.push_back({unit.Coordinates.x, unit.Coordinates.y});
      }
      Eliminated_Players_List.push_back(index);
    }
    index++;
  }
}

bool Game::Is_Player_AI(int player_id) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Is_Player_AI_List[player_id];
}

bool Game::Is_Currently_Moving_Player_AI() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Is_Player_AI(Get_Currently_Moving_Player_Id());
}

vector<array<int, 2>> Game::Get_Tiles_To_Update()
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  vector<array<int, 2>> out = Tiles_To_Update;
  Tiles_To_Update.clear();
  return out;
}

void Game::Recruit_Unit(string_view u, int x, int y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Get_Map().Get_Tile_Pointer(x,y).Put_Unit_On_Tile(Get_Currently_Moving_Player_Id());
  Get_Currently_Moving_Player().Recruit_Unit_By_Name(u,x,y);
  Tiles_To_Update.push_back({x,y});
}



void Game::Do_AI_Actions_For_Currently_Moving_Player(Magic_Thread_Communicator *Thread_Portal)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  AI AI_Player(this);
  AI_Data_For_Players[Get_Currently_Moving_Player_Id()] = AI_Player.Process_Turn(AI_Data_For_Players[Get_Currently_Moving_Player_Id()]);
  End_Player_Turn(Thread_Portal);
}

void Game::Start_Turn_Of_Currently_Moving_Player(Magic_Thread_Communicator *Thread_Portal)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Get_Player_By_Id(currently_moving_player).Start_Turn();
  if(Is_Currently_Moving_Player_AI())
  {
    Do_AI_Actions_For_Currently_Moving_Player(Thread_Portal);
  }
}

bool Game::Is_Map_Update_Required() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  bool out = false;
  for_each(Players.begin(), Players.end(), [&out](Civ& player)
{
  out = out || player.Has_Researched_Border_Expand_Tech_Recently();
});
  return out;
}

int Game::First_Not_Eliminated_Player_Id() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
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

bool Game::Is_Player_Eliminated(int player_id) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  if(find(Eliminated_Players_List.begin(), Eliminated_Players_List.end(), player_id) != Eliminated_Players_List.end())
    return true;
  return false;
}

bool Game::Is_Currently_Moving_Player_Eliminated() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Is_Player_Eliminated(Get_Currently_Moving_Player_Id());
}

bool Game::All_Humans_Are_Eliminated() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  //Logger::Log_Warning("Checking for alive humans! Turn off when AI only");
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

bool Game::Is_Only_One_Player_Alive() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  int player_count = 0;
  int index = 1; //fucking neutrals
  while(index <= static_cast<int>(Players.size()))
  {
    if((! Is_Player_Eliminated(index)))
      player_count++;
    index++;
  }
  if(player_count == 1)
    return true;
  return false;
}

int Game::Get_Only_Living_Player_Id() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  if(!Is_Only_One_Player_Alive())
    return Get_Currently_Moving_Player_Id();

  int index = 1; //fucking neutrals
  while(index <= static_cast<int>(Players.size()))
  {
    if((! Is_Player_Eliminated(index)))
      return index;
    index++;
  }
  Logger::Log_Error("One player is alive but no one was found!");
  return 1;
}

void Game::Set_Autosave(bool a)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  autosave = a;
}

void Game::Remove_All_Missle_Units()
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  for(auto& player : Players)
  {
    for(auto& unit : player.Get_Owned_Units())
    {
      if(unit.Self.Has_Trait("missle"))
      {
        Get_Player_By_Id(Get_Map().Get_Tile(unit.Coordinates.x,unit.Coordinates.y).Get_Unit_Owner_Id()).Remove_Unit_By_Coords(unit.Coordinates.x,unit.Coordinates.y);
        Get_Map().Get_Tile_Pointer(unit.Coordinates.x,unit.Coordinates.y).Remove_Unit_From_Tile();
        Tiles_To_Update.push_back({unit.Coordinates.x, unit.Coordinates.y});
      }
    }
  }
}

void Game::Update_Stability_For_Currently_Moving_Player()
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  auto Player_Cities = Get_Currently_Moving_Player().Get_Owned_Cities();
  vector<array<int, 2>> Rebel_Cities;
  for(auto& City : Player_Cities)
  {
    if(rand() % 150 == 149)
    {
      int catastrophe_type = rand() % 3;
      string message = "";
      switch(catastrophe_type)
      {
        case 0:
          City.Drought();
          message = "Drought in ";
        case 1:
          City.Flood();
          message = "Flood in ";
        case 2:
          City.Epidemy();
          message = "Epidemy in ";
      }
      message = message + City.Get_Name() + "!";
      Main_Newspaper.Add_Catastrophe(Get_Current_Turn_By_Years(), message, Get_Currently_Moving_Player_Id());
    }
    if(City.Does_Rebel())
      Rebel_Cities.push_back(City.Get_Coords());
  }
  if(rand() % 1000 == 69)
  {
    string message = Get_Currently_Moving_Player().Get_Leader_Name() + " killed in " + string(Get_Currently_Moving_Player().Get_Name()) + ", situation in the country is unstable!";
    Main_Newspaper.Add_Assassination(Get_Current_Turn_By_Years(), message, Get_Currently_Moving_Player_Id());
    for_each(Player_Cities.begin(), Player_Cities.end(), [](auto& City){City.Change_Stability(-20, false);});
  }
  if(Get_Currently_Moving_Player().Get_Gold() < 0)
    for_each(Player_Cities.begin(), Player_Cities.end(), [](auto& City){City.Change_Stability(-5, false);});
  if(Rebel_Cities.size())
  {
    array<string, 2> Name_And_Flag = Get_Currently_Moving_Player().Get_Rebellion_Name_And_Flag_Path();
    Civ Rebels(Name_And_Flag[0], {Get_Culture_By_Player_Id(Get_Currently_Moving_Player_Id()).Get_Random_Leader_Name()}, " ", {Get_Culture_By_Player_Id(Get_Currently_Moving_Player_Id()).Get_Random_City_Name()}, Get_Currently_Moving_Player().Get_Tech_Tree(), Units, rand() % 256, rand() % 256, rand() % 256, {}, Goverments, {}, "Holy", Upgrades, Name_And_Flag[1], Get_Currently_Moving_Player().Get_Audio_Path(), Get_Currently_Moving_Player().Get_Culture_Name(), {});
    Players.push_back(Rebels);
    Player_Border_Colors.push_back(Rebels.Get_Civ_Color());
    Players[Players.size() - 1].Assign_Id(Players.size());
    Is_Player_AI_List.push_back(true);
    Players[Players.size() - 1].Change_Goverment_By_Name(Players[rand() % (Players.size() - 1)].Get_Name(), Get_Culture_By_Player_Id(Get_Currently_Moving_Player_Id()).Get_Random_Leader_Name());
    string message = "Unrest in " + Get_Currently_Moving_Player().Get_Full_Name() + "! Rebels take control. " + Players[Players.size() - 1].Get_Full_Name() + " is proclaimed!";
    Main_Newspaper.Add_Rebellion(Get_Current_Turn_By_Years(), message, Get_Currently_Moving_Player_Id());
    for(auto& Coords : Rebel_Cities)
    {
      if(Get_Map().Get_Tile(Coords[0],Coords[1]).Has_Unit())
        Disband_Unit(Coords[0], Coords[1]);
      Get_Map().Change_Tile_Owner(Coords[0], Coords[1], Players.size());
      vector<array<int, 2>> tmp = Main_Radius_Generator.Get_Radius_For_Coords(Coords[0], Coords[1], Players[Players.size() -1].Get_Upgrade_Border_Radius());
      Get_Map().Retake_Owner_In_Radius_From(Coords[0], Coords[1], Players.size(), Players[Players.size() -1].Get_Upgrade_Border_Radius(), Get_Currently_Moving_Player_Id());
      Tiles_To_Update.insert(Tiles_To_Update.end(), tmp.begin(), tmp.end());
      City city_name = Get_Currently_Moving_Player().Lose_City_By_Coords(Coords[0], Coords[1]);
      string message = string(city_name.Get_Name()) + " has rebelled.";
      Main_Newspaper.Add_City_Conquer(Get_Current_Turn_By_Years(), message, Get_Currently_Moving_Player_Id());
      city_name.Change_Owner(Players[Players.size() - 1].Get_Name());
      Players[Players.size() - 1].Conquer_City(city_name);
    }
}
}

int Game::End_Player_Turn(Magic_Thread_Communicator* Thread_Portal)
{
  int out = -1;
  {
    is_in_thread = true;
    //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
    Remove_All_Missle_Units();
    vector<int> income = Get_Map().Get_Netto_Income_For_Player_By_Id(Get_Currently_Moving_Player_Id(), Get_Currently_Moving_Player());
    Get_Currently_Moving_Player().End_Turn(income);
    if(Get_Currently_Moving_Player().Has_Researched_Border_Expand_Tech_Recently())
    {
      vector<array<int,2>> tmp = Get_Map().Recalculate_Borders_For_Player_By_Id(Get_Currently_Moving_Player_Id(), Get_Currently_Moving_Player().Get_Upgrade_Border_Radius(), Get_Currently_Moving_Player());
      Tiles_To_Update.insert(Tiles_To_Update.end(), tmp.begin(), tmp.end());
    }
    Update_Stability_For_Currently_Moving_Player();
    Check_For_Dead_Players();
    do
    {
      currently_moving_player++;
      if(currently_moving_player > (int)Players.size())
        currently_moving_player = 1;
    }
    while(Is_Currently_Moving_Player_Eliminated());

    if(All_Humans_Are_Eliminated())
      return 0;
  }
  Thread_Portal->Notify_Window();
  {
    //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
    Get_Currently_Moving_Player().Refresh_Unit_Movement_Points();
    vector<Unit_On_Map> *u = Get_Currently_Moving_Player().Get_Owned_Units();
    for(auto &var : *u)
    {
      var.Self.Increase_Current_Movement(Get_Currently_Moving_Player().Find_Upgrade_By_Name(Get_Map().Get_Tile(var.Coordinates.x, var.Coordinates.y).Get_Upgrade()).How_Many_Times_Has_Trait("movementbonus"));
      if(var.Self.Get_All_Arguments_For_Trait("class")[0] == "flying" && Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(var.Coordinates.x, var.Coordinates.y)).Has_Trait("renewflyingmovement"))
        var.Self.Increase_Current_Movement(999);
    }

    if(currently_moving_player == First_Not_Eliminated_Player_Id())
      turn_counter++;
    Start_Turn_Of_Currently_Moving_Player(Thread_Portal);
    out = -1;
    if(All_Humans_Are_Eliminated())
      out = 0;
    if(Is_Only_One_Player_Alive())
      out = Get_Only_Living_Player_Id();
  }
  Thread_Portal->Set_Turn_Outcome(out);
  Thread_Portal->Finish();
  is_in_thread = false;
  return out;
}

Civ& Game::Get_Currently_Moving_Player()
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Players[currently_moving_player-1];
}

vector<Upgrade> Game::Get_Upgrades() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Upgrades;
}

vector<Unit> Game::Get_Units() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Units;
}

Upgrade Game::Get_Upgrade_By_Name(string_view name) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  for(auto &upg : Upgrades)
  {
    if(upg.Get_Name() == name)
      return upg;
  }
  throw;
}

double Game::Get_Defense_Bonus_For_Tile_And_Player(int x, int y, int player_id) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Get_Map().Get_Defense_Bonus_For_Tile(x, y) + (Get_Player_By_Id(player_id).Get_Defense_Bonus_For_Upgrade(Get_Map().Get_Tile(x,y).Get_Upgrade()) - 1.0);
}

array<int, 3> Game::Get_Units_Stats_For_Battle(int unit_x, int unit_y) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Get_Player_By_Id(Get_Map().Get_Tile(unit_x,unit_y).Get_Unit_Owner_Id()).Get_Unit_On_Tile_Pointer(unit_x, unit_y).Remove_All_Movement();
  Unit Stat_Unit = Get_Player_By_Id(Get_Map().Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id()).Get_Unit_On_Tile(unit_x, unit_y);
  array<int, 3> out;
  out[0] = Stat_Unit.Get_Attack_Power();
  out[1] = Stat_Unit.Get_Defense_Power();
  out[2] = Stat_Unit.Get_HP();
  double unit_def_mod = 0.0;
  if(!Stat_Unit.Has_Trait("doesnotrecievedefensivebonus"))
    unit_def_mod = Get_Defense_Bonus_For_Tile_And_Player(unit_x, unit_y, Get_Map().Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id());

  out[1] = static_cast<double>(out[1]) * unit_def_mod;

  if(Get_Map().Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id() == Get_Map().Get_Owner(unit_x, unit_y))
  {
    out[1] = static_cast<double>(out[1]) * (1 + (Get_Player_By_Id(Get_Map().Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id()).How_Many_Times_Has_Trait("P") / 10));
    out[0] = static_cast<double>(out[0]) * (1 + (Get_Player_By_Id(Get_Map().Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id()).How_Many_Times_Has_Trait("P") / 10));

  }

  if(Get_Player_By_Id(Get_Map().Get_Tile(unit_x, unit_y).Get_Unit_Owner_Id()).Get_Active_Goverment_Name() == "Fundamentalism")
  {
    out[1] = static_cast<double>(out[1]) * 0.9;
    out[0] = static_cast<double>(out[0]) * 0.9;
  }
  return out;
}


Unit Game::Get_Unit_By_Tile(int x, int y) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Get_Player_By_Id(Get_Map().Get_Tile(x,y).Get_Unit_Owner_Id()).Get_Unit_On_Tile(x,y);
}

Gov Game::Get_Goverment_By_Name(string_view gov_name) const
{
  return *find_if(Goverments.begin(), Goverments.end(), [&gov_name](auto& Gov){return Gov.Get_Name() == gov_name;});
}

void Game::Plunder_Tile(int x, int y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Sound_Manager::Play_Sound("assets/sounds/plunder-audio.mp3");
  Get_Currently_Moving_Player().Get_Unit_On_Tile_Pointer(x,y).Increase_Current_Movement(-1);
  Get_Map().Plunder_Tile(x,y);
  int tile_owner = Get_Map().Get_Owner(x,y);
  if(tile_owner) //neutrals
  {
    array<int, 2> City_Coords = Get_Map().Find_Closest_Upgrade_By_Name({x,y}, tile_owner, "City");
    Get_Player_By_Id(tile_owner).Get_City_By_Coordinates(City_Coords).Change_Stability(-4, Get_Map().Get_Tile(City_Coords[0], City_Coords[1]).Has_Unit());
  }
  Tiles_To_Update.push_back({x,y});
}

void Game::Battle_Units(int unit_1_x, int unit_1_y, int unit_2_x, int unit_2_y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  array<int, 3> unit_1_stats = Get_Units_Stats_For_Battle(unit_1_x, unit_1_y);
  array<int, 3> unit_2_stats = Get_Units_Stats_For_Battle(unit_2_x, unit_2_y);

  if(Get_Unit_By_Tile(unit_1_x,unit_1_y).Get_All_Arguments_For_Trait("class")[0] == "flying")
    unit_2_stats[0] = unit_2_stats[0] * Get_Unit_By_Tile(unit_2_x, unit_2_x).How_Many_Times_Has_Trait("fightflying");

  if(Get_Unit_By_Tile(unit_2_x,unit_2_y).Get_All_Arguments_For_Trait("class")[0] == "flying")
    unit_2_stats[0] = unit_2_stats[0] * Get_Unit_By_Tile(unit_1_x, unit_1_x).How_Many_Times_Has_Trait("fightflying");

  unit_1_stats[2] = unit_1_stats[2] - ((double) 30 * ((double) unit_2_stats[0] / (double) unit_1_stats[1]));
  unit_2_stats[2] = unit_2_stats[2] - ((double) 30 * ((double) unit_1_stats[0] / (double) unit_2_stats[1]));
  Get_Player_By_Id(Get_Map().Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id()).Get_Unit_On_Tile_Pointer(unit_1_x, unit_1_y).Set_HP(unit_1_stats[2]);
  Get_Player_By_Id(Get_Map().Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id()).Get_Unit_On_Tile_Pointer(unit_2_x, unit_2_y).Set_HP(unit_2_stats[2]);
  if(!is_in_thread)
  Sound_Manager::Play_Sound(Get_Player_By_Id(Get_Map().Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id()).Get_Unit_On_Tile_Pointer(unit_1_x, unit_1_y).Get_Audio_Path());
  if(unit_1_stats[2] <= 0 || Get_Unit_By_Tile(unit_1_x,unit_1_y).Get_All_Arguments_For_Trait("class")[0] == "missle")
  {
    Get_Player_By_Id(Get_Map().Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id()).Remove_Unit_By_Coords(unit_1_x,unit_1_y);
    Get_Map().Get_Tile_Pointer(unit_1_x,unit_1_y).Remove_Unit_From_Tile();
    if(unit_2_stats[2] > 0 && Get_Unit_By_Tile(unit_2_x, unit_2_y).Has_Trait("steal"))
      Get_Player_By_Id(Get_Map().Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id()).Give_One_Gold();
  }
  if(unit_2_stats[2] <= 0 || Get_Unit_By_Tile(unit_2_x,unit_2_y).Get_All_Arguments_For_Trait("class")[0] == "missle")
  {
    Get_Player_By_Id(Get_Map().Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id()).Remove_Unit_By_Coords(unit_2_x,unit_2_y);
    Get_Map().Get_Tile_Pointer(unit_2_x,unit_2_y).Remove_Unit_From_Tile();
    if(unit_1_stats[2] > 0 && Get_Unit_By_Tile(unit_1_x, unit_1_y).Has_Trait("steal"))
      Get_Player_By_Id(Get_Map().Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id()).Give_One_Gold();
  }
}

void Game::Move_Unit(int unit_x, int unit_y, int dest_x, int dest_y, int cost)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  int player_id = Get_Map().Get_Tile(unit_x,unit_y).Get_Unit_Owner_Id();
  Get_Player_By_Id(player_id).Move_Unit_To_By_Coords(unit_x, unit_y, dest_x, dest_y, cost);
  Get_Map().Get_Tile_Pointer(unit_x, unit_y).Remove_Unit_From_Tile();
  Get_Map().Get_Tile_Pointer(dest_x, dest_y).Put_Unit_On_Tile(player_id);
  if(!is_in_thread)
  Sound_Manager::Play_Sound("assets/sounds/unitmove-audio.mp3");
}

bool Game::Has_Currently_Moving_Player_Any_Actions_Left() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return (bool) Get_Currently_Moving_Player().Get_Current_Actions();
}

bool Game::Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(int unit_x, int unit_y, int dest_x, int dest_y, int movement_cost, bool combat, int enemy_unit_x, int enemy_unit_y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  int unit_owner_id = Get_Map().Get_Tile(unit_x,unit_y).Get_Unit_Owner_Id();
  int tile_owner_id = Get_Map().Get_Owner(dest_x, dest_y);
  Tiles_To_Update.push_back({unit_x, unit_y});
  Tiles_To_Update.push_back({dest_x, dest_y});
  Tiles_To_Update.push_back({enemy_unit_x, enemy_unit_y});
  bool is_combat = false;
  Move_Unit(unit_x, unit_y, dest_x, dest_y, movement_cost);
  if(unit_owner_id != tile_owner_id)
  {
    if(Get_Map().Get_Owner(dest_x, dest_y) != 0 && Get_Player_By_Id(Get_Map().Get_Owner(dest_x, dest_y)).Find_Upgrade_By_Name(Get_Map().Get_Tile(dest_x, dest_y).Get_Upgrade()).Has_Trait("borderexpand"))
    {
      Get_Map().Change_Tile_Owner(dest_x, dest_y, unit_owner_id);
      vector<array<int, 2>> tmp = Main_Radius_Generator.Get_Radius_For_Coords(dest_x, dest_y, Get_Player_By_Id(unit_owner_id).Get_Upgrade_Border_Radius());
      Get_Map().Retake_Owner_In_Radius_From(dest_x, dest_y, unit_owner_id, Get_Player_By_Id(unit_owner_id).Get_Upgrade_Border_Radius(), tile_owner_id);
      Tiles_To_Update.insert(Tiles_To_Update.end(), tmp.begin(), tmp.end());
      if(Get_Map().Get_Tile(dest_x, dest_y).Get_Upgrade() == "City")
      {
      //  if(!is_in_thread)
        Sound_Manager::Play_Sound("assets/sounds/citycapture-audio.mp3");
        string message;
        bool capital = false;
        if(Get_Player_By_Id(tile_owner_id).Get_City_Name_By_Coordinates(dest_x, dest_y) == Get_Player_By_Id(tile_owner_id).Get_Capital_Name())
        {
          message = "Capital of " + Get_Player_By_Id(tile_owner_id).Get_Full_Name() + " has been conquered! Goverment moves to ";
          capital = true;
          for_each(Get_Player_By_Id(tile_owner_id).Get_Owned_Cities().begin(), Get_Player_By_Id(tile_owner_id).Get_Owned_Cities().end(), [](auto& City){City.Change_Stability(-15, false);});
        }
        else
        {
          message = Get_Player_By_Id(unit_owner_id).Get_Full_Name() + " has conquered " + string(Get_Player_By_Id(tile_owner_id).Get_City_Name_By_Coordinates(dest_x, dest_y));
        }
        City city_name = Get_Player_By_Id(tile_owner_id).Lose_City_By_Coords(dest_x, dest_y);
        city_name.Change_Owner(Get_Player_By_Id(unit_owner_id).Get_Name());
        Get_Player_By_Id(unit_owner_id).Conquer_City(city_name);
        if(capital)
          message = message + Get_Player_By_Id(tile_owner_id).Get_Capital_Name();
        Main_Newspaper.Add_City_Conquer(Get_Current_Turn_By_Years(), message, tile_owner_id);
        Logger::Log_Info(message + " X: " + to_string(dest_x) + " Y: " + to_string(dest_y));
      }
    }
  }
  if(combat)
  {
    is_combat = true;
    Battle_Units(dest_x, dest_y, enemy_unit_x, enemy_unit_y);
  }
  return is_combat;
}

vector<Civ>& Game::Get_All_Civs()
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Civs;
}

void Game::Detonate_Atomic_Bomb(int x, int y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Sound_Manager::Play_Sound("assets/sounds/atomicexplosion-audio.mp3");
  Main_Newspaper.Add_Nuclear_Attack(Get_Current_Turn_By_Years(), Get_Currently_Moving_Player().Get_Full_Name() + " has dropped atomic bomb on tile X: " + to_string(x) + " Y: " + to_string(y) + "!", Get_Currently_Moving_Player_Id());
  Disband_Unit(x,y);
  vector<array<int, 2>> tmp = Main_Radius_Generator.Get_Radius_For_Coords(x,y,2);
  Tiles_To_Update.insert(Tiles_To_Update.end(), tmp.begin(), tmp.end());
  if(Get_Map().Get_Tile(x+1,y).Has_Unit())
    Disband_Unit(x+1,y);
  if(Get_Map().Get_Tile(x+1,y-1).Has_Unit())
    Disband_Unit(x+1,y-1);
  if(Get_Map().Get_Tile(x+1,y+1).Has_Unit())
    Disband_Unit(x+1,y+1);
  if(Get_Map().Get_Tile(x-1,y).Has_Unit())
    Disband_Unit(x-1,y);
  if(Get_Map().Get_Tile(x-1,y+1).Has_Unit())
    Disband_Unit(x-1,y+1);
  if(Get_Map().Get_Tile(x-1,y-1).Has_Unit())
    Disband_Unit(x-1,y-1);
  if(Get_Map().Get_Tile(x,y+1).Has_Unit())
    Disband_Unit(x,y+1);
  if(Get_Map().Get_Tile(x,y-1).Has_Unit())
    Disband_Unit(x,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x, y)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x,y);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x+1, y)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x+1,y);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x+1, y-1)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x+1,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x+1, y+1)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x+1,y+1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x-1, y)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x-1,y);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x-1, y-1)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x-1,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x-1, y+1)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x-1,y+1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x, y-1)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x,y-1);
  if(!Get_Upgrade_Of_Currently_Moving_Player(Get_Map().Get_Upgrade(x, y+1)).Has_Trait("cannotbeplundered"))
    Get_Map().Plunder_Tile(x,y+1);
}

string Game::Get_Current_Turn_By_Years() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  string out;
  int starting_year = -4000;
  int current_year = starting_year + (turn_counter * 15);
  out = out + to_string(current_year);
  if(current_year < 0)
    out = out + " BC";
  else
    out = out + " AD";
  return out;
}

Game::Game(xml_node<>* Root_Node) : Game_Map(Root_Node->first_node("map"))
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Main_Radius_Generator.Set_Size(Get_Map().Get_X_Size(), Get_Map().Get_Y_Size());
  Logger::Log_Info("Deserializing Game...");
  Deserialize(Root_Node);
}

void Game::Deserialize(xml_node<>* Root_Node)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  currently_moving_player = Get_Int_Value_From_Attribute(Root_Node, "currently_moving_player");
  turn_counter = Get_Int_Value_From_Attribute(Root_Node, "turn");

  Logger::Log_Info("Deserializing Tiles...");
  xml_node<>* Tiles_Node = Get_Subnode(Root_Node, "tiles");
  for(xml_node<> *Tile_Node = Tiles_Node->first_node("tile"); Tile_Node; Tile_Node = Tile_Node->next_sibling("tile"))
  {
    Tile tmp(Tile_Node);
    Tiles.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Upgrades...");
  xml_node<>* Upgrades_Node = Get_Subnode(Root_Node, "upgrades");
  for(xml_node<> *Upgrade_Node = Upgrades_Node->first_node("upgrade"); Upgrade_Node; Upgrade_Node = Upgrade_Node->next_sibling("upgrade"))
  {
    Upgrade tmp(Upgrade_Node);
    Upgrades.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Technologies...");
  xml_node<>* Technologies_Node = Get_Subnode(Root_Node, "techs");
  for(xml_node<> *Technology_Node = Technologies_Node->first_node("tech"); Technology_Node; Technology_Node = Technology_Node->next_sibling("tech"))
  {
    Tech tmp(Technology_Node);
    Technologies.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Units...");
  xml_node<>* Units_Node = Get_Subnode(Root_Node, "units");
  for(xml_node<> *Unit_Node = Units_Node->first_node("unit"); Unit_Node; Unit_Node = Unit_Node->next_sibling("unit"))
  {
    Unit tmp(Unit_Node);
    Units.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Goverments...");
  xml_node<>* Goverments_Node = Get_Subnode(Root_Node, "goverments");
  for(xml_node<> *Goverment_Node = Goverments_Node->first_node("goverment"); Goverment_Node; Goverment_Node = Goverment_Node->next_sibling("goverment"))
  {
    Gov tmp(Goverment_Node);
    Goverments.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Player Border Colors...");
  xml_node<>* Player_Border_Colors_Node = Get_Subnode(Root_Node, "player_border_colors");
  for(xml_node<> *Color_Node = Player_Border_Colors_Node->first_node("color"); Color_Node; Color_Node = Color_Node->next_sibling("color"))
  {
    uint32_t tmp = static_cast<uint32_t>(stoul(Color_Node->first_attribute("value")->value()));
  //  Logger::Log_Warning(Color_Node->first_attribute("value")->value());
    Player_Border_Colors.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Civs...");
  xml_node<>* Civs_Node = Get_Subnode(Root_Node, "civs");
  for(xml_node<> *Civ_Node = Civs_Node->first_node("civ"); Civ_Node; Civ_Node = Civ_Node->next_sibling("civ"))
  {
    Civ tmp(Civ_Node);
    Civs.push_back(tmp);
  }

  Logger::Log_Info("Deserializing Cultures...");
  xml_node<> *Cultures_Node = Get_Subnode(Root_Node, "cultures");
  for(xml_node<> *Culture_Node = Cultures_Node->first_node("culture"); Culture_Node; Culture_Node = Culture_Node->next_sibling("culture"))
  {
    string name = Culture_Node->first_attribute("name")->value();
    Culture tmp(Culture_Node->first_node("culture"));
    Cultures[name] = tmp;
  }

  Logger::Log_Info("Deserializing Players...");
  xml_node<>* Players_Node = Get_Subnode(Root_Node, "players");
  for(xml_node<> *Player_Node = Players_Node->first_node("civ"); Player_Node; Player_Node = Player_Node->next_sibling("civ"))
  {
    Civ tmp(Player_Node);
    Players.push_back(tmp);
  }

  xml_node<>* AI_Node = Get_Subnode(Root_Node, "ai_list");
  for(xml_node<> *Player_Node = AI_Node->first_node("ai"); Player_Node; Player_Node = Player_Node->next_sibling("ai"))
  {
    Is_Player_AI_List.push_back((bool) stoi(Player_Node->value()));
  }

  xml_node<>* Eliminated_Players_List_Node = Get_Subnode(Root_Node, "eliminated_list");
  for(xml_node<> *Eliminated_Player_Node = Eliminated_Players_List_Node->first_node("player_id"); Eliminated_Player_Node; Eliminated_Player_Node = Eliminated_Player_Node->next_sibling("player_id"))
  {
    Eliminated_Players_List.push_back(stoi(Eliminated_Player_Node->value()));
  }

  xml_node<>* AI_Data_Node = Get_Subnode(Root_Node, "ai_data_list");
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
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
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
    Tiles_Node->append_node(iterated_tile.Serialize(doc));
  });

  for_each(Upgrades.begin(), Upgrades.end(), [&](Upgrade& iterated_upgrade)
  {
    Upgrades_Node->append_node(iterated_upgrade.Serialize(doc));
  });

  for_each(Technologies.begin(), Technologies.end(), [&](Tech& iterated_tech)
  {
    Technologies_Node->append_node(iterated_tech.Serialize(doc));
  });

  for_each(Units.begin(), Units.end(), [&](Unit& iterated_unit)
  {
    Units_Node->append_node(iterated_unit.Serialize(doc));
  });

  for_each(Goverments.begin(), Goverments.end(), [&](Gov& iterated_gov)
  {
    Govs_Node->append_node(iterated_gov.Serialize(doc));
  });

  for_each(Players.begin(), Players.end(), [&](Civ& iterated_civ)
  {
    Players_Node->append_node(iterated_civ.Serialize(doc));
  });

  for_each(Civs.begin(), Civs.end(), [&](Civ& iterated_civ)
  {
    Civs_Node->append_node(iterated_civ.Serialize(doc));
  });

  Logger::Log_Info("Serializing Cultures...");
  xml_node<>* Cultures_Node = doc->allocate_node(node_element, "cultures");
  for(auto const& [culture_name, culture] : Cultures)
  {
    xml_node<>* Culture_Node = doc->allocate_node(node_element, "culture");
    xml_attribute<>* Culture_Name_Attribute = doc->allocate_attribute("name", culture_name.c_str());
    Culture_Node->append_attribute(Culture_Name_Attribute);
    Culture_Node->append_node(static_cast<Culture>(culture).Serialize(doc));
    Cultures_Node->append_node(Culture_Node);
  }

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
  Game_Node->append_node(Cultures_Node);
  Game_Node->append_node(Technologies_Node);
  Root_Node->append_node(Game_Node);
  //xml_document<> Document;
  //Document.append_node(Root_Node);
  return Root_Node;
}

bool Game::Save_Game(string_view path)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Game_Saver Saver(this);
  return Saver.Save_Game(path.data());
}

tuple<bool, Game*> Game::Load_Game(string_view path)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Game_Loader Loader;
  return Loader.Load_Game(path.data());
}

vector<tuple<array<string,2>, int>> Game::Get_Newspaper_Events() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Main_Newspaper.Get_Events_With_Icon_Paths();
}

void Game::Change_Goverment_For_Currently_Moving_Player_By_Name(string_view name)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  if(Get_Currently_Moving_Player().Get_Current_Actions() == Get_Currently_Moving_Player().Get_Max_Actions())
  {
    string message = "Riots in " + string(Get_Currently_Moving_Player().Get_Capital_Name()) + "!";
    message = message + " " + string(Get_Currently_Moving_Player().Get_Leader_Name()) + " has been killed.";
    message = message + " " + string(Get_Currently_Moving_Player().Get_Raw_Name()) + " has been proclaimed ";
    auto fallback_leader_name = Get_Culture_By_Player_Id(Get_Currently_Moving_Player_Id()).Get_Random_Leader_Name();
    bool has_been_fallback_name_used = Get_Currently_Moving_Player().Change_Goverment_By_Name(name, fallback_leader_name);
    if(has_been_fallback_name_used)
      Get_Culture_By_Player_Id(Get_Currently_Moving_Player_Id()).Remove_Last_Leader_Name();
    message = message + string(Get_Currently_Moving_Player().Get_Full_Name());
    Main_Newspaper.Add_Revolt(Get_Current_Turn_By_Years(), message, Get_Currently_Moving_Player_Id());
  }
}

void Game::Build_City(int x, int y, int owner, int radius)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  string name = "City";
  Build_Upgrade(name, x, y, owner);
  auto fallback_city_name = Get_Culture_By_Player_Id(owner).Get_Random_City_Name();
  bool has_been_fallback_name_used = Get_Player_By_Id(owner).Build_City_On_Map(x,y, fallback_city_name, Get_Current_Turn_By_Years());
  if(has_been_fallback_name_used)
    Get_Culture_By_Player_Id(owner).Remove_Last_City_Name();
  string message = string(Get_Player_By_Id(owner).Get_Full_Name()) + " has settled new city of " + string(Get_Player_By_Id(owner).Get_City_Name_By_Coordinates(x,y));
  Main_Newspaper.Add_City_Build(Get_Current_Turn_By_Years(), message, owner);
}

int Game::Get_Total_Cost_Of_Technology_By_Name(string_view name) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
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

Unit Game::Get_Unit_By_Name(string_view name) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  auto it = find_if(Units.begin(), Units.end(), [&name](Unit& u){
    return u.Get_Name() == name;
  });
  Unit out = *it;
  return out;
}

void Game::Disband_Unit(int x, int y)
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Get_Map().Get_Tile_Pointer(x,y).Remove_Unit_From_Tile();
  Get_Currently_Moving_Player().Remove_Unit_By_Coords(x,y);
  Tiles_To_Update.push_back({x,y});
}

vector<Civ> Game::Get_Players() const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Players;
}

Upgrade Game::Get_Upgrade_Of_Currently_Moving_Player(string_view upg_name) const
{
  //if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  return Get_Currently_Moving_Player().Find_Upgrade_By_Name(upg_name);
}
