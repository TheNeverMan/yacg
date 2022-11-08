#pragma once
#include<string>
#include<vector>

#include "gov.h"
#include "civ.h"
#include "themed_dialog.h"

using std::string;
using std::vector;

class Goverment_Dialog : public Themed_Dialog
{
  private:
    Civ Player;
    string Selected_Goverment;
  protected:
    void Goverment_Button_Clicked(Gov s_g);
  public:
    Goverment_Dialog(Civ p);
    string Get_Selected_Goverment();
};
