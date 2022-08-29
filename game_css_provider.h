#pragma once
#include<gtkmm.h>
#include<string>

#include "globals.h"
#include "logger.h"

using namespace std;

class Game_CSS_Provider
{
  private:
    Glib::RefPtr<Gtk::CssProvider> Main_Provider;
  public:
    Game_CSS_Provider();
    template<typename Gtk_Object> void Add_CSS(Gtk_Object *Object)
    {
      auto ctx = Object->get_style_context();
      ctx->add_provider(Main_Provider, 800);
    }
};
