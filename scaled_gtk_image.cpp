#include "scaled_gtk_image.h"

Scaled_Gtk_Image::Scaled_Gtk_Image(string p, int s_x, int s_y) : Path(p), size_x(s_x), size_y(s_y)
{
  Glib::RefPtr<Gdk::Pixbuf> Image_Pixbuf;
  Glib::RefPtr<Gdk::Pixbuf> Finished_Pixbuf;
  Finished_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, size_x, size_y);
  Image_Pixbuf = Gdk::Pixbuf::create_from_file(Path.Get_File_Path());
  Image_Pixbuf->scale(Finished_Pixbuf, 0, 0, size_x, size_y, 0, 0, (size_x / Image_Pixbuf->get_width()), (size_y / Image_Pixbuf->get_height()), Gdk::INTERP_BILINEAR);
  Main_Image = Gtk::Image(Finished_Pixbuf);
}

Gtk::Image* Scaled_Gtk_Image::Get_Gtk_Image()
{
  return &Main_Image;
}

void Scaled_Gtk_Image::Resize_Image(int s_x, int s_y)
{
  Glib::RefPtr<Gdk::Pixbuf> Image_Pixbuf;
  Glib::RefPtr<Gdk::Pixbuf> Finished_Pixbuf;
  Finished_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, s_x, s_y);
  Image_Pixbuf = Gdk::Pixbuf::create_from_file(Path.Get_File_Path());
  Image_Pixbuf->scale(Finished_Pixbuf, 0, 0, 64, 64, 0, 0, (s_x / Image_Pixbuf->get_width()), (s_y / Image_Pixbuf->get_height()), Gdk::INTERP_BILINEAR);
  Main_Image.set(Finished_Pixbuf);
  size_x = s_x;
  size_y = s_y;
}
