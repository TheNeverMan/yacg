#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<random>
#include<ctime>
#include<cmath>

#include "map_generator_data.h"
#include "tile.h"
#include "map.h"
#include "upgrade.h"
#include "logger.h"

using namespace std;

class Map_Generator
{
  private:
    Map_Generator_Data User_Data;
    vector<Tile> Tiles;
    vector<Upgrade> Upgrades;
    Map Game_Map;
    void Clean_Map();
    void Generate_Continent_At_Coords(int x, int y, unsigned long long size, int depth);
    void Fill_Territory(string tile_name,int start_x, int start_y, int end_x, int end_y);
    void Fray_Territory(int start_x, int start_y, int end_x, int end_y);
  public:
    Map_Generator(Map_Generator_Data mpg, vector<Tile> t, vector<Upgrade> u);
    string Info();
    Map Generate_Map_Using_User_Data();
};
