#include "civs_dialog.h"

Civs_Dialog::Civs_Dialog(vector<Civ> p) : Themed_Dialog("Foregin Ministry")
{
  Players = p;
  Gtk::Box *Dialog_Box = get_content_area();
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Civilizations");
  auto *Dialog_Scrolled_Window = Gtk::make_managed<Gtk::ScrolledWindow>();
  Dialog_Scrolled_Window->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window->set_min_content_height(300);
  auto* Civs_List_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL,2);
  Dialog_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Dialog_Scrolled_Window);
  Dialog_Scrolled_Window->add(*Civs_List_Box);

  int index = 1;
  for(auto &player : Players)
  {
    auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto *image = Gtk::make_managed<Gtk::Image>();
    auto *Name_Label = Gtk::make_managed<Gtk::Label>(player.Get_Full_Name() + " (" + player.Get_Leader_Name() + ") " + " ID: " + to_string(index));
    auto *Points_Label = Gtk::make_managed<Gtk::Label>(" Points: " + to_string(player.Get_Score()));
    auto *Army_Label = Gtk::make_managed<Gtk::Label>(" Army Size: " + to_string(player.Get_Army_Manpower()));
    auto *Population_Label = Gtk::make_managed<Gtk::Label>(" Population: " + to_string(player.Get_Population()));
    auto *Capital_Label = Gtk::make_managed<Gtk::Label>(" Capital: " + player.Get_Capital_Name());

    image->set(player.Get_Texture_Path());
    box->pack_start(*image, Gtk::PACK_SHRINK);
    box->pack_start(*Name_Label);
    box->pack_start(*Points_Label);
    box->pack_start(*Army_Label);
    box->pack_start(*Population_Label);
    box->pack_start(*Capital_Label);
    Main_Provider.Add_CSS(box);
    Main_Provider.Add_CSS(image);
    Main_Provider.Add_CSS(Name_Label);
    Main_Provider.Add_CSS(Points_Label);
    Main_Provider.Add_CSS(Army_Label);
    Main_Provider.Add_CSS(Population_Label);
    Main_Provider.Add_CSS(Points_Label);
    Civs_List_Box->pack_start(*box);
    index++;
  }
}
