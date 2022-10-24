#pragma once
#include<gtkmm.h>
#include<string>
#include<memory>

#include "assets_path.h"
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
    template<typename Gtk_Object> void Add_CSS(shared_ptr<Gtk_Object> Object)
    {
      auto ctx = Object->get_style_context();
      ctx->add_provider(Main_Provider, 800);
    }
    template<typename Gtk_Object> void Add_CSS_With_Class(Gtk_Object *Object, string class_name)
    {
      auto ctx = Object->get_style_context();
      ctx->add_class(class_name);
      ctx->add_provider(Main_Provider, 800);
    }
};
