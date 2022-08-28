#include "intro_window.h"
#include "globals.h"

Intro_Window::Intro_Window(Window_Manager* m_m)
{
  Main_Manager = m_m;
  UI_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Play_Button = Gtk::Button("Play!");
  Quit_Button = Gtk::Button("Exit");
  About_Button = Gtk::Button("About");
  Load_Game_Button = Gtk::Button("Load Game");
  Settings_Button = Gtk::Button("Settings");
  Version_Label = Gtk::Label(Display_Version());
  add(UI_Root_Box);
  UI_Root_Box.pack_start(Play_Button);
  UI_Root_Box.pack_start(Load_Game_Button);
  UI_Root_Box.pack_start(Settings_Button);
  UI_Root_Box.pack_start(About_Button);
  UI_Root_Box.pack_start(Quit_Button);
  UI_Root_Box.pack_start(Version_Label);
  Play_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Play_Button_Clicked) );
  Quit_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Quit_Button_Clicked) );

  show_all_children();
}

void Intro_Window::Play_Button_Clicked()
{
  Main_Manager->Switch_Current_Window(3);
}

void Intro_Window::Quit_Button_Clicked()
{
  Main_Manager->Switch_Current_Window(1);
}
