#include "trait_dialog.h"

int Trait_Dialog::Get_Column_For_Trait(string ntrait_name)
{
  for (const auto& [column, trait_name] : Trait_To_Column)
  {
    if(Trait_Manager.Get_Trait_Full_Name(ntrait_name) == trait_name)
      return column;
  }
  return column_index + 1;
}

Trait_Dialog::Trait_Dialog() : Themed_Dialog("All Civilizations", "Ok"), Explanation_Image("assets/textures/dialogs/trait-dialog-texture.svg", 64, 64)
{
  XML_Data_Loader Main_Loader(" ");
  Civs = Main_Loader.Load_Civs();
  for (auto& Civ : Civs)
  {
    for (auto& Trait : Civ.Get_Trait_Names())
    {
      bool contains = false;
      for (auto& [column, trait_name] : Trait_To_Column)
      {
        if(trait_name == Trait_Manager.Get_Trait_Full_Name(Trait))
          contains = true;
      }
      if(!contains)
        Trait_To_Column[column_index++] = Trait_Manager.Get_Trait_Full_Name(Trait);
    }
  }

  Gtk::Box* Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Grid_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  All_Traits_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  All_Traits_Frame = Gtk::Frame("Traits Legend");
  Explanation_Label = Gtk::Label("This is a table of all civilizations in the game, sorted by their trait combinations:");
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Explanation_Box);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  Root_Box.pack_start(Grid_Box);
  Grid_Box.pack_start(Civs_Grid);
  Grid_Box.pack_start(All_Traits_Frame, Gtk::PACK_SHRINK);
  All_Traits_Frame.add(All_Traits_Box);
  for (const auto& [column, trait_name] : Trait_To_Column)
  {
    auto* Trait_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto* Trait_Label = Gtk::make_managed<Gtk::Label>(trait_name);
    Main_Provider.Add_CSS_With_Class(Trait_Label, "medium_label");
    auto Trait_Image = Trait_Manager.Get_Trait_Icon(trait_name);
    Trait_Box->pack_start(*(Trait_Image->Get_Gtk_Image()));
    Trait_Box->pack_start(*Trait_Label);
    Civs_Grid.attach(*Trait_Box, column, 0);
  }
  for (const auto& [column, trait_name] : Trait_To_Column)
  {
    auto* Trait_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto* Trait_Label = Gtk::make_managed<Gtk::Label>(trait_name);
    auto Trait_Image = Trait_Manager.Get_Trait_Icon(trait_name);
    Main_Provider.Add_CSS_With_Class(Trait_Label, "medium_label");
    Trait_Box->pack_start(*(Trait_Image->Get_Gtk_Image()));
    Trait_Box->pack_start(*Trait_Label);
    Civs_Grid.attach(*Trait_Box, 0, column);
  }
  {
    auto* Trait_Label = Gtk::make_managed<Gtk::Label>("None");
    Main_Provider.Add_CSS_With_Class(Trait_Label, "medium_label");
    Civs_Grid.attach(*Trait_Label, column_index+1, 0);
  }
  {
    auto* Trait_Label = Gtk::make_managed<Gtk::Label>("None");
    Main_Provider.Add_CSS_With_Class(Trait_Label, "medium_label");
    Civs_Grid.attach(*Trait_Label, 0, column_index+1);
  }
  for(auto& Civ : Civs)
  {
    int col = 0;
    int row = 0;
    vector<string> Traits = Civ.Get_Trait_Names();
    if(Traits.size() == 1)
    {
      col = Get_Column_For_Trait(Traits[0]);
      row = column_index + 1;
    }
    else
    {
      col = Get_Column_For_Trait(Traits[0]);
      row = Get_Column_For_Trait(Traits[1]);
    }
    if(col > row)
    {
      int tmp = col;
      col = row;
      row = tmp;
    }
    auto* Civ_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    shared_ptr<Scaled_Gtk_Image> Civ_Icon = make_shared<Scaled_Gtk_Image>(Civ.Get_Texture_Path(), 48, 24);
    Trait_Images.push_back(Civ_Icon);
    auto* Civ_Label = Gtk::make_managed<Gtk::Label>(Civ.Get_Name());
    Main_Provider.Add_CSS_With_Class(Civ_Label, "medium_label");
    Civ_Box->pack_start(*(Civ_Icon->Get_Gtk_Image()));
    Civ_Box->pack_start(*Civ_Label);
    Civs_Grid.attach(*Civ_Box, col, row);
    auto* Reverse_Label = Gtk::make_managed<Gtk::Label>(" --- ");
    Main_Provider.Add_CSS_With_Class(Reverse_Label, "medium_label");
    if(col != row)
    Civs_Grid.attach(*Reverse_Label, row, col);
  }
  for(const auto& [column, trait_name] : Trait_To_Column)
  {
    auto* Trait_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto* Trait_Label = Gtk::make_managed<Gtk::Label>(trait_name + "-" + Trait_Manager.Get_Trait_Full_Explanation(Trait_Manager.Get_Trait_Letter(trait_name)));
    auto Trait_Icon = Trait_Manager.Get_Trait_Icon(trait_name);
    Main_Provider.Add_CSS_With_Class(Trait_Label, "medium_label");
    All_Traits_Box.pack_start(*Trait_Box, Gtk::PACK_SHRINK);
    Trait_Box->pack_start(*(Trait_Icon->Get_Gtk_Image()), Gtk::PACK_SHRINK);
    Trait_Box->pack_start(*Trait_Label, Gtk::PACK_SHRINK);
  }
}
