#pragma once
#include<memory>
#include<string>
#include<gtkmm.h>

#include "intro_window.h"
#include "game_window.h"
#include "logger.h"

class Intro_Window;
class Game_Window;

class Window_Manager
{
public:
    Window_Manager(int argc, char* argv[]);
    int Run();
    void Close_Game();
    void Show_Intro_Window();
    void Show_Game_Window(Settings_Manager m_s_m);
    void Show_Game_Window_Load_From_File(Settings_Manager m_s_m, string path);
private:
    void Create_Intro_Window();
    void Create_Game_Window(Settings_Manager m_s_m);
    void Create_Game_Window_From_File(Settings_Manager m_s_m, string path);
    void Switch_Current_Window(int specifier);
private:
    Glib::RefPtr<Gtk::Application> Main_Application;
    shared_ptr<Gtk::Window> Current_Window;
    shared_ptr<Intro_Window> Start_Window;
    shared_ptr<Game_Window> Main_Game_Window;
    bool is_running = false;
};
