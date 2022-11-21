#include "slaved_sound_button.h"

Slaved_Sound_Button::Slaved_Sound_Button(string t, string s = "assets/sounds/click-audio.mp3")
{
  set_label(t);
  Change_Sound(s);
  signal_clicked().connect(sigc::mem_fun(*this, &Slaved_Sound_Button::Button_Clicked));
  Main_Provider.Add_CSS(this);
}

void Slaved_Sound_Button::Change_Sound(string n_s)
{
  Click_Sound_Path.Set_File_Path(n_s);
}

void Slaved_Sound_Button::Button_Clicked()
{
  Click_Sound_Manager->Play_Sound(Click_Sound_Path.Get_File_Path());
}

Slaved_Sound_Button::Slaved_Sound_Button(string t, Sound_Manager* c_s_m)
{
  set_label(t);
  Click_Sound_Manager = c_s_m;
  Change_Sound("assets/sounds/click-audio.mp3");
  signal_clicked().connect(sigc::mem_fun(*this, &Slaved_Sound_Button::Button_Clicked));
  Main_Provider.Add_CSS(this);
}

void Slaved_Sound_Button::Change_Icon(string i_p)
{
  Image_Path Icon_Path(i_p);
  Main_Icon = make_shared<Scaled_Gtk_Image>(Icon_Path.Get_File_Path(), 24, 24);
  set_image(*(Main_Icon->Get_Gtk_Image()));
}
