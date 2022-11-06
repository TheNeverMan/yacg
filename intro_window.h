#pragma once
#include <gtkmm.h>
#include<string>

#include "window_manager.h"
#include "game_css_provider.h"
#include "settings_manager.h"
#include "save_loader_dialog.h"

class Window_Manager;

class Intro_Window : public Gtk::Window
{
  private:
    Window_Manager* Main_Manager;
    Game_CSS_Provider Main_Provider;
    Settings_Manager Main_Settings_Manager;
  protected:
    Gtk::Button Play_Button;
    Gtk::Box UI_Root_Box;
    Gtk::Button Quit_Button;
    Gtk::Button Settings_Button;
    Gtk::Button About_Button;
    Gtk::Button Load_Game_Button;
    Gtk::Button Load_Autosave_Button;
    Gtk::Image Logo;
    Gtk::Label Version_Label;
    void Play_Button_Clicked();
    void Quit_Button_Clicked();
    void Settings_Button_Clicked();
    void About_Button_Clicked();
    void Load_Game_Button_Clicked();
    void Load_Autosave_Button_Clicked();
    void Change_Startup_Tip_Value(Gtk::CheckButton* Startup_Tip_Button);
    void Change_Tile_Size_Value(Gtk::SpinButton* Tile_Size_Button);
    void Change_Autosave_Value(Gtk::CheckButton* Autosave_Button);
    void Change_Autoresize_Value(Gtk::CheckButton* Autoresize_Button);
    void Change_Mute_Value(Gtk::CheckButton* Mute_Button);
  public:
    Intro_Window(Window_Manager* m_m);
};
