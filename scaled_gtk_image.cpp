#include "scaled_gtk_image.h"

Scaled_Gtk_Image::Scaled_Gtk_Image(string p, int s_x, int s_y) : Path(p), size_x(s_x), size_y(s_y)
{
  Resize_Image(size_x, size_y);
}

Gtk::Image& Scaled_Gtk_Image::Get_Gtk_Image() const
{
  return &Main_Image;
}

void Scaled_Gtk_Image::Resize_Image(int s_x, int s_y)
{
  Glib::RefPtr<Gdk::Pixbuf> Image_Pixbuf;
  Glib::RefPtr<Gdk::Pixbuf> Finished_Pixbuf;
  Finished_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, s_x, s_y);
  Image_Pixbuf = Gdk::Pixbuf::create_from_file(Path.Get_File_Path());
  double x_proportion = 0;
  double y_proportion = 0;
  x_proportion = static_cast<double>(s_x) / static_cast<double>(Image_Pixbuf->get_width());
  y_proportion = static_cast<double>(s_y)  / static_cast<double>(Image_Pixbuf->get_height());
  Image_Pixbuf->scale(Finished_Pixbuf, 0, 0, s_x, s_y, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST);

  Main_Image.set(Finished_Pixbuf);
  size_x = s_x;
  size_y = s_y;
}

void Scaled_Gtk_Image::Change_Path(string_view p)
{
  Path.Set_File_Path(p);
  Resize_Image(size_x, size_y);
}

Scaled_Gtk_Image::Scaled_Gtk_Image(int s_x, int s_y) : size_x(s_x), size_y(s_y)
{

}
