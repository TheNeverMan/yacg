#pragma once
#include<string>

#include "themed_dialog.h"
#include "unit.h"

using namespace std;

class Unit_Info_Dialog : public Themed_Dialog
{
  private:
    Unit Described_Unit;
    string Get_Trait_Info();
  public:
    Unit_Info_Dialog(Unit u);
};
