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
    auto *label = Gtk::make_managed<Gtk::Label>(player.Get_Full_Name() + " (" + player.Get_Leader_Name() + ") " + " ID: " + to_string(index) + " Points: " + to_string(player.Get_Score()) + " Army Size: " + to_string(player.Get_Army_Manpower()) + " Population: " + to_string(player.Get_Population()));
    Glib::RefPtr<Gdk::Pixbuf> color_pix;
    color_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 32, 32);
    color_pix->fill(player.Get_Civ_Color());
    image->set(color_pix);
    box->pack_start(*image, Gtk::PACK_SHRINK);
    box->pack_start(*label);
    Main_Provider.Add_CSS(box);
    Main_Provider.Add_CSS(image);
    Main_Provider.Add_CSS(label);
    Civs_List_Box->pack_start(*box);
    index++;
  }
}
