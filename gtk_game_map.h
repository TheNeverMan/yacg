#pragma once
#include<string>
#include "thread.h"

#include<algorithm>
#include<vector>
#include<memory>

#include "gtk_tile.h"
#include "magic_map_generation_thread_communicator.h"
#include "game.h"

class Magic_Map_Generation_Thread_Communicator;

class Gtk_Game_Map
{
  private:
    vector<shared_ptr<Gtk_Tile>> Game_Map;
    int x_size;
    int y_size;
    int tile_size;
    bool is_in_thread;
    mutex Main_Mutex;
  public:
    void Generate_Map(Game* Main_Game, Magic_Map_Generation_Thread_Communicator* Thread_Portal);
    Gtk_Game_Map(int x_s, int y_s, int t_s);
    Gtk_Game_Map();
    void Set_Tile_Size(int t_s);
    void Update_Tile(vector<string> Textures, guint32 border_color, int x, int y);
    void Set_Map_Size(int x_s, int y_s);
    void Zoom_In();
    void Zoom_Out();
    shared_ptr<Gtk_Tile> Get_Gtk_Tile(int x, int y);
};
