#include "overview_dialog.h"

Overview_Dialog::Overview_Dialog(vector<int> p_i, Civ p, string year) : Themed_Dialog("Goverment Report"), Player(p)
{
  Player_Income = p_i;
  Gtk::Box *Dialog_Box = get_content_area();
  auto* Root_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 2);
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Statistical Yearbook for " + year);
  auto* Stats_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL,2);
  string text;
  text = Player.Get_Full_Name();
  text = text + "\n Leader: " + Player.Get_Leader_Name();
  text = text + "\n Capital: " + Player.Get_Capital_Name();
  text = text + "\n Goverment: " + Player.Get_Active_Goverment_Name();
  text = text + "\n Population: " + to_string(Player.Get_Population());
  text = text + "\n Army Size: " + to_string(Player.Get_Army_Manpower());
  double gnppc = Player_Income[0] * 1000000;
  gnppc = gnppc / (double) Player.Get_Population();
  text = text + "\n GNP per capita " + to_string(static_cast<int>(gnppc)) + " $";
  auto* Stats_Label = Gtk::make_managed<Gtk::Label>(text);

  Dialog_Box->pack_start(*Root_Box);
  Root_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Stats_Box);
  Stats_Box->pack_start(*Stats_Label);
}
