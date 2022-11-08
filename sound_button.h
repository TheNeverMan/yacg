#pragma once
#include<string>
#include<gtkmm.h>

#include "audio_path.h"
#include "sound_manager.h"
#include "game_css_provider.h"

using std::string;

class Sound_Button : public Gtk::Button
{
  private:
    Audio_Path Click_Sound_Path;
    Sound_Manager Click_Sound_Manager;
    Game_CSS_Provider Main_Provider;
  protected:
    void Change_Sound(string n_s);
    void Button_Clicked();
  public:
    Sound_Button(string t, string s);
    Sound_Button(string t);
};
