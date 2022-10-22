#include "gtk_tile.h"

Gtk_Tile::Gtk_Tile(string texture_path, int t_s) : Tile_Image(texture_path)
{
  cout << "." << endl;
  tile_size = t_s;
  Tile_Event_Box.add(Tile_Image);
  Tile_Event_Box.add_events(Gdk::BUTTON_PRESS_MASK);
  Tile_Image.set_hexpand(true);
  Tile_Image.set_vexpand(true);
  Gtk::EventBox& ref = Tile_Event_Box;
  City_Name_Overlay.add(static_cast<Gtk::Widget&>(ref));
}

Gtk::Widget* Gtk_Tile::Get_Event_Box()
{
  return static_cast<Gtk::Widget*>(&City_Name_Overlay);
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
//City_Name_Label.set_width_chars(50);
//  City_Name_Label.set_hexpand(true);
  Gtk::Label& ref = City_Name_Label;
  City_Name_Overlay.add_overlay(static_cast<Gtk::Widget&>(ref));
  City_Name_Overlay.set_overlay_pass_through(static_cast<Gtk::Widget&>(ref), false);
  //City_Name_Overlay.set_size_request(City_Name_Label.get_width(), tile_size);
  string class_name = "small_label";
  Main_Provider.Add_CSS_With_Class(&City_Name_Label, class_name);
  City_Name_Label.show();
}

Glib::RefPtr<Gdk::Pixbuf> Gtk_Tile::Create_Scaled_Pixbuf(string p, int x, int y)
{
  Image_Path Path(p);
  Glib::RefPtr<Gdk::Pixbuf> out = Gdk::Pixbuf::create_from_file(Path.Get_File_Path());
  double x_proportion = 0;
  double y_proportion = 0;
  x_proportion = static_cast<double>(x) / static_cast<double>(out->get_width());
  y_proportion = static_cast<double>(y)  / static_cast<double>(out->get_height());
  out->scale(out, 0, 0, out->get_width(), out->get_height(), 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST);
  return out;
}

void Gtk_Tile::Update_Texture(string tile_texture, string unit_texture, string upgrade_texture, guint32 border_color)
{
  cout << "." << endl;
  Glib::RefPtr<Gdk::Pixbuf> tile_pix = Create_Scaled_Pixbuf(tile_texture, tile_size, tile_size);
  Glib::RefPtr<Gdk::Pixbuf> upgrade_pix = Create_Scaled_Pixbuf(upgrade_texture, tile_size, tile_size);;
  Glib::RefPtr<Gdk::Pixbuf> finished_pix;
  Glib::RefPtr<Gdk::Pixbuf> border_pix;
  Glib::RefPtr<Gdk::Pixbuf> unit_pix = Create_Scaled_Pixbuf(unit_texture, tile_size, tile_size);;
  Glib::RefPtr<Gdk::Pixbuf> scaled_pix;

  scaled_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  finished_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  border_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  int border_alpha = 120;
  border_pix->fill(border_color);
  tile_pix->copy_area(0,0,tile_size,tile_size,finished_pix,0,0);
  upgrade_pix->composite(finished_pix,0,0,tile_size,tile_size,0,0,1,1,Gdk::INTERP_NEAREST,255);
  unit_pix->composite(finished_pix,0,0,tile_size,tile_size,0,0,1,1,Gdk::INTERP_NEAREST,255);
  border_pix->composite(finished_pix,0,0,tile_size,tile_size,0.0,0.0,1.0,1.0,Gdk::INTERP_NEAREST,border_alpha);
  finished_pix->scale(scaled_pix, 0, 0, tile_size, tile_size, 0, 0, ((double) tile_size / (double) tile_size), ((double) tile_size / (double) tile_size), Gdk::INTERP_BILINEAR);
  Tile_Image.set(scaled_pix);
  Tile_Image.set_size_request(tile_size, tile_size);
}

void Gtk_Tile::Increase_Tile_Size(int val)
{
  tile_size = tile_size + 4;
}

void Gtk_Tile::Decrease_Tile_Size(int val)
{
  tile_size = tile_size - 4;
}
