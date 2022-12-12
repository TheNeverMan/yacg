#pragma once
#include<map>
#include<memory>
#include <cctype>

#include "xml_data_loader.h"
#include "themed_dialog.h"
#include "scaled_gtk_image.h"
#include "civ_trait_manager.h"

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
    Gtk::Box Root_Box;
    Gtk::Box Explanation_Box;
    Gtk::Box Grid_Box;
    Gtk::Frame All_Traits_Frame;
    Gtk::Box All_Traits_Box;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Label Explanation_Label;
    Gtk::Grid Civs_Grid;
    int column_index = 1;
    int Get_Column_For_Trait(string ntrait_name);
    vector<shared_ptr<Scaled_Gtk_Image>> Trait_Images;
    Civ_Trait_Manager Trait_Manager;
  public:
    Trait_Dialog();
};
