#include "scaled_pixbuf.h"

bool Scaled_Pixbuf::Is_Size_Different(int x_s, int y_s) const
{
  bool out = true;
  if(x_s == x_size && y_s == y_size)
    out = false;
  return out;
}

bool Scaled_Pixbuf::Is_Texture_Different(string_view t_p) const
{
  bool out = true;
  Image_Path tmp(t_p.data());
  if(tmp.Get_File_Path() == Texture_Path.Get_File_Path())
    out = false;
  return out;
}

bool Scaled_Pixbuf::Is_Color_Different(guint32 color) const
{
  bool out = true;
  if(color == Main_Color)
    out = false;
  return out;
}

void Scaled_Pixbuf::Resize(int x_s, int y_s)
{
  if(!Is_Size_Different(x_s, y_s))
    return;
  Glib::RefPtr<Gdk::Pixbuf> Tmp_Pixbuf = Gdk::Pixbuf::create_from_file(Texture_Path.Get_File_Path());
  double x_proportion = static_cast<double>(Tmp_Pixbuf->get_width()) / static_cast<double>(x_s);
  double y_proportion = static_cast<double>(Tmp_Pixbuf->get_height()) / static_cast<double>(y_s);
  Tmp_Pixbuf->scale(Main_Pixbuf, 0, 0, x_s, y_s, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
  x_size = x_s;
  y_size = y_s;
}

void Scaled_Pixbuf::Set_Texture_And_Size(string_view t_p, int x_s, int y_s)
{
  Set_Texture(t_p);
  Resize(x_s, y_s);
}

void Scaled_Pixbuf::Set_Texture(string_view t_p)
{
  if(Is_Texture_Different(t_p))
  {
    Image_Path tmp(t_p.data());
    Main_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, x_size, y_size);
    Glib::RefPtr<Gdk::Pixbuf> Tmp_Pixbuf = Gdk::Pixbuf::create_from_file(tmp.Get_File_Path());

    if(Tmp_Pixbuf->get_width() != x_size || Tmp_Pixbuf->get_height() != y_size)
    {
      double x_proportion = static_cast<double>(x_size) / static_cast<double>(Tmp_Pixbuf->get_width());
      double y_proportion = static_cast<double>(y_size) / static_cast<double>(Tmp_Pixbuf->get_height());
      Tmp_Pixbuf->scale(Main_Pixbuf, 0, 0, x_size, y_size, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
    }
    else
    {
      Main_Pixbuf = Gdk::Pixbuf::create_from_file(tmp.Get_File_Path());
    }
    Texture_Path = tmp;
  }
}

Glib::RefPtr<Gdk::Pixbuf> Scaled_Pixbuf::Get_Pixbuf() const
{
  return Main_Pixbuf;
}

Scaled_Pixbuf::Scaled_Pixbuf(string t_p, int x_s, int y_s) : x_size(x_s), y_size(y_s), Main_Color(0)
{
  Main_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, x_s, y_s);
  Image_Path tmp(t_p);
  Glib::RefPtr<Gdk::Pixbuf> Tmp_Pixbuf = Gdk::Pixbuf::create_from_file(tmp.Get_File_Path());
  if(Tmp_Pixbuf->get_width() != x_size || Tmp_Pixbuf->get_height() != y_size)
  {
    double x_proportion = static_cast<double>(x_size) / static_cast<double>(Tmp_Pixbuf->get_width());
    double y_proportion = static_cast<double>(y_size) / static_cast<double>(Tmp_Pixbuf->get_height());
    Tmp_Pixbuf->scale(Main_Pixbuf, 0, 0, x_size, y_size, 0, 0, x_proportion, y_proportion, Gdk::INTERP_NEAREST );
  }
  else
  {
    Main_Pixbuf = Gdk::Pixbuf::create_from_file(tmp.Get_File_Path());

  }
  Texture_Path = tmp;
}

Scaled_Pixbuf::Scaled_Pixbuf(int x_s, int y_s) : x_size(x_s), y_size(y_s), Main_Color(0)
{
  Main_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, x_s, y_s);
}

void Scaled_Pixbuf::Set_Color(guint32 color)
{
  if(!Is_Color_Different(color))
    return;
  Main_Pixbuf->fill(color);
  Main_Color = color;
}

Scaled_Pixbuf::Scaled_Pixbuf(guint32 color, int x_s, int y_s) : x_size(x_s), y_size(y_s), Texture_Path(assets_directory_path + "textures/broken-texture.png")
{
  Main_Pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, x_s, y_s);
  Main_Pixbuf->fill(color);
  Main_Color = color;
}
