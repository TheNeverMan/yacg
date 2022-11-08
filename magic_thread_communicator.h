#pragma once
#include<thread>
#include<mutex>
#include<gtkmm.h>

#include "game.h"
#include "game_window.h"

class Game;
class Game_Window;

using std::thread;
using std::mutex;

class Magic_Thread_Communicator
{
  private:
    mutable mutex Main_Mutex;
    Game* Main_Game;
    Game_Window* Main_Window;
    int turn_outcome;
    bool has_finished;
  public:
    Magic_Thread_Communicator(Game *g, Game_Window *m_g);
    void Do_Task();
    void Notify_Window();
    void Set_Turn_Outcome(int t_o);
    int Get_Turn_Outcome();
    void Finish();
    bool Has_Finished();
};
