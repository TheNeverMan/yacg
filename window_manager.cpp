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

void Window_Manager::Create_Game_Window()
{
  Main_Game_Window = make_shared<Game_Window>(this);
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
          Create_Game_Window();
          Logger::Log_Info("Changing to Game Window!");
          Current_Window = Main_Game_Window;
          break;
        }
    }
    Main_Application->add_window(*Current_Window);
    Current_Window->show();
    Main_Application->release();
}
