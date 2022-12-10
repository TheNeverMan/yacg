#pragma once
#include<string>
#include<vector>
#include<array>
#include<memory>

#include "newspaper.h"
#include "scaled_gtk_image.h"
#include "themed_dialog.h"
#include "assets_path.h"

using std::shared_ptr;
using std::make_shared;

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
    vector<shared_ptr<Scaled_Gtk_Image>> Event_Images;
  public:
    Newspaper_Dialog(vector<array<string,2>> events);
};
