#pragma once
#include<tuple>
#include<string>
#include<memory>
#include<algorithm>
#include<vector>

#include "themed_dialog.h"
#include "civ.h"
#include "tech.h"
#include "sound_button.h"
#include "scaled_gtk_image.h"
#include "assets_path.h"
#include "slaved_sound_button.h"

using std::string;
using std::tuple;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::get;

class Tech_Dialog : public Themed_Dialog
{
  private:
    vector<shared_ptr<Scaled_Gtk_Image>> Tech_Images;
    Civ Player;
    Sound_Manager Click_Sound_Manager;
    Tech Selected_Tech;
    int research_funds_percent;
    Gtk::Box Root_Box;
    Gtk::Frame Dialog_Root_Frame;
    Gtk::Grid Research_Box;
    Gtk::Box Explanation_Box;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Label Explanation_Label;
    Gtk::SpinButton Research_Percent_Switch;
    Gtk::Label Research_Percent_Info;
    vector<tuple<string, shared_ptr<Gtk::Button>>> Tech_Buttons;
    void Set_Currently_Researched_Button();
  protected:
    void Tech_Button_Clicked(Tech t);
    void Research_Percent_Switch_Changed();
  public:
    Tech_Dialog(Civ p);
    int Get_Research_Percent();
    Tech Get_Selected_Tech();
    ~Tech_Dialog();
};
