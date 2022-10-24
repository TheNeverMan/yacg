#pragma once
#include<gtkmm.h>
#include<vector>
#include<memory>
#include<string>

#include "themed_dialog.h"
#include "civ.h"
#include "assets_path.h"
#include "image_path.h"
#include "scaled_pixbuf.h"
#include "scaled_gtk_image.h"

using namespace std;

class Civs_Dialog : public Themed_Dialog
{
  private:
    vector<Civ> Players;
    Gtk::Label Explanation_Label;
    Gtk::Frame Dialog_Root_Frame;
    Gtk::ScrolledWindow Dialog_Scrolled_Window;
    Gtk::Grid Civs_List_Box;
    Gtk::Box Explanation_Box;
    Gtk::CheckButton Hide_Dead_Button;
    Scaled_Gtk_Image Explanation_Image;
    int rows;
    void Update_Players();
  public:
    Civs_Dialog(vector<Civ> p);
};
