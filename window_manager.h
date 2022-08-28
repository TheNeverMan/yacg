#pragma once
#include<memory>
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
    void Switch_Current_Window(int specifier);
private:
    void Create_Intro_Window();
    void Create_Game_Window();

private:
    Glib::RefPtr<Gtk::Application> Main_Application;
    shared_ptr<Gtk::Window> Current_Window;
    shared_ptr<Intro_Window> Start_Window;
    shared_ptr<Game_Window> Main_Game_Window;
    bool is_running = false;
};
