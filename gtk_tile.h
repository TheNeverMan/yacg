#pragma once
#include<gtkmm.h>
#include<memory>
#include<vector>
#include<string>

#include "logger.h"
#include "game_css_provider.h"
#include "image_path.h"
#include "scaled_pixbuf.h"

using std::string;
using std::vector;
using std::shared_ptr;

class Gtk_Tile : public Gtk::Overlay
{
  private:
    vector<Scaled_Pixbuf> Pixbufs;
    Gtk::EventBox Tile_Event_Box;
    Gtk::Image Tile_Image;
    Gtk::Label City_Name_Label;
    Game_CSS_Provider Main_Provider;
    int tile_size;
    bool has_city = false;
  public:
    void Increase_Tile_Size();
    void Decrease_Tile_Size();
    Gtk_Tile(vector<string> Textures, guint32 border_color, int t_s);
    Gtk_Tile(int layers, int t_s);
    Gtk::Widget* Get_Event_Box();
    Gtk::Image* Get_Image();
    void Update_Texture(vector<string> Textures, guint32 border_color);
    bool Has_City_Set();
    void Set_City_Name(string city_name);
};
