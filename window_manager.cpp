#include "window_manager.h"

Window_Manager::Window_Manager(int argc, char* argv[])
{
  Logger::Prepare_Logger("miniyacg-log.log", true);
  Logger::Log_Info("Preparing Windows...");
  Main_Application = Gtk::Application::create(argc, argv, "org.yacg.miniyacg");
  Create_Intro_Window();
}

void Window_Manager::Create_Intro_Window()
{
  Start_Window = make_shared<Intro_Window>(this);
}

void Window_Manager::Create_Game_Window(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players, bool spectator_mode)
{
  Main_Game_Window = make_shared<Game_Window>(this, m_s_m, Map_Data, players, false, spectator_mode);
}

void Window_Manager::Create_Game_Window_Load_Starting_Positions_From_File(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players, bool spectator_mode)
{
  Main_Game_Window = make_shared<Game_Window>(this, m_s_m, Map_Data, players, true, spectator_mode);
}

void Window_Manager::Create_Game_Window_From_File(Settings_Manager m_s_m, string path, bool spectator_mode)
{
  Main_Game_Window = make_shared<Game_Window>(this, m_s_m, path, spectator_mode);
}

void Window_Manager::Create_Game_Creation_Window(Settings_Manager m_s_m)
{
  Creation_Window = make_shared<Game_Creation_Window>(this, m_s_m);
}

int Window_Manager::Run()
{
    if (!is_running) {

        // Set the current window to entry window for startup
        Current_Window = Start_Window;
        return Main_Application->run(*Start_Window);
    } else {
        return -1;
    }
}


void Window_Manager::Close_Game()
{
  Switch_Current_Window(1);
}

void Window_Manager::Show_Intro_Window()
{
  Switch_Current_Window(2);
}

void Window_Manager::Show_Game_Creation_Window(Settings_Manager m_s_m)
{
  Create_Game_Creation_Window(m_s_m);
  Switch_Current_Window(4);
}

void Window_Manager::Show_Game_Window(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players, bool spectator_mode)
{
  Create_Game_Window(m_s_m, Map_Data, players, spectator_mode);
  Switch_Current_Window(3);
}

void Window_Manager::Show_Game_Window_Load_Starting_Positions(Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players, bool spectator_mode)
{
  Create_Game_Window_Load_Starting_Positions_From_File(m_s_m, Map_Data, players, spectator_mode);
  Switch_Current_Window(3);
}

void Window_Manager::Show_Game_Window_Load_From_File(Settings_Manager m_s_m, string path, bool spectator_mode)
{
  Create_Game_Window_From_File(m_s_m, path, spectator_mode);
  Switch_Current_Window(3);
}

void Window_Manager::Switch_Current_Window(int specifier)
{

    if (specifier != 0)
    {
        Main_Application->hold();
        Current_Window->hide();
        Main_Application->remove_window(*Current_Window);
    }
    if(specifier == 1)
    {
      is_running = false;
      Logger::Log_Info("Exiting...");
      Main_Application->release();
      Logger::Log_Info("Closing Game!");
      Main_Application->quit();
      return;
    }
    switch (specifier)
    {
        case 2:
        {
          Logger::Log_Info("Changing to Intro Window!");
          Current_Window = Start_Window;
          break;
        }
        case 3:
        {
          Logger::Log_Info("Changing to Game Window!");
          Current_Window = Main_Game_Window;
          break;
        }
        case 4:
        {
          Logger::Log_Info("Changing to Game Creation Window!");
          Current_Window = Creation_Window;
          break;
        }
    }
    Main_Application->add_window(*Current_Window);
    Current_Window->show();
    Main_Application->release();
}
