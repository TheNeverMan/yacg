#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<random>
#include<ctime>
#include<cmath>
#include<map>
#include<array>
#include<iterator>

#include "map_generator_data.h"
#include "tile.h"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "map.h"
#include "upgrade.h"
#include "logger.h"
#include "FastNoise.h"

using std::string;
using std::vector;
using std::map;
using std::array;
using std::iterator;

class Map_Generator
{
  private:
    vector<char> file_buf;
    Map_Generator_Data User_Data;
    vector<Tile> Tiles;
    vector<Upgrade> Upgrades;
    Map Game_Map;
    void Clean_Map();
    void Fill_Territory(string tile_name,int start_x, int start_y, int end_x, int end_y);
    xml_node<>* Load_Map(xml_document<>* doc);
    Map Generate_Map_From_File();
    map<char, Tile> Load_Replacements(xml_node<>* Root_Node);
    void Load_Map_From_File(xml_node<>* Root_Node, map<char, Tile> Replacements);
    string Get_Land_Tile_For_Y(int y);
    string Get_Sea_Tile_For_Y(int y);
    string Get_Mountain_Tile_For_Y(int y);
  public:
    Map_Generator(Map_Generator_Data mpg, vector<Tile> t, vector<Upgrade> u);
    string Info();
    Map Generate_Map_Using_User_Data();
    map<string, array<int,2>> Fetch_Starting_Positions_From_Map_Data();
};
