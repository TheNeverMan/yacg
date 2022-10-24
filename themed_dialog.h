#pragma once
#include<gtkmm.h>
#include<string>

#include "game_css_provider.h"
#include "logger.h"

using namespace std;

class Themed_Dialog : public Gtk::Dialog
{
  protected:
    Game_CSS_Provider Main_Provider;
  private:
    string message;
  public:
    Themed_Dialog(string m, string title);
    Themed_Dialog(string title);
    void Show();
};
