#pragma once
#include<vector>
#include<array>
#include<string>

using std::string;
using std::vector;
using std::array;

class Radius_Generator
{
  private:
    int x_size;
    int y_size;
    bool Is_Out_Of_Bounds(int x, int y) const;
    void Add_Tile_If_Not_Out_Of_Bounds(int x, int y, vector<array<int, 2>>& Vector) const;
  public:
    Radius_Generator(int x, int y);
    Radius_Generator();
    vector<array<int, 2>> Get_Radius_For_Coords(int x, int y, int radius) const;
    void Set_Size(int x, int y);
};
