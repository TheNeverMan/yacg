#pragma once
#include<string>
#include<vector>

#include "civ.h"
#include "themed_dialog.h"

using namespace std;

class Overview_Dialog : public Themed_Dialog
{
  private:
    vector<int> Player_Income;
    Civ Player;
  public:
    Overview_Dialog(vector<int> p_i, Civ p, string year);
};
