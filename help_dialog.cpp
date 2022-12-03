#include "help_dialog.h"

Help_Dialog::Help_Dialog() : Themed_Dialog("Help"), Help_Text_Button("Help Manual", &Main_Sound_Manager), Basic_Tutorial_Button("Basic Tutorial", &Main_Sound_Manager), Keybindings_Tutorial_Button("Keybindings", &Main_Sound_Manager), Explanation_Image("assets/textures/tip.png",64,64)
{
  auto* Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Learn how to play the game!");
  Root_Frame = Gtk::Frame("Help");
  Help_Buttons_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Help_Text_Button.Change_Icon("assets/textures/icons/help-icon.svg.png");
  Basic_Tutorial_Button.Change_Icon("assets/textures/icons/help-icon.svg.png");
  Keybindings_Tutorial_Button.Change_Icon("assets/textures/icons/help-icon.svg.png");

  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Explanation_Box);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  Root_Box.pack_start(Root_Frame);
  Root_Frame.add(Help_Buttons_Box);
  Help_Buttons_Box.pack_start(Help_Text_Button);
  Help_Buttons_Box.pack_start(Basic_Tutorial_Button);
  Help_Buttons_Box.pack_start(Keybindings_Tutorial_Button);

  Help_Text_Button.signal_clicked().connect( sigc::mem_fun(*this, &Help_Dialog::Help_Text_Button_Clicked) );
  Basic_Tutorial_Button.signal_clicked().connect( sigc::mem_fun(*this, &Help_Dialog::Basic_Tutorial_Button_Clicked) );
  Keybindings_Tutorial_Button.signal_clicked().connect( sigc::mem_fun(*this, &Help_Dialog::Keybindings_Tutorial_Button_Clicked) );
}


void Help_Dialog::Help_Text_Button_Clicked()
{
  Main_Help_Manager.Show_Game_Help();
}

void Help_Dialog::Basic_Tutorial_Button_Clicked()
{
  Main_Help_Manager.Show_Basic_Tutorial();
}

void Help_Dialog::Keybindings_Tutorial_Button_Clicked()
{
  Main_Help_Manager.Show_Keybindings_Dialog();
}
