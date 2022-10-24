#pragma once
#include<thread>
#include<memory>
#include<mutex>
#include<iostream>
#include</usr/include/gtkmm-3.0/gtkmm.h>

#include "map_generation_dialog.h"
#include "gtk_game_map.h"
#include "game.h"

class Map_Generation_Dialog;
class Gtk_Game_Map;
class Game;

using namespace std;

class Magic_Map_Generation_Thread_Communicator
{
  private:
    mutable mutex Main_Mutex;
    Map_Generation_Dialog* Generation_Dialog;
    shared_ptr<Gtk_Game_Map> Game_Map;
  public:
    Magic_Map_Generation_Thread_Communicator(Map_Generation_Dialog *g, shared_ptr<Gtk_Game_Map> m_g);
    void Do_Task(Game* m_g);
    void Notify_Dialog();
};
