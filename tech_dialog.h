#pragma once
#include<string>
#include<vector>

#include "themed_dialog.h"
#include "civ.h"
#include "tech.h"

using namespace std;

class Tech_Dialog : public Themed_Dialog
{
  private:
    Civ Player;
    Tech Selected_Tech;
    int research_funds_percent;
  protected:
    void Tech_Button_Clicked(Tech t);
    void Research_Percent_Switch_Changed(Gtk::SpinButton* Switch);
  public:
    Tech_Dialog(Civ p);
    int Get_Research_Percent();
    Tech Get_Selected_Tech();
};
