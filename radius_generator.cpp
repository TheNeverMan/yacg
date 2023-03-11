#include "radius_generator.h"
#include<iostream>

Radius_Generator::Radius_Generator(int x, int y)
{
  x_size = x;
  y_size = y;
}

Radius_Generator::Radius_Generator()
{

}

void Radius_Generator::Set_Size(int x, int y)
{
  x_size = x;
  y_size = y;
}

bool Radius_Generator::Is_Out_Of_Bounds(int x, int y) const
{
  if(x >= x_size || y >= y_size)
    return true;
  if(x < 0 || y < 0)
    return true;
  return false;
}

void Radius_Generator::Add_Tile_If_Not_Out_Of_Bounds(int x, int y, vector<array<int, 2>>& Vector) const
{
  if(Is_Out_Of_Bounds(x,y))
    return;
  Vector.push_back({x,y});
}

vector<array<int, 2>> Radius_Generator::Get_Radius_For_Coords(int x, int y, int radius) const
{
  vector<array<int, 2>> out;
  Add_Tile_If_Not_Out_Of_Bounds(x,y,out);
  if(radius > 0)
  {
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y+1,out);
  }
  if(radius > 1)
  {
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y+1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y+1,out);
  }
  if(radius > 2)
  {
    Add_Tile_If_Not_Out_Of_Bounds(x-2,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y-2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+2,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y+2,out);
  }
  if(radius > 3)
  {
    Add_Tile_If_Not_Out_Of_Bounds(x-2,y+1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y-2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+2,y+1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y+2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-2,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y-2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+2,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y+2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-2,y-2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+2,y-2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+2,y+2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-2,y+2,out);
  }
  if(radius > 4)
  {
    Add_Tile_If_Not_Out_Of_Bounds(x+3,y+1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+3,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+3,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-3,y+1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-3,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-3,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y+3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y+3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y+3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y-3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y-3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y-3,out);
  }
  if(radius > 5)
  {
    Add_Tile_If_Not_Out_Of_Bounds(x+3,y+3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+3,y+2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+3,y-3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+3,y-2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-3,y-3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-3,y-2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-3,y+3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-3,y+2,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-2,y+3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-2,y-3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+2,y-3,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+2,y+3,out);
  }
  if(radius > 6)
  {
    Add_Tile_If_Not_Out_Of_Bounds(x+4,y+1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+4,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+4,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-4,y+1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-4,y-1,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-4,y,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y+4,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y+4,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y+4,out);
    Add_Tile_If_Not_Out_Of_Bounds(x+1,y-4,out);
    Add_Tile_If_Not_Out_Of_Bounds(x-1,y-4,out);
    Add_Tile_If_Not_Out_Of_Bounds(x,y-4,out);
  }
  return out;
}
