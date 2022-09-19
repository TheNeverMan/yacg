#include "economy_dialog.h"

Economy_Dialog::Economy_Dialog(vector<int> p_i, Civ p) : Themed_Dialog("Finance Ministry"), Player(p)
{
  Player_Income = p_i;
  Gtk::Box *Dialog_Box = get_content_area();
  auto* Root_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 2);
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Economy of " + Player.Get_Name());
  auto* Finance_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL,2);
  string text;
  text = " Brutto Income " + to_string(Player_Income[0]);
  text = text + "\n Tile Maitenance -" + to_string(Player_Income[1]);
  text = text + "\n Unit Maitenance -" + to_string(Player.Get_Unit_Maitenance());
  text = text + "\n Research Funds -" + to_string(static_cast<int>((Player_Income[0] - Player_Income[1] - Player.Get_Unit_Maitenance()) * ((double) Player.Get_Research_Percent() / 100.0 )));
  auto* Finance_Label = Gtk::make_managed<Gtk::Label>(text);
  Dialog_Box->pack_start(*Root_Box);
  Root_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Finance_Box);
  Finance_Box->pack_start(*Finance_Label);
}
