#include "sound_button.h"

Sound_Button::Sound_Button(string t, string s = "assets/sounds/click-audio.wav")
{
  set_label(t);
  Change_Sound(s);
  signal_clicked().connect(sigc::mem_fun(*this, &Sound_Button::Button_Clicked));
  Main_Provider.Add_CSS(this);
}

void Sound_Button::Change_Sound(string n_s)
{
  Click_Sound_Path.Set_File_Path(n_s);
}

void Sound_Button::Button_Clicked()
{
  Click_Sound_Manager.Play_Sound(Click_Sound_Path.Get_File_Path());
}

Sound_Button::Sound_Button(string t)
{
  set_label(t);
  Change_Sound("assets/sounds/click-audio.wav");
  signal_clicked().connect(sigc::mem_fun(*this, &Sound_Button::Button_Clicked));
  Main_Provider.Add_CSS(this);
}
