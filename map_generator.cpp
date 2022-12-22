#include "map_generator.h"

Map_Generator::Map_Generator(Map_Generator_Data mpg, vector<Tile> t, vector<Upgrade> u) : Game_Map(t, u)
{
  srand(time(0));
  User_Data = mpg;
  Tiles = t;
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

string Map_Generator::Get_Sea_Tile_For_Y(int y)
{
  double lower_border = User_Data.size_y * 0.15;
  double upper_border = User_Data.size_y * 0.85;
  string out = "Sea";
  if((y < lower_border && rand() % static_cast<int>(lower_border) < (lower_border - y)) || (y > upper_border && rand() % static_cast<int>(lower_border) < (y - upper_border)))
    out = "Ice";
  return out;
}

string Map_Generator::Get_Land_Tile_For_Y(int y)
{
  double ice_lower_border = User_Data.size_y * 0.10;
  double ice_upper_border = User_Data.size_y * 0.90;
  string out = "Land";
  if(rand() % 10 < 3)
    out = "Forest";
  if((y < ice_lower_border && rand() % static_cast<int>(ice_lower_border) < (ice_lower_border - y)) || (y > ice_upper_border && rand() % static_cast<int>(ice_lower_border) < (y - ice_upper_border)))
    out = "Ice";
  double desert_lower_border = User_Data.size_y * 0.37;
  double desert_upper_border = User_Data.size_y * 0.53;
  double desert_middle = User_Data.size_y * 0.45;
  if(y < desert_upper_border && y > desert_lower_border && rand() % static_cast<int>(desert_upper_border-desert_middle) > (abs(y-desert_middle)))
    out = "Desert";
  else if (y < desert_upper_border && y > desert_lower_border && out == "Forest")
    out = "Land";
  return out;
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
  double frequency = User_Data.continents_number * 0.007;
  Logger::Log_Info("Frequency: " + to_string(frequency));
  FastNoise Noise_Generator(rand());
  Noise_Generator.SetNoiseType(FastNoise::Perlin);
  Noise_Generator.SetFrequency(frequency);
  vector<float> Map(x*y);
  int start = 0;
  int start_y = 0;
  int index = 0;
  while(start < x)
  {
    while(start_y < y)
    {
      Map[index] = Noise_Generator.GetNoise((float) start, (float) start_y);
      index++;
      start_y++;
    }
    start_y = 0;
    start++;
  }
  double lowest = 0.0;
  double highest = 0.0;
  for_each(Map.begin(), Map.end(), [&](float f){if(f < lowest){lowest = f;}if(f > highest){highest = f;}});
  double border = (lowest+(highest*(50/User_Data.oceans_precentage)))/2;
  start = 0;
  start_y = 0;
  index = 0;
  double mountain_border = lowest / 2;
  Logger::Log_Info("Lowest Value: " + to_string(lowest) + " Highest Value: " + to_string(highest) + " Border: " + to_string(border) + " Mountain Border: " + to_string(mountain_border));
  while(start < x)
  {
    while(start_y < y)
    {
      if(Map[index] < border)
        Fill_Territory(Get_Land_Tile_For_Y(start_y), start, start_y, start+1, start_y+1);
      else
        Fill_Territory(Get_Sea_Tile_For_Y(start_y), start, start_y, start+1, start_y+1);
      if(Map[index] < mountain_border && rand() % 10 < 7)
        Fill_Territory("Mountain", start, start_y, start+1, start_y+1);
      index++;
      start_y++;
    }
    start_y = 0;
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
  vector<char> tmp_v((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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
