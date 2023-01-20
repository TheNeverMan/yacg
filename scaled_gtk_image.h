#pragma once
#include<gtkmm.h>
#include<string>

#include "image_path.h"

using std::string;
using std::string_view;

class Scaled_Gtk_Image
{
  private:
    Gtk::Image Main_Image;
    Image_Path Path;
    int size_x;
    int size_y;
  public:
    Scaled_Gtk_Image(string p, int s_x, int s_y);
    void Resize_Image(int s_x, int s_y);
    void Change_Path(string_view p);
    Scaled_Gtk_Image(int s_x, int s_y);
    Gtk::Image& Get_Gtk_Image() const;
};
