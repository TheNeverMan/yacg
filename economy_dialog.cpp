#include "economy_dialog.h"

Economy_Dialog::Economy_Dialog(vector<int> p_i, Civ p) : Themed_Dialog("Finance Ministry"), Player(p), Info_Image(assets_directory_path + "textures/dialogs/economy-dialog-texture.svg", 64, 64)
{
  Player_Income = p_i;
  Gtk::Box *Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Image_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Dialog_Root_Frame = Gtk::Frame("Economy of " + Player.Get_Name());
  Finance_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Explanation_Label = Gtk::Label("Every turn some of your upgrades produce gold, \n then some of it gets consumed by your units and upgrades, \n finally precentage of (set in research funds) goes to research, \n rest is added to your budget.");
  string text;
  text = " Brutto Income " + to_string(Player_Income[0]);
  text = text + "\n Tile Maitenance -" + to_string(Player_Income[1]);
  text = text + "\n Unit Maitenance -" + to_string(Player.Get_Unit_Maitenance());
  text = text + "\n Research Funds -" + to_string(static_cast<int>((Player_Income[0] - Player_Income[1] - Player.Get_Unit_Maitenance()) * ((double) Player.Get_Research_Percent() / 100.0 )));
  int netto_income = Player_Income[0] - (Player_Income[1] + Player.Get_Unit_Maitenance() + static_cast<int>((Player_Income[0] - Player_Income[1] - Player.Get_Unit_Maitenance()) * ((double) Player.Get_Research_Percent() / 100.0 )));
  text = text + "\n Netto Income " + to_string(netto_income);
  Finance_Label = Gtk::Label(text);
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Image_Box);
  Image_Box.pack_start(*(Info_Image.Get_Gtk_Image()));
  Image_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Finance_Box);
  Main_Provider.Add_CSS_With_Class(&Finance_Label, "big_label");
  Finance_Box.pack_start(Finance_Label);
  Root_Box.pack_start(Explanation_Label);
}
