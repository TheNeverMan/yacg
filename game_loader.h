#pragma once
#include<fstream>
#include<iostream>
#include<string>
#include<tuple>

#include "game.h"
#include "logger.h"

using namespace std;

class Game;

class Game_Loader
{
  public:
    Game_Loader();
    tuple<bool, Game*> Load_Game(string path);
};
