#pragma once
#include<vector>
#include<string>

#include "themed_dialog.h"
#include "civ.h"

using namespace std;

class Economy_Dialog : public Themed_Dialog
{
  private:
    vector<int> Player_Income;
    Civ Player;
  public:
    Economy_Dialog(vector<int> p_i, Civ p);
};
