#pragma once
#include<memory>
#include<string>
#include<gtkmm.h>

#include "audio_path.h"
#include "sound_manager.h"
#include "game_css_provider.h"
#include "scaled_gtk_image.h"
#include "image_path.h"

using std::string;
using std::shared_ptr;
using std::make_shared;

class Slaved_Sound_Button : public Gtk::Button
{
  private:
    Audio_Path Click_Sound_Path;
    Sound_Manager *Click_Sound_Manager;
    Game_CSS_Provider Main_Provider;
    shared_ptr<Scaled_Gtk_Image> Main_Icon;
    string label;
  protected:
    void Change_Sound(string n_s);
    void Button_Clicked();
  public:
    Slaved_Sound_Button(string t, string s);
    Slaved_Sound_Button(string t, Sound_Manager *c_s_m);
    void Change_Icon(string i_p);
};
