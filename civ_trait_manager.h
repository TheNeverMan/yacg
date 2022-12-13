#pragma once
#include<string>
#include<memory>
#include<map>
#include<array>
#include<vector>

#include "scaled_gtk_image.h"
#include "logger.h"

using std::vector;
using std::string;
using std::make_shared;
using std::map;
using std::array;
using std::shared_ptr;

class Civ_Trait_Manager
{
  private:
    vector<shared_ptr<Scaled_Gtk_Image>> Trait_Icons;
    map<string, array<string, 2>> Traits;
  public:
    string Get_Trait_Full_Name(string trait_name);
    string Get_Trait_Full_Explanation(string trait_name);
    shared_ptr<Scaled_Gtk_Image> Get_Trait_Icon(string trait_name);
    string Get_Trait_Letter(string trait_name);
    Civ_Trait_Manager();
};
