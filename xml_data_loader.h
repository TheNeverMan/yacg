#pragma once
#include<string>
#include<vector>
#include<memory>

#include "civ.h"
#include "gov.h"
#include "tile.h"
#include "unit.h"
#include "upgrade.h"
#include "tech.h"
#include "xml_serializable.h"
#include "logger.h"

using namespace std;

class XML_Data_Loader
{
  private:
    string path_to_xml;
    vector<char> Load_File(string path);
  public:
    XML_Data_Loader(string p_t_x);
    vector<Civ> Load_Civs();
    vector<Gov> Load_Govs();
    vector<Tile> Load_Tiles();
    vector<Unit> Load_Units();
    vector<Upgrade> Load_Upgrades();
    vector<Tech> Load_Techs();
};
