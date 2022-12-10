#pragma once
#include<gtkmm.h>
#include<memory>
#include<string>

#include "game_css_provider.h"
#include "apply_button.h"
#include "logger.h"
#include "image_path.h"
#include "scaled_gtk_image.h"

using std::string;
using std::make_shared;
using std::shared_ptr;

class Themed_Dialog : public Gtk::Dialog
{
  protected:
    Game_CSS_Provider Main_Provider;
    Apply_Button Close_Button;
    void Close_Button_Clicked();
  private:
    shared_ptr<Scaled_Gtk_Image> Icon_Image;
    string message;
    void Add_Close_Button();
  public:
    Themed_Dialog(string m, string title);
    Themed_Dialog(string title);
    void Show();
};
