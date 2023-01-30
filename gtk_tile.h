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
using std::string_view;

class Gtk_Tile
{
  private:
    vector<Scaled_Pixbuf> Pixbufs;
    Glib::RefPtr<Gdk::Pixbuf> Tile_Pixbuf;
    int tile_size;
    bool has_city = false;
    string city_name;
  public:
    void Increase_Tile_Size();
    void Decrease_Tile_Size();
    Gtk_Tile(vector<string_view> Textures, guint32 border_color, int t_s);
    Gtk_Tile(int layers, int t_s);
    void Update_Texture(vector<string_view> Textures, guint32 border_color);
    bool Has_City_Set() const;
    void Set_City_Name(string_view cn);
    void Add_Combat_Overlay();
    void Add_Selection_Overlay();
    void Add_Overlay_From_Path(string_view path);
    Glib::RefPtr<Gdk::Pixbuf> Get_Pixbuf() const;
    string_view Get_City_Name() const;
};
