#pragma once
#include<gtkmm.h>
#include<string>

#include "game_css_provider.h"
#include "apply_button.h"
#include "logger.h"

using namespace std;

class Themed_Dialog : public Gtk::Dialog
{
  protected:
    Game_CSS_Provider Main_Provider;
    Apply_Button Close_Button;
    void Close_Button_Clicked();
  private:
    string message;
    void Add_Close_Button();
  public:
    Themed_Dialog(string m, string title);
    Themed_Dialog(string title);
    void Show();
};
