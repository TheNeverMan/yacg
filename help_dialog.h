#pragma once

#include "themed_dialog.h"
#include "slaved_sound_button.h"
#include "sound_manager.h"
#include "help_manager.h"
#include "scaled_gtk_image.h"

class Help_Dialog : public Themed_Dialog
{
  private:
    Gtk::Box Root_Box;
    Gtk::Box Explanation_Box;
    Gtk::Label Explanation_Label;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Frame Root_Frame;
    Gtk::Box Help_Buttons_Box;
    Slaved_Sound_Button Help_Text_Button;
    Slaved_Sound_Button Basic_Tutorial_Button;
    Slaved_Sound_Button Keybindings_Tutorial_Button;
    Sound_Manager Main_Sound_Manager;
    Help_Manager Main_Help_Manager;
  protected:
    void Help_Text_Button_Clicked();
    void Basic_Tutorial_Button_Clicked();
    void Keybindings_Tutorial_Button_Clicked();
  public:
    Help_Dialog();
};
