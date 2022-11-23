#pragma once
#include<gtkmm.h>
#include<string>

#include "window_manager.h"
#include "game_css_provider.h"
#include "settings_manager.h"
#include "save_loader_dialog.h"
#include "settings_dialog.h"
#include "apply_button.h"
#include "trait_dialog.h"

class Window_Manager;

class Intro_Window : public Gtk::Window
{
  private:
    Window_Manager* Main_Manager;
    Game_CSS_Provider Main_Provider;
    Settings_Manager Main_Settings_Manager;
  protected:
    Apply_Button Play_Button;
    Gtk::Box UI_Root_Box;
    Apply_Button Quit_Button;
    Apply_Button Settings_Button;
    Apply_Button About_Button;
    Apply_Button Load_Game_Button;
    Apply_Button Civs_Button;
    Apply_Button Load_Autosave_Button;
    Gtk::Image Logo;
    Gtk::Label Version_Label;
    void Play_Button_Clicked();
    void Quit_Button_Clicked();
    void Settings_Button_Clicked();
    void About_Button_Clicked();
    void Load_Game_Button_Clicked();
    void Load_Autosave_Button_Clicked();
    void Civs_Button_Clicked();
  public:
    Intro_Window(Window_Manager* m_m);
};
