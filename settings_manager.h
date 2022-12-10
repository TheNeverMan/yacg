#pragma once
#include<string>
#include<fstream>
#include<vector>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "logger.h"
#include "xml_loader.h"

using namespace rapidxml;
using std::string;
using std::fstream;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::to_string;
using std::istreambuf_iterator;
using std::stoi;

class Settings_Manager
{
  private:
    bool autosave;
    int tile_size;
    string path_to_file;
    bool autoresize;
    bool mute;
    bool mute_music;
    bool show_random_tip_on_startup;
    void Load_Data_From_XML();
    void Set_To_Default_Values();
  public:
    Settings_Manager(string p_t_f);
    Settings_Manager();
    bool Get_Autosave_Value();
    bool Is_Game_Muted();
    void Set_Mute_Value(bool m);
    bool Get_Autoresize_Tiles_Value();
    bool Get_Random_Tip_On_Startup_Value();
    void Set_Random_Tip_On_Startup_Value(bool s);
    void Set_Autoresize_Tiles_Value(bool a);
    bool Check_If_Game_Is_Launched_First_Time();
    void Set_Music_Mute_Value(bool m);
    bool Is_Music_Muted();
    void Launch_Game_First_Time();
    int Get_Tile_Size_Value();
    void Set_Autosave_Value(bool a);
    void Set_Tile_Size_Value(int t_s);
    void Write_To_File();
    void Reload_Settings();
};
