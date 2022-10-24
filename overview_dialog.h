#pragma once
#include<string>
#include<vector>

#include "civ.h"
#include "themed_dialog.h"
#include "scaled_gtk_image.h"

using namespace std;

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
  public:
    Overview_Dialog(vector<int> p_i, Civ p, string year);
};
