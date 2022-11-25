#pragma once
#include<string>
#include<vector>

#include "civ.h"
#include "themed_dialog.h"
#include "scaled_gtk_image.h"
#include "civ_trait_manager.h"

using std::string;
using std::vector;

class Overview_Dialog : public Themed_Dialog
{
  private:
    vector<int> Player_Income;
    Civ Player;
    Gtk::Box Root_Box;
    Gtk::Frame Dialog_Root_Frame;
    Gtk::Box Stats_Box;
    Gtk::Label Stats_Label;
    Scaled_Gtk_Image Flag_Image;
    Civ_Trait_Manager Trait_Manager;
  public:
    Overview_Dialog(vector<int> p_i, Civ p, string year);
};
