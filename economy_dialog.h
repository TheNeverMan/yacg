#pragma once
#include<vector>
#include<string>

#include "themed_dialog.h"
#include "civ.h"
#include "image_path.h"

using namespace std;

class Economy_Dialog : public Themed_Dialog
{
  private:
    vector<int> Player_Income;
    Civ Player;
    Gtk::Box Root_Box;
    Gtk::Box Image_Box;
    Gtk::Frame Dialog_Root_Frame;
    Gtk::Label Finance_Label;
    Gtk::Box Finance_Box;
    Gtk::Image Info_Image;
  public:
    Economy_Dialog(vector<int> p_i, Civ p);
};
