#pragma once
#include<string>
#include<map>
#include<array>

using std::string;
using std::map;
using std::array;

struct Map_Generator_Data
{
  int continents_number;
  int size_x;
  int size_y;
  int oceans_precentage;
  string map_path;
  map<string, array<int,2>> Starting_Positions;
};
