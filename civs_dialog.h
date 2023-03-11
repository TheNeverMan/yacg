#pragma once
#include<gtkmm.h>
#include<vector>
#include<memory>
#include<string>
#include<algorithm>

#include "themed_dialog.h"
#include "civ.h"
#include "assets_path.h"
#include "image_path.h"
#include "scaled_pixbuf.h"
#include "scaled_gtk_image.h"

using std::shared_ptr;
using std::vector;
using std::string;
using std::sort;

class Civs_Dialog : public Themed_Dialog
{
  private:
    vector<Civ>& Players;
    Gtk::Label Explanation_Label;
    Gtk::Frame Dialog_Root_Frame;
    Gtk::ScrolledWindow Dialog_Scrolled_Window;
    Gtk::Grid Civs_List_Box;
    Gtk::Box Explanation_Box;
    Gtk::CheckButton Hide_Dead_Button;
    Scaled_Gtk_Image Explanation_Image;
    int rows;
    void Update_Players();
    string Get_Number_In_Thousands(int number);
    struct Row
    {
      Gtk::Image* Flag_Image;
      Gtk::Image* Color_Image;
      Gtk::Label* Name_Label;
      Gtk::Label* Points_Label;
      Gtk::Label* Army_Label;
      Gtk::Label* Population_Label;
      Gtk::Label* Capital_Label;
      int points;
    };
    void Attach_Row(Row& Row_To_Attach, int index);
  public:
    Civs_Dialog(vector<Civ>& p);
};
