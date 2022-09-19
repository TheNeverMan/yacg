#pragma once
#include<string>
#include<vector>
#include<memory>
#include<filesystem>

#include "civ.h"
#include "gov.h"
#include "tile.h"
#include "unit.h"
#include "upgrade.h"
#include "tech.h"
#include "xml_serializable.h"
#include "logger.h"
#include "globals.h"

using namespace std;

class XML_Data_Loader
{
  private:
    string path_to_xml;
    vector<char> Load_File(string path);
    filesystem::directory_iterator Get_Files_In_Directory(string path);
    vector<Tile> Load_Tiles_From_File(string path);
    vector<Civ> Load_Civs_From_File(string path);
    vector<Gov> Load_Govs_From_File(string path);
    vector<Unit> Load_Units_From_File(string path);
    vector<Upgrade> Load_Upgrades_From_File(string path);
    vector<Tech> Load_Techs_From_File(string path);
    vector<string> Load_Traits_From_Root_Node(xml_node<>* Root_Node);
  public:
    XML_Data_Loader(string p_t_x);
    vector<Civ> Load_Civs();
    vector<Gov> Load_Govs();
    vector<Tile> Load_Tiles();
    vector<Unit> Load_Units();
    vector<Upgrade> Load_Upgrades();
    vector<Tech> Load_Techs();
};
