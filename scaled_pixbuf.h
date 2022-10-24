#pragma once
#include<string>
#include<gtkmm.h>

#include "image_path.h"
#include "assets_path.h"

using namespace std;

class Scaled_Pixbuf
{
  private:
    Glib::RefPtr<Gdk::Pixbuf> Main_Pixbuf;
    Image_Path Texture_Path;
    guint32 Main_Color;
    int x_size;
    int y_size;
    bool Is_Size_Different(int x_s, int y_s);
    bool Is_Texture_Different(string t_p);
    bool Is_Color_Different(guint32 color);
  public:
    void Set_Texture_And_Size(string t_p, int x_s, int y_s);
    void Set_Texture(string t_p);
    void Set_Color(guint32 color);
    void Resize(int x_s, int y_s);
    Glib::RefPtr<Gdk::Pixbuf> Get_Pixbuf();
    Scaled_Pixbuf(string t_p, int x_s, int y_s);
    Scaled_Pixbuf(int x_s, int y_s);
    Scaled_Pixbuf(guint32 color, int x_s, int y_s);
};
