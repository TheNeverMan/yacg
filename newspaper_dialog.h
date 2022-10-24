#pragma once
#include<string>
#include<vector>

#include "newspaper.h"
#include "scaled_gtk_image.h"
#include "themed_dialog.h"
#include "assets_path.h"

class Newspaper_Dialog : public Themed_Dialog
{
  private:
    Gtk::Box Root_Box;
    Gtk::Box Explanation_Box;
    Gtk::Label Explanation_Label;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Frame Dialog_Root_Frame;
    Gtk::ScrolledWindow Dialog_Scrolled_Window;
    Gtk::Box Events_List_Box;
  public:
    Newspaper_Dialog(vector<string> events);
};
