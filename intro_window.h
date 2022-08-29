#pragma once
#include <gtkmm.h>

#include "window_manager.h"
#include "game_css_provider.h"

class Window_Manager;

class Intro_Window : public Gtk::Window
{
  private:
    Window_Manager* Main_Manager;
    Game_CSS_Provider Main_Provider;
  protected:
    Gtk::Button Play_Button;
    Gtk::Box UI_Root_Box;
    Gtk::Button Quit_Button;
    Gtk::Button Settings_Button;
    Gtk::Button About_Button;
    Gtk::Button Load_Game_Button;
    Gtk::Image Logo;
    Gtk::Label Version_Label;
    void Play_Button_Clicked();
    void Quit_Button_Clicked();
  public:
    Intro_Window(Window_Manager* m_m);
};
