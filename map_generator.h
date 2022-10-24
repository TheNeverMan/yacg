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

using namespace std;

class Map_Generator
{
  private:
    vector<char> file_buf;
    Map_Generator_Data User_Data;
    vector<Tile> Tiles;
    vector<Upgrade> Upgrades;
    Map Game_Map;
    void Clean_Map();
    void Generate_Continent_At_Coords(int x, int y, unsigned long long size, int depth);
    void Fill_Territory(string tile_name,int start_x, int start_y, int end_x, int end_y);
    void Fray_Territory(int start_x, int start_y, int end_x, int end_y);
    void Noise_Territory(string tile_name, int x, int y, int size);
    xml_node<>* Load_Map(xml_document<>* doc);
    Map Generate_Map_From_File();
    map<char, Tile> Load_Replacements(xml_node<>* Root_Node);
    void Load_Map_From_File(xml_node<>* Root_Node, map<char, Tile> Replacements);
  public:
    Map_Generator(Map_Generator_Data mpg, vector<Tile> t, vector<Upgrade> u);
    string Info();
    Map Generate_Map_Using_User_Data();
    map<string, array<int,2>> Fetch_Starting_Positions_From_Map_Data();
};
