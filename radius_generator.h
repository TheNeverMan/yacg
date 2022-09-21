#pragma once
#include<vector>
#include<array>

using namespace std;

class Radius_Generator
{
  private:
    int x_size;
    int y_size;
    bool Is_Out_Of_Bounds(int x, int y);
    void Add_Tile_If_Not_Out_Of_Bounds(int x, int y, vector<array<int, 2>> *Vector);
  public:
    Radius_Generator(int x, int y);
    vector<array<int, 2>> Get_Radius_For_Coords(int x, int y, int radius);
};
