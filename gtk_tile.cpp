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
  Tile_Event_Box.add(Tile_Image);
  Tile_Event_Box.add_events(Gdk::BUTTON_PRESS_MASK);
  Tile_Image.set_hexpand(true);
  Tile_Image.set_vexpand(true);
  Gtk::EventBox& ref = Tile_Event_Box;
  add(static_cast<Gtk::Widget&>(ref));
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
  Tile_Event_Box.add(Tile_Image);
  Tile_Event_Box.add_events(Gdk::BUTTON_PRESS_MASK);
  Tile_Image.set_hexpand(true);
  Tile_Image.set_vexpand(true);
  Gtk::EventBox& ref = Tile_Event_Box;
  add(static_cast<Gtk::Widget&>(ref));
}

Gtk::Widget* Gtk_Tile::Get_Event_Box()
{
  return static_cast<Gtk::Widget*>(&Tile_Event_Box);
}

Gtk::Image* Gtk_Tile::Get_Image()
{
  return &Tile_Image;
}

bool Gtk_Tile::Has_City_Set()
{
  return has_city;
}

void Gtk_Tile::Set_City_Name(string city_name)
{
  City_Name_Label.set_text(city_name);
  City_Name_Label.set_halign(Gtk::ALIGN_CENTER);
  City_Name_Label.set_valign(Gtk::ALIGN_END);
  Gtk::Label& ref = City_Name_Label;
  add_overlay(static_cast<Gtk::Widget&>(ref));
  set_overlay_pass_through(static_cast<Gtk::Widget&>(ref), false);
  string class_name = "small_label";
  Main_Provider.Add_CSS_With_Class(&City_Name_Label, class_name);
  City_Name_Label.show();
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

  Tile_Image.set(Scaled_Pixbuf);
  Tile_Image.set_size_request(tile_size, tile_size);
}

void Gtk_Tile::Increase_Tile_Size()
{
  tile_size = tile_size + 4;
  Glib::RefPtr<Gdk::Pixbuf> Current_Pixbuf = Tile_Image.get_pixbuf();
  Glib::RefPtr<Gdk::Pixbuf> Scaled_Pixbuf;
  Scaled_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  double x_proportion = static_cast<double>(tile_size) / static_cast<double>(Current_Pixbuf->get_width());
  double y_proportion = static_cast<double>(tile_size) / static_cast<double>(Current_Pixbuf->get_height());
  Current_Pixbuf->scale(Scaled_Pixbuf, 0, 0, tile_size, tile_size, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
  Tile_Image.set(Scaled_Pixbuf);
  Tile_Image.set_size_request(tile_size, tile_size);
}

void Gtk_Tile::Decrease_Tile_Size()
{
  tile_size = tile_size - 4;
  Glib::RefPtr<Gdk::Pixbuf> Current_Pixbuf = Tile_Image.get_pixbuf();
  Glib::RefPtr<Gdk::Pixbuf> Scaled_Pixbuf;
  Scaled_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  double x_proportion = static_cast<double>(tile_size) / static_cast<double>(Current_Pixbuf->get_width());
  double y_proportion = static_cast<double>(tile_size) / static_cast<double>(Current_Pixbuf->get_height());
  Current_Pixbuf->scale(Scaled_Pixbuf, 0, 0, tile_size, tile_size, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
  Tile_Image.set(Scaled_Pixbuf);
  Tile_Image.set_size_request(tile_size, tile_size);
}
