#include "game_css_provider.h"

Game_CSS_Provider::Game_CSS_Provider()
{
  Logger::Log_Info("Loading CSS...");
  Main_Provider = Gtk::CssProvider::create();
  Main_Provider->load_from_path(assets_directory_path + "style.css");
}
