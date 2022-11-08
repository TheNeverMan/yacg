#pragma once
#include<fstream>
#include<iostream>
#include<string>

#include "game.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "logger.h"

using std::string;
using std::tuple;
using std::fstream;

class Game;

class Game_Saver
{
  private:
    Game *Game_To_Save;
  public:
    Game_Saver(Game *g_t_s);
    bool Save_Game(string path);
};
