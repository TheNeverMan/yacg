#pragma once
#include<string>
#include<vector>
#include<sstream>
#include<fstream>

#include "tutorial_dialog.h"
#include "game_css_provider.h"
#include "assets_path.h"

using std::string;
using std::fstream;

class Help_Manager
{
  private:
    string tutorial_text_path;
    string tutorial_image_path;
  public:
    void Show_Basic_Tutorial();
    void Show_All_Traits_Dialog();
    void Show_Keybindings_Dialog();
    void Show_Advanced_Tutorial();
    void Show_Game_Help();
    Help_Manager();
};
