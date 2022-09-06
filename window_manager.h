#pragma once
#include<memory>
#include<string>
#include<gtkmm.h>

#include "intro_window.h"
#include "game_window.h"
#include "logger.h"
#include "game_creation_window.h"

class Intro_Window;
class Game_Window;
class Game_Creation_Window;

class Window_Manager
{
public:
    Window_Manager(int argc, char* argv[]);
    int Run();
    void Close_Game();
    void Show_Intro_Window();
    void Show_Game_Window(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players);
    void Show_Game_Window_Load_Starting_Positions(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players);
    void Show_Game_Window_Load_From_File(Settings_Manager m_s_m, string path);
    void Show_Game_Creation_Window(Settings_Manager m_s_m);
private:
    void Create_Game_Creation_Window(Settings_Manager m_s_m);
    void Create_Intro_Window();
    void Create_Game_Window(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players);
    void Create_Game_Window_Load_Starting_Positions_From_File(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players);
    void Create_Game_Window_From_File(Settings_Manager m_s_m, string path);
    void Switch_Current_Window(int specifier);
private:
    Glib::RefPtr<Gtk::Application> Main_Application;
    shared_ptr<Gtk::Window> Current_Window;
    shared_ptr<Intro_Window> Start_Window;
    shared_ptr<Game_Window> Main_Game_Window;
    shared_ptr<Game_Creation_Window> Creation_Window;
    bool is_running = false;
};
