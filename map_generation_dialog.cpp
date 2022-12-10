#include "map_generation_dialog.h"

Map_Generation_Dialog::Map_Generation_Dialog() : Themed_Dialog("Please Wait..."), Explanation_Image("assets/textures/dialogs/map-generation-dialog-texture.svg", 64 ,64)
{
  Gtk::Box* Root_Box = get_content_area();
  Dialog_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Text_Tip_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Fraction_Bar_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Please Wait...\nLoading game...");
  Tip_Label = Gtk::Label(Random_Tip_Manager.Get_Random_Tip());
  Fraction_Label = Gtk::Label("0.0");
  Main_Provider.Add_CSS(&Tip_Label);
  Main_Provider.Add_CSS(Root_Box);
  Main_Provider.Add_CSS_With_Class(&Main_Progress_Bar, "big_label");
  Main_Progress_Bar = Gtk::ProgressBar();
  Main_Progress_Bar.set_show_text(false);
  Main_Progress_Bar.set_fraction(0.0);
  Root_Box->pack_start(Dialog_Box);
  Dialog_Box.pack_start(Explanation_Box);
  Dialog_Box.pack_start(Fraction_Bar_Box);
  Fraction_Bar_Box.pack_start(Main_Progress_Bar);
  Fraction_Bar_Box.pack_start(Fraction_Label);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Text_Tip_Box);
  Text_Tip_Box.pack_start(Explanation_Label);
  Text_Tip_Box.pack_start(Tip_Label);
  Main_Provider.Add_CSS_With_Class(&Explanation_Label, "big_label");
  Event_Dispatcher.connect(sigc::mem_fun(*this, &Map_Generation_Dialog::Pulse_Fraction));
  Close_Button.set_sensitive(false);
}

void Map_Generation_Dialog::Pulse_Fraction()
{
  double fraction = Main_Progress_Bar.get_fraction() + 0.01;
  string fraction_text = to_string(static_cast<int>(fraction*100));
  fraction_text = fraction_text + " %";
  Fraction_Label.set_text(fraction_text.c_str());
  Main_Progress_Bar.set_fraction(fraction);
//  cout << fraction << endl;
  if(fraction >= 0.99)
  {
    Close_Button.set_sensitive(true);
    close();
  }
}

void Map_Generation_Dialog::Notify()
{
  Event_Dispatcher.emit();
}
