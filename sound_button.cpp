#include "sound_button.h"

Sound_Button::Sound_Button(string t, string s = "assets/sounds/click-audio.mp3") : label(t)
{
  set_label(t);
  Change_Sound(s);
  signal_clicked().connect(sigc::mem_fun(*this, &Sound_Button::Button_Clicked));
  Main_Provider.Add_CSS(this);
}

void Sound_Button::Change_Sound(string_view n_s)
{
  Click_Sound_Path.Set_File_Path(n_s);
}

void Sound_Button::Button_Clicked()
{
  Sound_Manager::Play_Sound(Click_Sound_Path.Get_File_Path());
}

void Sound_Button::Change_Icon(string_view i_p)
{
  remove();
  Image_Path Icon_Path((string(i_p)));
  Main_Icon = make_shared<Scaled_Gtk_Image>(Icon_Path.Get_File_Path().data(), 24, 24);
  add_pixlabel(Icon_Path.Get_File_Path().data(), label);
}

Sound_Button::Sound_Button(string t) : label(t)
{
  set_label(t);
  Change_Sound("assets/sounds/click-audio.mp3");
  signal_clicked().connect(sigc::mem_fun(*this, &Sound_Button::Button_Clicked));
  Main_Provider.Add_CSS(this);
}
