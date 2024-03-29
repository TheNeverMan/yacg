#pragma once
#include<string>
#include<vector>
#include<array>
#include<tuple>
#include<memory>

#include "newspaper.h"
#include "scaled_gtk_image.h"
#include "themed_dialog.h"
#include "assets_path.h"

using std::shared_ptr;
using std::make_shared;
using std::tuple;

class Newspaper_Dialog : public Themed_Dialog
{
  private:
    Gtk::Box Root_Box;
    Gtk::Box Explanation_Box;
    Gtk::Label Explanation_Label;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::CheckButton Show_Only_Currently_Moving_Player_Events_Button;
    Gtk::Frame Dialog_Root_Frame;
    Gtk::ScrolledWindow Dialog_Scrolled_Window;
    Gtk::Box Events_List_Box;
    Gtk::Frame Turn_Frame;
    Gtk::ScrolledWindow Turn_Window;
    Gtk::Box Turn_Events_Box;
    vector<shared_ptr<Scaled_Gtk_Image>> Event_Images;
    const vector<Newspaper_Event>& Events;
    int currently_moving_player_id;
    void Update_Events(bool only_currently_moving_player);
  protected:
    void Check_Button_Clicked();
  public:
    Newspaper_Dialog(const vector<Newspaper_Event>& events, int currently_moving_player_id);
};
