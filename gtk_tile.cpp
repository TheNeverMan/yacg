#include "gtk_tile.h"

Gtk_Tile::Gtk_Tile(vector<string> Textures, guint32 border_color, int t_s)
{
  for(string &t_p : Textures)
  {
    Scaled_Pixbuf tmp(t_p, 32, 32);
    Pixbufs.push_back(tmp);
  }
  Scaled_Pixbuf color(border_color, 32, 32);
  Pixbufs.push_back(color);
  tile_size = t_s;
}

Gtk_Tile::Gtk_Tile(int layers, int t_s)
{
  tile_size = t_s;
  int start = 0;
  while(start < layers)
  {
    Scaled_Pixbuf tmp(32,32);
    Pixbufs.push_back(tmp);
    start++;
  }
}

bool Gtk_Tile::Has_City_Set()
{
  return has_city;
}

void Gtk_Tile::Set_City_Name(string cn)
{
  has_city = true;
  this->city_name = cn;
  /*
  City_Name_Label.set_text(city_name);
  City_Name_Label.set_halign(Gtk::ALIGN_CENTER);
  City_Name_Label.set_valign(Gtk::ALIGN_END);
  Gtk::Label& ref = City_Name_Label;
  add_overlay(static_cast<Gtk::Widget&>(ref));
  set_overlay_pass_through(static_cast<Gtk::Widget&>(ref), false);
  string class_name = "small_label";
  Main_Provider.Add_CSS_With_Class(&City_Name_Label, class_name);
  has_city = true;
  City_Name_Label.show();
  */
}

string Gtk_Tile::Get_City_Name()
{
  return city_name;
}



void Gtk_Tile::Update_Texture(vector<string> Textures, guint32 border_color)
{
  Glib::RefPtr<Gdk::Pixbuf> Finished_Pixbuf;
  Glib::RefPtr<Gdk::Pixbuf> Scaled_Pixbuf;
  Finished_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 32, 32);
  Scaled_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  int start = 0;
  while(start < static_cast<int>(Textures.size()))
  {
    Pixbufs[start].Set_Texture_And_Size(Textures[start], 32, 32);
    Pixbufs[start].Get_Pixbuf()->composite(Finished_Pixbuf, 0, 0, static_cast<int>(32), static_cast<int>(32), 0.0, 0.0, 1.0, 1.0, Gdk::INTERP_NEAREST, 255);
    start++;
  }
  Pixbufs[Pixbufs.size() - 1].Set_Color(border_color);
  Pixbufs[Pixbufs.size() - 1].Resize(32, 32);
  Pixbufs[Pixbufs.size() - 1].Get_Pixbuf()->composite(Finished_Pixbuf, 0, 0, static_cast<int>(32), static_cast<int>(32), 0.0, 0.0, 1.0, 1.0, Gdk::INTERP_NEAREST, 120);
  double x_proportion = static_cast<double>(tile_size) / static_cast<double>(32);
  double y_proportion = static_cast<double>(tile_size) / static_cast<double>(32);
  Finished_Pixbuf->scale(Scaled_Pixbuf, 0, 0, tile_size, tile_size, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
  Tile_Pixbuf = Scaled_Pixbuf;
}

Glib::RefPtr<Gdk::Pixbuf> Gtk_Tile::Get_Pixbuf()
{
  return Tile_Pixbuf;
}

void Gtk_Tile::Add_Combat_Overlay()
{
  Glib::RefPtr<Gdk::Pixbuf> selection_texture;
  Glib::RefPtr<Gdk::Pixbuf> scaled_pix;
  selection_texture = Gdk::Pixbuf::create_from_file(assets_directory_path + "textures" + path_delimeter + "other" + path_delimeter + "combat-texture.svg");
  int true_tile_size = tile_size;
  scaled_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, true_tile_size, true_tile_size);
  selection_texture->scale(scaled_pix, 0, 0, true_tile_size, true_tile_size, 0, 0, ((double) true_tile_size / (double) selection_texture->get_width()), ((double) true_tile_size / (double) selection_texture->get_height()), Gdk::INTERP_BILINEAR);
  scaled_pix->composite(Tile_Pixbuf,0,0,true_tile_size,true_tile_size,0,0,1,1,Gdk::INTERP_BILINEAR,255);
}

void Gtk_Tile::Add_Selection_Overlay()
{
  Glib::RefPtr<Gdk::Pixbuf> selection_texture;
  Glib::RefPtr<Gdk::Pixbuf> scaled_pix;
  selection_texture = Gdk::Pixbuf::create_from_file(assets_directory_path + "textures" + path_delimeter + "other" + path_delimeter + "selection-texture.svg");
  int true_tile_size = tile_size;
  scaled_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, true_tile_size, true_tile_size);
  selection_texture->scale(scaled_pix, 0, 0, true_tile_size, true_tile_size, 0, 0, ((double) true_tile_size / (double) selection_texture->get_width()), ((double) true_tile_size / (double) selection_texture->get_height()), Gdk::INTERP_BILINEAR);
  scaled_pix->composite(Tile_Pixbuf,0,0,true_tile_size,true_tile_size,0,0,1,1,Gdk::INTERP_BILINEAR,255);
}

void Gtk_Tile::Increase_Tile_Size()
{
  tile_size = tile_size + 4;
  Glib::RefPtr<Gdk::Pixbuf> Scaled_Pixbuf;
  Scaled_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  double x_proportion = static_cast<double>(tile_size) / static_cast<double>(Tile_Pixbuf->get_width());
  double y_proportion = static_cast<double>(tile_size) / static_cast<double>(Tile_Pixbuf->get_height());
  Tile_Pixbuf->scale(Scaled_Pixbuf, 0, 0, tile_size, tile_size, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
  Tile_Pixbuf = Scaled_Pixbuf;
}

void Gtk_Tile::Decrease_Tile_Size()
{
  tile_size = tile_size - 4;
  Glib::RefPtr<Gdk::Pixbuf> Scaled_Pixbuf;
  Scaled_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  double x_proportion = static_cast<double>(tile_size) / static_cast<double>(Tile_Pixbuf->get_width());
  double y_proportion = static_cast<double>(tile_size) / static_cast<double>(Tile_Pixbuf->get_height());
  Tile_Pixbuf->scale(Scaled_Pixbuf, 0, 0, tile_size, tile_size, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
  Tile_Pixbuf = Scaled_Pixbuf;
}
