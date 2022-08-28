#include "map_generator.h"

Map_Generator::Map_Generator(Map_Generator_Data mpg, vector<Tile> t, vector<Upgrade> u) : Game_Map(mpg.size_x, mpg.size_y, t, u)
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
  bool loop = true;
  int limit = size;
  while(loop)
  {
    int mod_x = rand() % (int)sqrt(size);
    int mod_y = rand() % (int)sqrt(size);
    int for_x = x + mod_x;
    int for_y = y + mod_y;
    Game_Map.Change_Tile_Type(for_x, for_y, Forest);
    int random = rand() % (size);
    if(random > limit)
      loop = false;
    limit--;
  }
  loop = true;
  limit = size;
  while(loop)
  {
    int mod_x = rand() % (int)sqrt(size);
    int mod_y = rand() % (int)sqrt(size);
    int for_x = x + mod_x;
    int for_y = y + mod_y;
    Game_Map.Change_Tile_Type(for_x, for_y, Mountain);
    int random = rand() % (size);
    if(random > limit)
      loop = false;
    limit--;
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

Map Map_Generator::Generate_Map_Using_User_Data()
{
  //firstly fill the map with ocean
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
