#pragma once

#include "settings_manager.h"
#include "themed_dialog.h"
#include "sound_manager.h"
#include "scaled_gtk_image.h"

class Settings_Dialog : public Themed_Dialog
{
  private:
    Gtk::CheckButton Startup_Tip_Button;
    Gtk::CheckButton Autosave_Button;
    Gtk::CheckButton Autoresize_Button;
    Gtk::CheckButton Mute_Button;
    Gtk::CheckButton Mute_Music_Button;
    Settings_Manager Main_Settings_Manager;
    Gtk::Frame Dialog_Settings_Frame;
    Gtk::Label Tile_Size_Label;
    Gtk::Box Explanation_Box;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Label Explanation_Label;
    Gtk::Box Dialog_Root_Box;
    Gtk::SpinButton Tile_Size_Switch;
  protected:
    void Change_Startup_Tip_Value();
    void Change_Tile_Size_Value();
    void Change_Autosave_Value();
    void Change_Autoresize_Value();
    void Change_Mute_Value();
    void Change_Music_Mute_Value();
  public:
    Settings_Dialog();
    ~Settings_Dialog();
};
