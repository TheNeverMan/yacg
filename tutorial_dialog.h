#pragma once
#include<string>

#include "themed_dialog.h"
#include "image_path.h"
#include "text_path.h"

class Tutorial_Dialog : public Themed_Dialog
{
  private:
    Image_Path Explanation_Image_Path;
    Text_Path Explanation_Text_Path;
    Gtk::Box Dialog_Box;
    Gtk::Frame Text_Frame;
    Gtk::Image Explanation_Image;
    Gtk::Label Explanation_Text;
  public:
    Tutorial_Dialog(string i_p, string t_p);
};
