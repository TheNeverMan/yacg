#pragma once
#include<map>
#include<memory>
#include <cctype>

#include "xml_data_loader.h"
#include "themed_dialog.h"
#include "scaled_gtk_image.h"

using std::map;
using std::tolower;
using std::shared_ptr;
using std::make_shared;
using std::transform;

class Trait_Dialog : public Themed_Dialog
{
  private:
    ::map<int, string> Trait_To_Column;
    vector<Civ> Civs;
    string Get_Trait_Full_Name(string trait_name);
    void Get_Trait_Icon(string full_trait_name);
    Gtk::Box Root_Box;
    Gtk::Box Explanation_Box;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Label Explanation_Label;
    Gtk::Grid Civs_Grid;
    int column_index = 1;
    int Get_Column_For_Trait(string trait_name);
    vector<shared_ptr<Scaled_Gtk_Image>> Trait_Images;
  public:
    Trait_Dialog();
};
