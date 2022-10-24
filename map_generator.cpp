#include "map_generator.h"

Map_Generator::Map_Generator(Map_Generator_Data mpg, vector<Tile> t, vector<Upgrade> u) : Game_Map(t, u)
{
  srand(time(0));
  User_Data = mpg;
  Tiles = t;
}

void Map_Generator::Fray_Territory(int start_x, int start_y, int end_x, int end_y)
{
  Tile Filler;
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == "Sea")
      Filler = var;
  }
  int proces_x = start_x;
  int proces_y = start_y;
  while(proces_x < end_x)
  {
    if(rand() % 2 == 0)
    Game_Map.Change_Tile_Type(proces_x,start_y,Filler);
    proces_x++;
  }
  while(proces_y < end_y)
  {
    if(rand() % 2 == 0)
    Game_Map.Change_Tile_Type(start_x,proces_y,Filler);
    proces_y++;
  }
  while(proces_x > end_x)
  {
    if(rand() % 2 == 0)
    Game_Map.Change_Tile_Type(proces_x,proces_y,Filler);
    proces_x--;
  }
  while(proces_y > end_y)
  {
    if(rand() % 2 == 0)
    Game_Map.Change_Tile_Type(proces_x,proces_y,Filler);
    proces_y--;
  }
}

void Map_Generator::Fill_Territory(string tile_name,int start_x, int start_y, int end_x, int end_y)
{
  Tile Filler;
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == tile_name)
      Filler = var;
  }
  int old_y = start_y;
  int limit_x = Game_Map.Get_X_Size() - 1;
  int limit_y = Game_Map.Get_Y_Size() - 1;
  while(start_x < end_x)
  {
    while(start_y < end_y)
    {
      if(start_x > limit_x || start_y > limit_y || start_x < 0 || start_y < 0)
        break;
      Game_Map.Change_Tile_Type(start_x,start_y,Filler);
      start_y++;
    }
    start_y = old_y;
    start_x++;
  }
}

void Map_Generator::Clean_Map()
{
  Fill_Territory("Sea", 0, 0, Game_Map.Get_X_Size() - 1, Game_Map.Get_Y_Size() - 1);
}

void Map_Generator::Noise_Territory(string tile_name, int x, int y, int size)
{
  Tile Filler;
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == tile_name)
      Filler = var;
  }
  bool loop = true;
  int limit = size;
  while(loop)
  {
    int mod_x = rand() % (int)sqrt(size);
    int mod_y = rand() % (int)sqrt(size);
    int for_x = x + mod_x;
    int for_y = y + mod_y;
    Game_Map.Change_Tile_Type(for_x, for_y, Filler);
    int random = rand() % (size);
    if(random > limit)
      loop = false;
    limit--;
  }
}

void Map_Generator::Generate_Continent_At_Coords(int x, int y, unsigned long long size, int depth)
{
  if(depth <= 0 || size <= 0)
    return;
  Logger::Log_Info( "Generating Continent at " + to_string(x) + " " + to_string(y) + " Depth: " + to_string(depth));
  Tile Sea, Forest, Land, Mountain;
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == "Sea")
      Sea = var;
  }
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == "Forest")
      Forest = var;
  }
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == "Land")
      Land = var;
  }
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == "Mountain")
      Mountain = var;
  }
  Fill_Territory("Land", x, y, sqrt(size) + x, sqrt(size) + y);
  Fray_Territory(x, y, sqrt(size) + x, sqrt(size) + y);
  // after we habe landmass lets add some trees

  Noise_Territory("Mountain", x, y, size);

  if(y < (User_Data.size_y * 0.15) || y > (User_Data.size_y * 0.85))
  {
    Logger::Log_Info("Polar continent!");
    Noise_Territory("Forest", x, y, size);
    Noise_Territory("Ice", x, y, size);
    Noise_Territory("Ice", x, y, size);
  }
  if(y < (User_Data.size_y * 0.60) && y > (User_Data.size_y * 0.45))
  {
    Logger::Log_Info("Desert continent!");
    Noise_Territory("Desert", x, y, size);
    Noise_Territory("Desert", x, y, size);
    Noise_Territory("Desert", x, y, size);
  }
  else
  {
    Noise_Territory("Forest", x, y, size);
  }

  int new_size = size / (rand() % 5 + 3);
  int new_x = x;
  int new_y = y;
  int side = rand() % 4 + 1;
  switch(side)
  {
    case 1:
    {
      new_x = new_x + sqrt(size);
      break;
    }
    case 2:
    {
      new_x = new_x - sqrt(size);
      break;
    }
    case 3:
    {
      new_y = new_y + sqrt(size);
      break;
    }
    case 4:
    {
      new_y = new_y - sqrt(size);
      break;
    }
    default:
      break;
  }
  Generate_Continent_At_Coords(new_x, new_y, new_size, depth-1);
  //Logger::Log_Info( "Generated!" );
}

map<char, Tile> Map_Generator::Load_Replacements(xml_node<>* Root_Node)
{
  xml_node<>* Replacements_Node = Root_Node->first_node("replacements");
  map<char, Tile> out;
  for(xml_node<> *Replacement_Node = Replacements_Node->first_node("replacement"); Replacement_Node; Replacement_Node = Replacement_Node->next_sibling("replacement"))
  {
    string name = Replacement_Node->first_attribute("name")->value();
    char r = Replacement_Node->first_attribute("char")->value()[0];
    Tile tmp;
    for(auto &var : Tiles)
    {
      if(var.Get_Name() == name)
        tmp = var;
    }
    out[r] = tmp;
  }
  return out;
}

void Map_Generator::Load_Map_From_File(xml_node<>* Root_Node, map<char, Tile> Replacements)
{
  string map_data = Root_Node->first_node("map")->value();
  vector<string> map;
  string tmp = "";
  for(char c : map_data)
  {
    if(c == '\n')
    {
      map.push_back(tmp);
      tmp = "";
      continue;
    }
    tmp = tmp + c;
  }
  map.erase(map.begin());
  Game_Map.Set_Size(map[0].size(), map.size());

  int x = 0;
  int y = 0;

  for(string &row : map)
  {
    for(char c : row)
    {
      Game_Map.Change_Tile_Type(x, y, Replacements[c]);
      x++;
    }
    y++;
    x = 0;
  }
}

Map Map_Generator::Generate_Map_From_File()
{
  xml_document<> doc;
  xml_node<>* Root_Node = Load_Map(&doc);
  map<char, Tile> Replacements = Load_Replacements(Root_Node);
  Load_Map_From_File(Root_Node, Replacements);
  return Game_Map;
}

Map Map_Generator::Generate_Map_Using_User_Data()
{
  if(User_Data.map_path != " ")
  {
    Logger::Log_Info("Loading map from file!");
    return Generate_Map_From_File();
  }
  //firstly fill the map with ocean
  Game_Map.Set_Size(User_Data.size_x, User_Data.size_y);
  Tile Ocean;
  for(auto &var : Tiles)
  {
    if(var.Get_Name() == "Sea")
      Ocean = var;
  }
  int x = Game_Map.Get_X_Size();
  int y = Game_Map.Get_Y_Size();
  Logger::Log_Info( "Map Generator" );
  Logger::Log_Info( "Map Size X: " + to_string(x) + " Y: " + to_string(y) );
  Logger::Log_Info( "Cleaning The Map..." );
  Clean_Map();
  Logger::Log_Info( "Map Clean!" );
  unsigned long long tile_number = x * y;
  unsigned long long land_tiles = tile_number * (double)((double)User_Data.oceans_precentage / 100);
  Logger::Log_Info( "Continents: " + to_string(User_Data.continents_number) + " Land Percentage: " + to_string(User_Data.oceans_precentage) + " Land Tiles: " + to_string(land_tiles) );
  land_tiles = land_tiles / User_Data.continents_number;
  Logger::Log_Info( "Tiles Per Continent: " + to_string(land_tiles) );
  int start = 0;
  while(start < User_Data.continents_number)
  {
    Generate_Continent_At_Coords(rand() % x, rand() % y, land_tiles, rand() % 5 + 3);
    start++;
  }
  return Game_Map;
}

xml_node<>* Map_Generator::Load_Map(xml_document<>* doc)
{
  if(User_Data.map_path == " ")
  {
    Logger::Log_Error("Map path not found");
    return nullptr;
  }
  Logger::Log_Info("Loading map " + User_Data.map_path + "...");
  fstream file (User_Data.map_path, std::ios::in | std::ios::binary);
  if(! file.is_open())
  {
    Logger::Log_Error("Loading Map failed");
    return nullptr;
  }
  vector<char> tmp_v((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  tmp_v.push_back('\0');
  file_buf.clear();
  copy(tmp_v.begin(), tmp_v.end(), back_inserter(file_buf));
  doc->parse<0>(&file_buf[0]);
  xml_node<>* Root_Node = doc->first_node()->first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Map Data failed!");
    return nullptr;
  }
  Logger::Log_Info("Loading finished!");
  return Root_Node;
}

map<string, array<int, 2>> Map_Generator::Fetch_Starting_Positions_From_Map_Data()
{
  map<string, array<int,2>> out;
  xml_document<> doc;
  xml_node<>* Root_Node = Load_Map(&doc);
  if(Root_Node == nullptr)
  {
    Logger::Log_Warning("No starting positions found, this may not be an error!");
    return out;
  }
  xml_node<>* Starting_Positions_Node = Root_Node->first_node("starting_positions");
  for(xml_node<> *Position_Node = Starting_Positions_Node->first_node("starting_position"); Position_Node; Position_Node = Position_Node->next_sibling("starting_position"))
  {
    out[Position_Node->first_attribute("name")->value()] = {stoi(Position_Node->first_attribute("x")->value()), stoi(Position_Node->first_attribute("y")->value())};
  }
  return out;
}
