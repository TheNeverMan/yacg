#pragma once
#include<string>
#include<fstream>
#include<vector>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "logger.h"

using namespace std;
using namespace rapidxml;

class Settings_Manager
{
  private:
    bool autosave;
    int tile_size;
    string path_to_file;
    void Load_Data_From_XML();
  public:
    Settings_Manager(string p_t_f);
    Settings_Manager();
    bool Get_Autosave_Value();
    int Get_Tile_Size_Value();
    void Set_Autosave_Value(bool a);
    void Set_Tile_Size_Value(int t_s);
    void Write_To_File();
};
