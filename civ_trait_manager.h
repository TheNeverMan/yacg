#pragma once
#include<string>
#include<memory>
#include<vector>

#include "scaled_gtk_image.h"

using std::vector;
using std::string;
using std::make_shared;
using std::shared_ptr;

class Civ_Trait_Manager
{
  private:
    vector<shared_ptr<Scaled_Gtk_Image>> Trait_Icons;
  public:
    string Get_Trait_Full_Name(string trait_name);
    string Get_Trait_Full_Explanation(string trait_name);
    shared_ptr<Scaled_Gtk_Image> Get_Trait_Icon(string trait_name);
    string Get_Trait_Letter(string trait_name);
};
