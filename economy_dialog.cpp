#include "economy_dialog.h"

Economy_Dialog::Economy_Dialog(vector<int> p_i, Civ p) : Themed_Dialog("Finance Ministry"), Player(p)
{
  Player_Income = p_i;
  Gtk::Box *Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Image_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Dialog_Root_Frame = Gtk::Frame("Economy of " + Player.Get_Name());
  Finance_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Glib::RefPtr<Gdk::Pixbuf> image_pix;
  Glib::RefPtr<Gdk::Pixbuf> finished_pix;
  finished_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 64, 64);
  Image_Path Path("assets/textures/dialogs/economy-dialog-texture.png");
  image_pix = Gdk::Pixbuf::create_from_file(Path.Get_File_Path());
  image_pix->scale(finished_pix, 0, 0, 64, 64, 0, 0, 2, 2, Gdk::INTERP_BILINEAR);
  Info_Image = Gtk::Image(finished_pix);
  string text;
  text = " Brutto Income " + to_string(Player_Income[0]);
  text = text + "\n Tile Maitenance -" + to_string(Player_Income[1]);
  text = text + "\n Unit Maitenance -" + to_string(Player.Get_Unit_Maitenance());
  text = text + "\n Research Funds -" + to_string(static_cast<int>((Player_Income[0] - Player_Income[1] - Player.Get_Unit_Maitenance()) * ((double) Player.Get_Research_Percent() / 100.0 )));
  Finance_Label = Gtk::Label(text);
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Image_Box);
  Image_Box.pack_start(Info_Image);
  Image_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Finance_Box);
  Finance_Box.pack_start(Finance_Label);
}
