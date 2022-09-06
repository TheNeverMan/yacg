#pragma once
#include<gtkmm.h>
#include<vector>
#include<string>

#include "themed_dialog.h"
#include "civ.h"

using namespace std;

class Civs_Dialog : public Themed_Dialog
{
  private:
    vector<Civ> Players;
  public:
    Civs_Dialog(vector<Civ> p);
};
