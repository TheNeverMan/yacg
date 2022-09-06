#include "map.h"

Map::Map(vector<Tile> t, vector<Upgrade> u)
{
  Upgrades = u;
  Tiles = t;

}

void Map::Set_Size(int x, int y)
{
  int start = 0;
  while(start < x)
  {
    vector<Owned_Tile> v;
    Game_Map.push_back(v);
    int start_y = 0;
    while(start_y < y)
    {
      //generate random tile
      Owned_Tile tmp;
      tmp.Place = Tiles[0];
      tmp.owner = 0;
      Game_Map[start].push_back(tmp); //to do map generator
      start_y++;
    }
    start++;
  }
}

Map::Map()
{

}

int Map::Get_X_Size()
{
  return Game_Map.size();
}

int Map::Get_Y_Size()
{
  return Game_Map[0].size();
}

void Map::Change_Tile_Owner(int x, int y, int id)
{
  Game_Map[x][y].owner = id;
}

void Map::Change_Tile_Type(int x, int y, Tile t)
{
  if(!(x < 0 || y < 0 || x >= Get_X_Size() || y >= Get_Y_Size()))
    Game_Map[x][y].Place = t;
}

void Map::Upgrade_Tile(int x, int y, Upgrade t_u)
{
  Game_Map[x][y].Place.Upgrade_Tile(t_u);
}

void Map::Print_Map_In_ASCII()
{
  Logger::Log_Error("Unused Function");
}

Tile Map::Get_Tile(int x, int y)
{
  return Game_Map[x][y].Place;
}

bool Map::Is_Tile_Neutral(int x, int y)
{
  if(Game_Map[x][y].owner)
    return false;
  return true;
}

//to be outsourced to some utility file?
//no use std::algorithm
Upgrade Map::Find_Upgrade_By_Name_In_Vector(string n, vector<Upgrade> u)
{
  for(auto &var : u)
  {
    if(var.Get_Name() == n)
      return var;
  }
  //ing od we trust
  Logger::Log_Error("in Find_Upgrade_By_Name_In_Vector: No Upgrade Found");
  throw;

}

void Map::Claim_Tile(int x, int y, int owner)
{
  if(x >= 0 && y >= 0)
    if(x < Get_X_Size() && y < Get_Y_Size())
    {
      if(Get_Owner(x,y) == 0)
        Change_Tile_Owner(x,y,owner);
    }
}

void Map::Claim_Tiles_In_Radius(int x, int y, int owner, int radius)
{
  if(radius > 0)
  {
    Claim_Tile(x-1,y,owner);
    Claim_Tile(x,y-1,owner);
    Claim_Tile(x+1,y,owner);
    Claim_Tile(x,y+1,owner);
  }
  if(radius > 1)
  {
    Claim_Tile(x-1,y-1,owner);
    Claim_Tile(x-1,y+1,owner);
    Claim_Tile(x+1,y-1,owner);
    Claim_Tile(x+1,y+1,owner);
  }
  if(radius > 2)
  {
    Claim_Tile(x-2,y,owner);
    Claim_Tile(x,y-2,owner);
    Claim_Tile(x+2,y,owner);
    Claim_Tile(x,y+2,owner);
  }
  if(radius > 3)
  {
    Claim_Tile(x-2,y+1,owner);
    Claim_Tile(x+1,y-2,owner);
    Claim_Tile(x+2,y+1,owner);
    Claim_Tile(x+1,y+2,owner);
    Claim_Tile(x-2,y-1,owner);
    Claim_Tile(x-1,y-2,owner);
    Claim_Tile(x+2,y-1,owner);
    Claim_Tile(x-1,y+2,owner);
    Claim_Tile(x-2,y-2,owner);
    Claim_Tile(x+2,y-2,owner);
    Claim_Tile(x+2,y+2,owner);
    Claim_Tile(x-2,y+2,owner);
  }
}

void Map::Build_City(int x, int y, int owner,int radius)
{
  Claim_Tile(x,y,owner);
  Build_Upgrade(Find_Upgrade_By_Name_In_Vector("City", Upgrades), x, y, owner, radius);
}

int Map::Get_Owner(int x, int y)
{
  return Game_Map[x][y].owner;
}

vector<int> Map::Get_Netto_Income_For_Player_By_Id(int id, Civ player)
{
  vector<int> out;
  out.push_back(4);
  out.push_back(0);
  int x = Get_X_Size();
  int y = Get_Y_Size();
  int start = 0;
  int start_y = 0;
  while(start < x)
  {
    while(start_y < y)
    {
      if(Get_Owner(start, start_y) == id)
      {
        int prod = Get_Tile(start, start_y).Get_Upgrade().Get_Production();
        int mait =  Get_Tile(start, start_y).Get_Upgrade().Get_Maitenance();
        if(prod > 0 && Get_Tile(start, start_y).Is_Buffed())
        {
          prod++;
          prod = prod + player.How_Many_Times_Has_Trait("O");
          if(player.Get_Active_Goverment_Name() == "Democracy")
          {
            prod++;
          }
        }
        if(Get_Tile(start, start_y).Get_Upgrade().Get_Name() == "Farm")
        {
          prod = prod + player.How_Many_Times_Has_Trait("A");
          if(player.Get_Active_Goverment_Name() == "Monarchy")
            prod++;
        }
        if(Get_Tile(start, start_y).Get_Upgrade().Get_Name() == "City" && player.Get_Active_Goverment_Name() == "Theocracy")
        {
          mait = mait + 2;
        }
        out[0] = out[0] + prod;
        out[1] = out[1] + mait;
      }
      start_y++;
    }
    start++;
    start_y = 0;
  }
  return out;
}

bool Map::Is_Upgrade_In_Radius_By_Name(string upg_name, int x, int y)
{
  bool out = false;
  if((!Is_Tile_Out_Of_Bounds(x+1,y)) && Get_Tile(x+1,y).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x+1,y-1)) && Get_Tile(x+1,y-1).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x-1,y+1)) && Get_Tile(x-1,y+1).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x-1,y-1)) && Get_Tile(x-1,y-1).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x,y-1)) && Get_Tile(x,y-1).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x-1,y)) && Get_Tile(x-1,y).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x,y+1)) && Get_Tile(x,y+1).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x+1,y+1)) && Get_Tile(x+1,y+1).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  if((!Is_Tile_Out_Of_Bounds(x,y)) && Get_Tile(x,y).Get_Upgrade().Get_Name() == upg_name)
    out = true;
  return out;
}

Tile* Map::Get_Tile_Pointer(int x, int y)
{
  return &Game_Map[x][y].Place;
}

void Map::Recalculate_Borders_For_Player_By_Id(int owner, int radius)
{
  int x = Get_X_Size();
  int y = Get_Y_Size();
  int start = 0;
  int start_y = 0;
  while(start < x)
  {
      while(start_y < y)
      {
        if(Get_Owner(start, start_y) == owner)
        {
          if(Get_Tile(start, start_y).Get_Upgrade().Has_Trait("border_expand"))
            Claim_Tiles_In_Radius(start, start_y, owner, radius);
        }
        start_y++;
      }
      start_y = 0;
      start++;
  }
}
void Map::Build_Upgrade(Upgrade upg, int x, int y, int owner, int radius)
{
  Game_Map[x][y].Place.Upgrade_Tile(upg);
  if(upg.Has_Trait("economic_buff"))
    {
      Get_Tile_Pointer(x+1,y)->Buff_Tile();
      Get_Tile_Pointer(x+1,y-1)->Buff_Tile();
      Get_Tile_Pointer(x-1,y+1)->Buff_Tile();
      Get_Tile_Pointer(x-1,y-1)->Buff_Tile();
      Get_Tile_Pointer(x,y-1)->Buff_Tile();
      Get_Tile_Pointer(x-1,y)->Buff_Tile();
      Get_Tile_Pointer(x,y+1)->Buff_Tile();
      Get_Tile_Pointer(x+1,y+1)->Buff_Tile();
    }
  if(upg.Has_Trait("border_expand"))
  {
    Claim_Tiles_In_Radius(x,y,owner,radius);
  }
}

vector<int> Map::Check_If_Path_For_Unit_Exists(int unit_x, int unit_y, int dest_x, int dest_y, Unit u)
{
  int does_exist = 1;
  int cost = 0;
  vector<int> out;
  out.push_back(does_exist);
  out.push_back(cost);
  if(dest_x == 10000)
  {
    out[0] = 0;
    return out;
  }
  if(Get_Tile(unit_x,unit_y).Get_Unit_Owner_Id() == Get_Tile(dest_x, dest_y).Get_Unit_Owner_Id())
  {
    out[0] = 0;
    return out;
  }
  bool there_is_someone_waiting = false;
  if(Get_Tile(dest_x, dest_y).Has_Unit())
    there_is_someone_waiting = true;
  //we don't need to move on tile exactly just close to it and have one point left to attack
  int current_movement_points = u.Get_Current_Actions();
  int form_x;
  int form_y;
  form_x = unit_x;
  form_y = unit_y;
  while(current_movement_points > 0)
  {
    if(unit_x > dest_x)
    {
      unit_x--;
    }
    else if(unit_x < dest_x)
    {
      unit_x++;
    }
    if(unit_y > dest_y)
    {
      unit_y--;
    }
    else if(unit_y < dest_y)
    {
      unit_y++;
    }
    if(unit_x == dest_x && unit_y == dest_y && there_is_someone_waiting)
    {
      current_movement_points--;
      out[1]++;
      if(current_movement_points >= 0)
      {
        out[0] = 1;
        out.push_back(form_x);
        out.push_back(form_y);
        out.push_back(1);
        out.push_back(dest_x);
        out.push_back(dest_y);
        return out;
      }
    }
    if(Get_Tile(unit_x,unit_y).Has_Unit())
      break;
    if(!u.Can_Move_On_Tile_By_Name(Get_Tile(unit_x,unit_y).Get_Name()))
      break;
    if(current_movement_points - Get_Tile(unit_x, unit_y).Get_Movement_Cost() < 0)
      break;
    out[1] = out[1] + Get_Tile(unit_x, unit_y).Get_Movement_Cost();
    current_movement_points = current_movement_points - Get_Tile(unit_x, unit_y).Get_Movement_Cost();
    if(unit_x == dest_x && unit_y == dest_y)
    {
      out[0] = 1;
      out.push_back(unit_x);
      out.push_back(unit_y);
      out.push_back(0);
      out.push_back(0);
      out.push_back(0);
      return out;
    }
    form_x = unit_x;
    form_y = unit_y;
  }
  out[0] = 1;
  out.push_back(form_x);
  out.push_back(form_y);
  out.push_back(0);
  out.push_back(0);
  out.push_back(0);
  return out;
}

void Map::Retake_Tile(int x, int y, int owner)
{
  if(x >= 0 && y >= 0)
    if(x < Get_X_Size() && y < Get_Y_Size())
    {
      if(Get_Owner(x,y) != 0 && Get_Tile(x,y).Get_Upgrade().Get_Name() != "City")
        Change_Tile_Owner(x,y,owner);
    }
}

void Map::Retake_Owner_In_Radius(int x, int y, int owner, int radius)
{
  if(radius > 0)
  {
    Retake_Tile(x-1,y,owner);
    Retake_Tile(x,y-1,owner);
    Retake_Tile(x+1,y,owner);
    Retake_Tile(x,y+1,owner);
  }
  if(radius > 1)
  {
    Retake_Tile(x-1,y-1,owner);
    Retake_Tile(x-1,y+1,owner);
    Retake_Tile(x+1,y-1,owner);
    Retake_Tile(x+1,y+1,owner);
  }
  if(radius > 2)
  {
    Retake_Tile(x-2,y,owner);
    Retake_Tile(x,y-2,owner);
    Retake_Tile(x+2,y,owner);
    Retake_Tile(x,y+2,owner);
  }
  if(radius > 3)
  {
    Retake_Tile(x-2,y+1,owner);
    Retake_Tile(x+1,y-2,owner);
    Retake_Tile(x+2,y+1,owner);
    Retake_Tile(x+1,y+2,owner);
    Retake_Tile(x-2,y-1,owner);
    Retake_Tile(x-1,y-2,owner);
    Retake_Tile(x+2,y-1,owner);
    Retake_Tile(x-1,y+2,owner);
    Retake_Tile(x-2,y-2,owner);
    Retake_Tile(x+2,y-2,owner);
    Retake_Tile(x+2,y+2,owner);
    Retake_Tile(x-2,y+2,owner);
  }
}

void Map::Unclaim_All_Player_Tiles(int player)
{
  int x = Get_X_Size();
  int y = Get_Y_Size();
  int start = 0;
  int start_y = 0;
  while(start < x)
  {
    while(start_y < y)
    {
      if(Get_Owner(start, start_y) == player)
        Change_Tile_Owner(start, start_y, 0);
      start_y++;
    }
    start++;
    start_y = 0;
  }
}

double Map::Get_Defense_Bonus_For_Tile(int x, int y)
{
  double out = 1.0;
  if(Get_Tile(x,y).Has_Trait("minor_def_bonus"))
    out = out + 0.1;
  if(Get_Tile(x,y).Has_Trait("major_def_bonus"))
    out = out + 0.2;
  if(Get_Tile(x,y).Get_Upgrade().Has_Trait("minor_def_bonus"))
    out = out + 0.1;
  if(Get_Tile(x,y).Get_Upgrade().Has_Trait("major_def_bonus"))
    out = out + 0.2;
  return out;
}

Map::Map(xml_node<>* Root_Node)
{
  Logger::Log_Info("Deserializing Map...");
  Deserialize(Root_Node);
}

void Map::Deserialize(xml_node<>* Root_Node)
{
  xml_node<>* Tiles_Node = Root_Node->first_node("tiles");
  for(xml_node<> *Tile_Node = Tiles_Node->first_node("tile"); Tile_Node; Tile_Node = Tile_Node->next_sibling("tile"))
  {
    Tile tmp(Tile_Node);
    Tiles.push_back(tmp);
  }

  xml_node<>* Upgrades_Node = Root_Node->first_node("upgrades");
  for(xml_node<> *Upgrade_Node = Upgrades_Node->first_node("upgrade"); Upgrade_Node; Upgrade_Node = Upgrade_Node->next_sibling("upgrade"))
  {
    Upgrade tmp(Upgrade_Node);
    Upgrades.push_back(tmp);
  }

  int x = 0;
  int y = 0;
  xml_node<>* Tile_Map_Node = Root_Node->first_node("tile_map");
  for(xml_node<> *Row_Node = Tile_Map_Node->first_node("row"); Row_Node; Row_Node = Row_Node->next_sibling("row"))
  {
    vector<Owned_Tile> v;
    Game_Map.push_back(v);
    for(xml_node<> *Owned_Tile_Node = Row_Node->first_node("owned_tile"); Owned_Tile_Node; Owned_Tile_Node = Owned_Tile_Node->next_sibling("owned_tile"))
    {
      Owned_Tile tmp(Owned_Tile_Node);
      tmp.owner = stoi(Owned_Tile_Node->first_attribute("owner_id")->value());
      Game_Map[x].push_back(tmp);
      y++;
    }
    x++;
    y = 0;
  }

}

xml_node<>* Map::Serialize(memory_pool<>* doc)
{
  xml_node<>* Root_Node = doc->allocate_node(node_element, "map");
  Logger::Log_Info("Serializing Map...");

  xml_node<>* Tiles_Node = doc->allocate_node(node_element, "tiles");
  for_each(Tiles.begin(), Tiles.end(), [&](Tile& iterated_tile)
  {
    Tiles_Node->append_node(iterated_tile.Serialize(doc));
  });

  xml_node<>* Upgrades_Node = doc->allocate_node(node_element, "upgrades");
  for_each(Upgrades.begin(), Upgrades.end(), [&](Upgrade& iterated_upgrade)
  {
    Upgrades_Node->append_node(iterated_upgrade.Serialize(doc));
  });

  xml_node<>* Map_Node = doc->allocate_node(node_element, "tile_map");


  for_each(Game_Map.begin(), Game_Map.end(), [&](vector<Owned_Tile>& row)
  {
    xml_node<>* Row_Node = doc->allocate_node(node_element, "row");
    for_each(row.begin(), row.end(), [&](Owned_Tile& tile )
    {
      xml_node<>* Owned_Tile_Node = doc->allocate_node(node_element, "owned_tile");
      Owned_Tile_Node->append_node(tile.Place.Serialize(doc));
      xml_attribute<> *Owner = doc->allocate_attribute("owner_id", doc->allocate_string(to_string(tile.owner).c_str()));
      Owned_Tile_Node->append_attribute(Owner);
      Row_Node->append_node(Owned_Tile_Node);
    });
    Map_Node->append_node(Row_Node);
  });

  Root_Node->append_node(Map_Node);
  Root_Node->append_node(Tiles_Node);
  Root_Node->append_node(Upgrades_Node);
  return Root_Node;
}

vector<int> Map::Find_Owned_Tile_For_Upgrade(int owner_id, string upg_name)
{
  int x = Get_X_Size();
  int y = Get_Y_Size();
  int start = 0;
  int start_y = 0;
  vector<int> out;
  out.push_back(-1);
  out.push_back(-1);
  Upgrade u = Find_Upgrade_By_Name_In_Vector(upg_name, Upgrades);
  while(start < x)
  {
    while(start_y < y)
    {
      if(Get_Owner(start, start_y) == owner_id)
      {
        if(u.Is_Tile_Allowed_By_Name(Get_Tile(start, start_y).Get_Name()) && Get_Tile(start, start_y).Get_Upgrade().Get_Name() == "none")
        {
          out[0] = start;
          out[1] = start_y;
          return out;
        }
      }
      start_y++;
    }
    start++;
    start_y = 0;
  }
  return out;
}

int Map::Count_Tiles_Owned_By_Player(int owner, string tile_name)
{
  int x = Get_X_Size();
  int y = Get_Y_Size();
  int start = 0;
  int start_y = 0;
  int out = 0;
  while(start < x)
  {
    while(start_y < y)
    {
      if(Get_Owner(start, start_y) == owner)
      {
        if(Get_Tile(start, start_y).Get_Name() == tile_name)
        {
          out++;
        }
      }
      start_y++;
    }
    start++;
    start_y = 0;
  }
  return out;
}

int Map::Calculate_Distance_Between_Points(int p_x, int p_y, int g_x, int g_y)
{
  int out = sqrt((abs((p_x - g_x)^2)) + abs(((p_y - g_y)^2)));
  return out;
}

array<int ,2> Map::Get_Closest_Point(int x, int y, vector<array<int ,2>> points)
{
  map<int, array<int, 2>> out;
  for_each(points.begin(), points.end(), [&](array<int, 2> point)
  {
    out[Calculate_Distance_Between_Points(x,y,point[0],point[1])] = point;
  });
  for(auto &var : out)
  {
    return var.second;
  }
  Logger::Log_Error("No points in Get_Closest_Point");
  return {0,0};
}

bool Map::Is_Tile_Out_Of_Bounds(int x, int y)
{
  if(x < 0 || y < 0)
  {
    return true;
  }
  if(x >= Get_X_Size() || y >= Get_Y_Size())
  {
    return true;
  }
  return false;
}

array<int ,2> Map::Find_Closest_Tile_Owned_By_One_Direction(int owner, int x, int y, int depth, int x_dir, int y_dir, Unit u)
{
  depth--;
  array<int, 2> out;
  out[0] = x;
  out[1] = y;
  if(depth <= 0)
    return {10000, 10000};
  if(Is_Tile_Out_Of_Bounds(x,y))
    return {10000, 100000};
  if(! u.Can_Move_On_Tile_By_Name(Get_Tile(x,y).Get_Name()))
    return {10000, 10000};
  if(owner == Get_Owner(x,y))
    return out;
  vector<array<int, 2>> points;
  if(x_dir != 0)
    points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x+x_dir, y, depth, x_dir, y_dir, u));
  if(y_dir != 0)
    points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x, y+y_dir, depth, x_dir, y_dir, u));
  return Get_Closest_Point(x, y, points);
}

array<int, 2> Map::Find_Closest_Tile_Owned_By(int owner, int x, int y, int depth, Unit u)
{
  array<int, 2> out;
  out[0] = x;
  out[1] = y;
  if(Is_Tile_Out_Of_Bounds(x,y))
    return {10000, 100000};

  if(owner == Get_Owner(x,y))
    return out;
  vector<array<int, 2>> points;
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x+1, y+1, depth, 1, 1, u));
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x-1, y-1, depth, -1, -1, u));
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x-1, y+1, depth, -1, 1, u));
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x+1, y-1, depth, 1, -1, u));
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x+1, y, depth, 1, 0, u));
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x-1, y, depth, -1, 0, u));
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x, y+1, depth, 0, 1, u));
  points.push_back(Find_Closest_Tile_Owned_By_One_Direction(owner, x, y-1, depth, 0, -1, u));
  return Get_Closest_Point(x, y, points);
}

vector<int> Map::Find_Direction_Away_From_Borders(int owner, int x, int y, int movement_points, Unit u)
{
  array<int, 2> neutral_tile = Find_Closest_Tile_Owned_By(0, x, y, 10, u);
  return Check_If_Path_For_Unit_Exists(x,y,neutral_tile[0], neutral_tile[1], u);
}

array<int, 2> Map::Find_In_One_Direction_To_Enemy_City_Or_Unit(int owner, int x, int y, int depth, int x_dir, int y_dir, Unit u)
{
  depth--;
  array<int, 2> out;
  out[0] = x;
  out[1] = y;
  if(Is_Tile_Out_Of_Bounds(x,y))
    return {10000, 100000};
  if(owner != Get_Owner(x,y) && Get_Tile(x,y).Get_Upgrade().Get_Name() == "City")
    return out;
  if(Get_Tile(x,y).Has_Unit() && Get_Tile(x,y).Get_Unit_Owner_Id() != owner)
    return out;
  if(depth <= 0)
      return out;

  vector<array<int, 2>> points;
  if(x_dir != 0)
    points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(owner, x+x_dir, y, depth, x_dir, y_dir, u));
  if(y_dir != 0)
    points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(owner, x, y+y_dir, depth, x_dir, y_dir, u));
  return Get_Closest_Point(x,y,points);
}

vector<int> Map::Find_Direction_To_Enemy_City_Or_Unit(int unit_owner_id, int x, int y, int movement_points, Unit u)
{
  int depth = 7;

  vector<array<int, 2>> points;
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x+1, y+1, depth, 1, 1, u));
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x-1, y-1, depth, -1, -1, u));
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x-1, y+1, depth, -1, 1, u));
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x+1, y-1, depth, 1, -1, u));
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x+1, y, depth, 1, 0, u));
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x-1, y, depth, -1, 0, u));
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x, y+1, depth, 0, 1, u));
  points.push_back(Find_In_One_Direction_To_Enemy_City_Or_Unit(unit_owner_id, x, y-1, depth, 0, -1, u));

  array<int, 2> closest_point = Get_Closest_Point(x, y, points);
  return Check_If_Path_For_Unit_Exists(x,y,closest_point[0], closest_point[1], u);
}
