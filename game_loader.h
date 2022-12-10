#pragma once
#include<fstream>
#include<string>
#include<tuple>

#include "game.h"
#include "logger.h"

using std::string;
using std::tuple;
using std::make_tuple;
using std::get;
using std::fstream;

class Game;

class Game_Loader
{
  public:
    Game_Loader();
    tuple<bool, Game*> Load_Game(string path);
};
