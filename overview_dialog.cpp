#include "overview_dialog.h"

Overview_Dialog::Overview_Dialog(vector<int> p_i, Civ p, string year) : Themed_Dialog("Goverment Report"), Player(p), Flag_Image(p.Get_Texture_Path(), 192, 96)
{
  Player_Income = p_i;
  Gtk::Box *Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Dialog_Root_Frame = Gtk::Frame("Statistical Yearbook for " + year);
  Stats_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL,2);
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
  Stats_Label = Gtk::Label(text);

  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Dialog_Root_Frame);
  Stats_Box.pack_start(*(Flag_Image.Get_Gtk_Image()));
  Dialog_Root_Frame.add(Stats_Box);
  Main_Provider.Add_CSS_With_Class(&Stats_Label, "big_label");
  Stats_Box.pack_start(Stats_Label);
}
