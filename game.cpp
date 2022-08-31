#include "game.h"

void Game::XML_Load_Tiles()
{
  Logger::Log_Info("Loading XML Tile Data..." );
  xml_document<> doc;
	ifstream file ("miniyacg-config-tiles.xml");
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
  file.close();
	doc.parse<0>(&buffer[0]);
  xml_node<> *tiles_node = doc.first_node()->first_node("tiles");
  for(xml_node<> * tile_node = tiles_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling())
  {
    int c = stoi(tile_node->first_attribute("cost")->value());
    string n = tile_node->first_attribute("name")->value();
    string t_p = tile_node->first_attribute("texture")->value();
    vector<string> traits;
    for(xml_node<> *trait_node = tile_node->first_node("trait"); trait_node; trait_node = trait_node->next_sibling("trait"))
    {
      traits.push_back(trait_node->first_attribute("name")->value());
    }
    Tile tmp(c, n, t_p, traits);
    Tiles.push_back(tmp);
  }
  Logger::Log_Info("XML Tile Data Loaded!" );
}

int Game::Get_Amount_Of_Players()
{
  return Players.size();
}

void Game::XML_Load_Techs()
{
  Logger::Log_Info("Loading XML Techs Data..." );
  xml_document<> doc;
	ifstream file ("miniyacg-config-techs.xml");
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
  file.close();
	doc.parse<0>(&buffer[0]);
  xml_node<> *technologies_node = doc.first_node()->first_node("technologies");
  for(xml_node<> * tech_node = technologies_node->first_node("technology"); tech_node; tech_node = tech_node->next_sibling())
  {
    int c = stoi(tech_node->first_attribute("cost")->value());
    string n = tech_node->first_attribute("name")->value();
    string h_t = tech_node->first_attribute("info")->value();
    string t_p = tech_node->first_attribute("texture")->value();
    vector<string> req;
    vector<string> traits;
    for(xml_node<> *req_node = tech_node->first_node("requirement"); req_node; req_node = req_node->next_sibling("requirement"))
    {
      req.push_back(req_node->first_attribute("name")->value());
    }
    for(xml_node<> *trait_node = tech_node->first_node("trait"); trait_node; trait_node = trait_node->next_sibling("trait"))
    {
      traits.push_back(trait_node->first_attribute("name")->value());
    }
    Tech tmp(n, c, req, h_t, t_p, traits);
    Technologies.push_back(tmp);
  }
  //for(auto &var : Technologies)
  //{
  //  var.Info();
  //}
  Logger::Log_Info( "XML Techs Data Loaded!" );
}

void Game::XML_Load_Units()
{
  Logger::Log_Info("Loading XML Units Data..." );
  xml_document<> doc;
  ifstream file("miniyacg-config-units.xml");
  vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  buffer.push_back('\0');
  file.close();
	doc.parse<0>(&buffer[0]);
  xml_node<> *units_node = doc.first_node()->first_node("units");
  for(xml_node<> * unit_node = units_node->first_node("unit"); unit_node; unit_node = unit_node->next_sibling())
  {
    string n = unit_node->first_attribute("name")->value();
    int c = stoi(unit_node->first_attribute("cost")->value());
    int a = stoi(unit_node->first_attribute("atk")->value());
    int d = stoi(unit_node->first_attribute("def")->value());
    int m = stoi(unit_node->first_attribute("move")->value());
    int ma = stoi(unit_node->first_attribute("mait")->value());
    string r = unit_node->first_attribute("requirement")->value();
    string h_t = unit_node->first_attribute("info")->value();
    string t_p = unit_node->first_attribute("texture")->value();
    vector<string> a_t;
    for(xml_node<> *tile_node = unit_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling("tile"))
    {
      a_t.push_back(tile_node->first_attribute("name")->value());
    }
    Unit tmp(n, c, a, d, m, ma, h_t, r, t_p, a_t);
    Units.push_back(tmp);
  }
  Logger::Log_Info("XML Units Data Loaded!" );
}

void Game::XML_Load_Civs()
{
  Logger::Log_Info("Loading XML Civs Data..." );
  xml_document<> doc;
  ifstream file("miniyacg-config-civs.xml");
  vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  buffer.push_back('\0');
  file.close();
	doc.parse<0>(&buffer[0]);
  xml_node<> *civs_node = doc.first_node()->first_node("civs");
  for(xml_node<> * civ_node = civs_node->first_node("civ"); civ_node; civ_node = civ_node->next_sibling())
  {
    string n = civ_node->first_attribute("name")->value();
    string h_t = civ_node->first_node("info")->value();
    int r = stoi(civ_node->first_attribute("r")->value());
    int g = stoi(civ_node->first_attribute("g")->value());
    int b = stoi(civ_node->first_attribute("b")->value());
    string p = civ_node->first_attribute("personality")->value();
    vector<string> cities;
    vector<string> traits;
    vector<string> leaders;
    map<string, vector<string>> g_n_r;
    for(xml_node<> *city_node = civ_node->first_node("city"); city_node; city_node = city_node->next_sibling("city"))
    {
      cities.push_back(city_node->first_attribute("name")->value());
    }
    for(xml_node<> *leader_node = civ_node->first_node("leader"); leader_node; leader_node = leader_node->next_sibling("leader"))
    {
      leaders.push_back(leader_node->first_attribute("name")->value());
    }
    for(xml_node<> *trait_node = civ_node->first_node("trait"); trait_node; trait_node = trait_node->next_sibling("trait"))
    {
      traits.push_back(trait_node->first_attribute("name")->value());
    }
    for(xml_node<> *rep_node = civ_node->first_node("replacement"); rep_node; rep_node = rep_node->next_sibling("replacement"))
    {
      g_n_r[rep_node->first_attribute("name")->value()].push_back(rep_node->first_attribute("leader")->value());
      g_n_r[rep_node->first_attribute("name")->value()].push_back(rep_node->first_attribute("state_name")->value());
    }
    Civ tmp(n, leaders, h_t, cities, Technologies, Units, r, g, b, traits, Goverments, g_n_r, p);
    Civs.push_back(tmp);
  }

  Logger::Log_Info("XML Civs Data Loaded!" );
}

void Game::XML_Load_Upgrades()
{
  Logger::Log_Info("Loading XML Upgrades Data..." );
  xml_document<> doc;
  ifstream file("miniyacg-config-upgrades.xml");
  vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  buffer.push_back('\0');
  file.close();
  doc.parse<0>(&buffer[0]);
  xml_node<> *upgrades_node = doc.first_node()->first_node("upgrades");
  for(xml_node<> * upgrade_node = upgrades_node->first_node("upgrade"); upgrade_node; upgrade_node = upgrade_node->next_sibling())
  {
    string n = upgrade_node->first_attribute("name")->value();
    int c = stoi(upgrade_node->first_attribute("cost")->value());
    int m = stoi(upgrade_node->first_attribute("mait")->value());
    int p = stoi(upgrade_node->first_attribute("prod")->value());
    string h_t = upgrade_node->first_attribute("info")->value();
    string r = upgrade_node->first_attribute("requirement")->value();
    string t_p = upgrade_node->first_attribute("texture")->value();
    bool avoid = (bool) stoi(upgrade_node->first_attribute("avoid")->value());
    vector<string> correct_tiles;
    vector<string> traits;
    for(xml_node<> *tile_node = upgrade_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling("tile"))
    {
      correct_tiles.push_back(tile_node->first_attribute("name")->value());
    }
    for(xml_node<> *trait_node = upgrade_node->first_node("trait"); trait_node; trait_node = trait_node->next_sibling("trait"))
    {
      traits.push_back(trait_node->first_attribute("name")->value());
    }
    Upgrade tmp(c, p, m, h_t, n, r, correct_tiles, t_p, avoid, traits);
    Upgrades.push_back(tmp);
  }
//  for(auto &var : Upgrades)
//  {
//    var.Info();
//  }
  Logger::Log_Info("XML Upgrades Data Loaded!" );
}

void Game::XML_Load_Goverments()
{
  Logger::Log_Info("Loading XML Goverments Data..." );
  xml_document<> doc;
  ifstream file("miniyacg-config-goverments.xml");
  vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  buffer.push_back('\0');
  file.close();
  doc.parse<0>(&buffer[0]);
  xml_node<> *govs_node = doc.first_node()->first_node("goverments");
  for(xml_node<> * gov_node = govs_node->first_node("goverment"); gov_node; gov_node = gov_node->next_sibling())
  {
    string n = gov_node->first_attribute("name")->value();
    string i = gov_node->first_attribute("info")->value();
    string t_r = gov_node->first_attribute("requirement")->value();
    string t_p = gov_node->first_attribute("texture")->value();
    string l_t = gov_node->first_attribute("title")->value();
    string s_n = gov_node->first_attribute("state_name")->value();
    Gov tmp(n, l_t, s_n, t_r, i, t_p);
    Goverments.push_back(tmp);
  }
  Logger::Log_Info("XML Goverments Data Loaded!" );
}
void Game::XML_Load_Data()
{
  XML_Load_Tiles();
  XML_Load_Techs();
  XML_Load_Units();
  XML_Load_Goverments();
  XML_Load_Civs();
  XML_Load_Upgrades();
}

Map_Generator_Data Game::Show_Map_Generator_UI()
{
  Map_Generator_Data out;
  //to do gtkmm
  //right now it will be in terminal

  Logger::Log_Info("Alpha Map Generator" );
  Logger::Log_Info("TO DO: Safety, This script has no correctness checking!" );
  Logger::Log_Info("Please enter X size: ");
  cin >> out.size_x;
  Logger::Log_Info("Please enter Y size: ");
  cin >> out.size_y;
  Logger::Log_Info("Please enter amount of continents:");
  cin >> out.continents_number;
  Logger::Log_Info("Please enter oceans oceans_precentage:");
  cin >> out.oceans_precentage;
  return out;
}

void Game::Generate_Map(Map_Generator_Data User_Data)
{
  Map_Generator Gen(User_Data, Tiles, Upgrades);
  Game_Map = Gen.Generate_Map_Using_User_Data();
  //Game_Map.Print_Map_In_ASCII();
}

void Game::Show_Players_Generator_UI()
{
  Logger::Log_Error("Unused Function");
}

void Game::Assign_Starting_Positions()
{
  int player_id = 1;
  for(auto &player : Players)
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
        player.Assign_Id(player_id);
        Logger::Log_Info("Player " + player.Get_Leader_Name() + " will start at position " + to_string(x) + " " + to_string(y) );
        player.Build_City_On_Map(x,y);
        Game_Map.Build_City(x,y, player_id, 2);
        loop = false;
        player_id++;
      }
    }
  }
}

void Game::Add_Players(int player_id, int other_players_player_count, bool allow_duplicates)
{
  currently_moving_player = 1;
  Players.push_back(Civs[player_id]);
  Is_Player_AI_List.push_back(0); //neutrals
  Is_Player_AI_List.push_back(0);
  Player_Border_Colors.push_back(Players[0].Get_Civ_Color());
  if(!allow_duplicates)
    Civs.erase(Civs.begin() + player_id);
  int added_civs = 0;
  while(Civs.size() != 0 && other_players_player_count != added_civs)
  {
    Is_Player_AI_List.push_back(1);
    AI_Data tmp;
    AI_Data_For_Players[Players.size() - 1] = tmp;
    int rand_id = rand() % Civs.size();
    Players.push_back(Civs[rand_id]);
    Player_Border_Colors.push_back((uint32_t)Players[Players.size() - 1].Get_Civ_Color());
    if(!allow_duplicates)
      Civs.erase(Civs.begin() + rand_id);
    added_civs++;
  }
  if(Civs.size() == 0)
    Logger::Log_Warning("Not enough civilizations, player count decreased!");
  Logger::Log_Info("Players added!" );
}

Game::Game(bool a)
{
  srand(time(0));
  turn_counter = 1;
  Logger::Log_Info("Starting XML Data Loading...");
  Logger::Log_Warning("TO DO: Safety, If code crashes here check XML!" );
  XML_Load_Data();
  Logger::Log_Info("XML Data Loaded!" );
  autosave = a;
}

Game::Game()
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

vector<Coords> Game::Get_Tiles_To_Update()
{
  vector<Coords> out = Tiles_To_Update;
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

bool Game::End_Player_Turn()
{
  vector<int> income = Get_Map()->Get_Netto_Income_For_Player_By_Id(Get_Currently_Moving_Player_Id(), *Get_Currently_Moving_Player());
  Get_Currently_Moving_Player()->End_Turn(income);
  if(Get_Currently_Moving_Player()->Has_Researched_Border_Expand_Tech_Recently())
  {
    Get_Map()->Recalculate_Borders_For_Player_By_Id(Get_Currently_Moving_Player_Id(), Get_Currently_Moving_Player()->Get_Upgrade_Border_Radius());
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
    if(Get_Map()->Get_Tile(var.Coordinates.x, var.Coordinates.y).Get_Upgrade().Has_Trait("movement_buff"))
    {
      var.Self.Increase_Current_Movement(2);
      if(Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Trait("improve_movement_boost"))
      {
        var.Self.Increase_Current_Movement(2);
      }
    }
  }

  if(currently_moving_player == First_Not_Eliminated_Player_Id())
    turn_counter++;
  if(autosave)
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

void Game::Battle_Units(int unit_1_x, int unit_1_y, int unit_2_x, int unit_2_y)
{
  Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_1_x, unit_1_y).Remove_All_Movement();
  Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_2_x, unit_2_y).Remove_All_Movement();
  int unit_1_atk, unit_1_def, unit_1_hp;
  int unit_2_atk, unit_2_def, unit_2_hp;
  unit_1_atk = Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_1_x, unit_1_y).Get_Attack_Power();
  unit_1_def = Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_1_x, unit_1_y).Get_Defense_Power();
  unit_1_hp = Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_1_x, unit_1_y).Get_HP();

  unit_2_atk = Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_2_x, unit_2_y).Get_Attack_Power();
  unit_2_def = Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_2_x, unit_2_y).Get_Defense_Power();
  unit_2_hp = Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(unit_2_x, unit_2_y).Get_HP();

  //defense modifiers
  double unit_1_def_mod, unit_2_def_mod;
  unit_1_def_mod = Get_Map()->Get_Defense_Bonus_For_Tile(unit_1_x, unit_1_y);
  unit_2_def_mod = Get_Map()->Get_Defense_Bonus_For_Tile(unit_2_x, unit_2_y);

  if(Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Get_Active_Goverment_Name() == "Fundamentalism")
  {
    unit_1_def = (double) unit_1_def * 0.9;
    unit_1_atk = (double) unit_1_atk * 0.9;

  }
  if(Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Get_Active_Goverment_Name() == "Fundamentalism")
  {
    unit_2_def = (double) unit_2_def * 0.9;
    unit_2_atk = (double) unit_2_atk * 0.9;

  }

  unit_1_def = (double) unit_1_def * unit_1_def_mod;
  unit_2_def = (double) unit_2_def * unit_2_def_mod;
  unit_1_hp = unit_1_hp - ((double) 30 * ((double) unit_2_atk / (double) unit_1_def ));
  unit_2_hp = unit_2_hp - ((double) 30 * ((double) unit_1_atk / (double) unit_2_def ));

  Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile_Pointer(unit_1_x, unit_1_y)->Set_HP(unit_1_hp);
  Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile_Pointer(unit_2_x, unit_2_y)->Set_HP(unit_2_hp);

  if(unit_1_hp <= 0)
  {
    Get_Player_By_Id(Get_Map()->Get_Tile(unit_1_x,unit_1_y).Get_Unit_Owner_Id())->Remove_Unit_By_Coords(unit_1_x,unit_1_y);
    Get_Map()->Get_Tile_Pointer(unit_1_x,unit_1_y)->Remove_Unit_From_Tile();
  }
  if(unit_2_hp <= 0)
  {
    Get_Player_By_Id(Get_Map()->Get_Tile(unit_2_x,unit_2_y).Get_Unit_Owner_Id())->Remove_Unit_By_Coords(unit_2_x,unit_2_y);
    Get_Map()->Get_Tile_Pointer(unit_2_x,unit_2_y)->Remove_Unit_From_Tile();
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
  bool is_map_update_necesary = false;
  Move_Unit(unit_x, unit_y, dest_x, dest_y, movement_cost);
  if(unit_owner_id != tile_owner_id)
  {
    if(Get_Map()->Get_Tile(dest_x, dest_y).Get_Upgrade().Has_Trait("border_expand"))
    {
      is_map_update_necesary = true;
      Get_Map()->Change_Tile_Owner(dest_x, dest_y, unit_owner_id);
      Get_Map()->Retake_Owner_In_Radius(dest_x, dest_y, unit_owner_id, Get_Player_By_Id(unit_owner_id)->Get_Upgrade_Border_Radius());
      if(Get_Map()->Get_Tile(dest_x, dest_y).Get_Upgrade().Get_Name() == "City")
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

Game::Game(xml_node<>* Root_Node) : Game_Map(Root_Node->first_node("map"))
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
  Get_Map()->Build_City(x,y,owner, radius);
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
