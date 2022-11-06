#include "map_generation_dialog.h"

Map_Generation_Dialog::Map_Generation_Dialog() : Themed_Dialog("Please Wait..."), Explanation_Image("assets/textures/dialogs/map-generation-dialog-texture.svg", 64 ,64)
{
  Gtk::Box* Root_Box = get_content_area();
  Dialog_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Please Wait...\nLoading game...");
  Main_Provider.Add_CSS(Root_Box);
  Main_Provider.Add_CSS_With_Class(&Main_Progress_Bar, "big_label");
  Main_Progress_Bar = Gtk::ProgressBar();
  Main_Progress_Bar.set_show_text(false);
  Main_Progress_Bar.set_fraction(0.0);
  Root_Box->pack_start(Dialog_Box);
  Dialog_Box.pack_start(Explanation_Box);
  Dialog_Box.pack_start(Main_Progress_Bar);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  Main_Provider.Add_CSS_With_Class(&Explanation_Label, "big_label");
  Event_Dispatcher.connect(sigc::mem_fun(*this, &Map_Generation_Dialog::Pulse_Fraction));

}

void Map_Generation_Dialog::Pulse_Fraction()
{
  double fraction = Main_Progress_Bar.get_fraction() + 0.01;
  Main_Progress_Bar.set_fraction(fraction);
//  cout << fraction << endl;
  if(fraction >= 0.99)
  {
  //  cout << "close" << endl;
    close();
  }
}

void Map_Generation_Dialog::Notify()
{
  Event_Dispatcher.emit();
}
