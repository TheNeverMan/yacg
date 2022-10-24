#include "gtk_game_map.h"

Gtk_Game_Map::Gtk_Game_Map(int x_s, int y_s, int t_s)
{
  tile_size = t_s;
  is_in_thread = false;
  x_size = x_s;
  y_size = y_s;
}

Gtk_Game_Map::Gtk_Game_Map()
{
  is_in_thread = false;
}

void Gtk_Game_Map::Set_Tile_Size(int t_s)
{
  tile_size = t_s;
}

void Gtk_Game_Map::Set_Map_Size(int x_s, int y_s)
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  x_size = x_s;
  y_size = y_s;
}

void Gtk_Game_Map::Update_Tile(vector<string> Textures, guint32 border_color, int x, int y)
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Game_Map[y + (x * y_size)]->Update_Texture(Textures, border_color);
}

void Gtk_Game_Map::Zoom_In()
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  tile_size = tile_size + 4;
  for_each(Game_Map.begin(), Game_Map.end(), [&](shared_ptr<Gtk_Tile> Tile){Tile->Increase_Tile_Size();});
}

void Gtk_Game_Map::Zoom_Out()
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  tile_size = tile_size - 4;
  for_each(Game_Map.begin(), Game_Map.end(), [&](shared_ptr<Gtk_Tile> Tile){Tile->Decrease_Tile_Size();});
}

void Gtk_Game_Map::Generate_Map(Game* Main_Game, Magic_Map_Generation_Thread_Communicator* Thread_Portal)
{

    is_in_thread = true;
    long long int tiles;
    long long int start;
    long long int tiles_processed;
    long long int start_y;
    int percent;
    {
      if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
      tiles = x_size * y_size;
      tiles_processed = 0;
      start = 0;
      start_y = 0;
      percent = tiles / 100;
    }
    while(start < x_size)
    {
      while(start_y < y_size)
      {
        {
          if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
          string tile_texture = Main_Game->Get_Map()->Get_Tile(start,start_y).Get_Texture_Path(); //this is incredibly slow pls fix
          string unit_texture = assets_directory_path + "textures" + path_delimeter + "upgrades" + path_delimeter + "none-upgrade-texture.png";
          if(Main_Game->Get_Map()->Get_Tile(start,start_y).Has_Unit())
            unit_texture = Main_Game->Get_Player_By_Id(Main_Game->Get_Map()->Get_Tile(start,start_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(start,start_y).Get_Texture_Path();
          string upgrade_texture = Main_Game->Get_Upgrade_By_Name(Main_Game->Get_Map()->Get_Upgrade(start,start_y)).Get_Texture_Path();
          guint32 border_color = Main_Game->Get_Border_Color_By_Player_Id(Main_Game->Get_Map()->Get_Owner(start,start_y));

          vector<string> Textures {tile_texture, upgrade_texture, unit_texture};
          auto tmp = make_shared<Gtk_Tile>(Textures, border_color, tile_size);
          Game_Map.push_back(tmp);
          //cout << start_y << " " << start << endl;
          start_y++;
          tiles_processed++;
        }
        if(tiles_processed % percent == 0)
        {
          cout << start << " " << start_y << " " << percent << endl;
          Thread_Portal->Notify_Dialog();

        }
      }
      start_y = 0;
      start++;
    }
    is_in_thread = false;
}

shared_ptr<Gtk_Tile> Gtk_Game_Map::Get_Gtk_Tile(int x, int y)
{
  return Game_Map[y + (x * y_size)];
}
