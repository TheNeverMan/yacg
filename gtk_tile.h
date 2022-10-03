#pragma once
#include<gtkmm.h>
#include<memory>
#include<vector>
#include<string>

#include "logger.h"

using namespace std;

class Gtk_Tile
{
  private:
    Gtk::EventBox Tile_Event_Box;
    Gtk::Image Tile_Image;
    Gtk::Overlay City_Name_Overlay;
    Gtk::Label City_Name_Label;
    int tile_size;
    bool has_city;
  public:
    Gtk_Tile(string texture_path, int t_s);
    Gtk::Widget* Get_Event_Box();
    Gtk::Image* Get_Image();
    void Update_Texture(string tile_texture, string unit_texture, string upgrade_texture, guint32 border_color);
    bool Has_City_Set();
    void Set_City_Name(string city_name);
};
