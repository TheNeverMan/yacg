#include "tech_dialog.h"

Tech_Dialog::Tech_Dialog(Civ p) : Themed_Dialog("Science Ministry"), Player(p), Selected_Tech(*Player.Get_Currently_Researched_Tech())
{
  add_button("Apply", 0);
  Gtk::Box *Dialog_Box = get_content_area();
  auto* Root_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 2);
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Set Research Goal");
  auto* Research_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 2);
  vector<Tech> techs = Player.Get_Possible_Research_Techs();
  research_funds_percent = Player.Get_Research_Percent();
  Dialog_Box->pack_start(*Root_Box);
  Root_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Research_Box);
  for(auto &tech : techs)
  {
    auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto *image = Gtk::make_managed<Gtk::Image>(tech.Get_Texture_Path());
    auto *button = Gtk::make_managed<Gtk::Button>("Research " + tech.Get_Name());
    auto *label = Gtk::make_managed<Gtk::Label>(tech.Info(), Gtk::ALIGN_START, Gtk::ALIGN_FILL);
    Research_Box->pack_start(*box);
    box->pack_start(*image, Gtk::PACK_SHRINK);
    box->pack_start(*button, Gtk::PACK_SHRINK);
    box->pack_start(*label, Gtk::PACK_SHRINK);
    Main_Provider.Add_CSS(box);
    Main_Provider.Add_CSS(image);
    Main_Provider.Add_CSS(button);
    Main_Provider.Add_CSS(label);
    button->signal_clicked().connect(sigc::bind<Tech>(sigc::mem_fun(*this, &Tech_Dialog::Tech_Button_Clicked), tech ));
  }
  Glib::RefPtr<Gtk::Adjustment> Research_Adjustment;
  Research_Adjustment = Gtk::Adjustment::create((double) research_funds_percent ,0.0,100.0,1.0,10,0.0);
  auto* Research_Percent_Switch = Gtk::make_managed<Gtk::SpinButton>(Research_Adjustment);
  auto* info = Gtk::make_managed<Gtk::Label>("Set Research Funds Percent:");
  Root_Box->pack_start(*info);
  Root_Box->pack_start(*Research_Percent_Switch);
  Research_Percent_Switch->signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Tech_Dialog::Research_Percent_Switch_Changed), Research_Percent_Switch ));
}

void Tech_Dialog::Tech_Button_Clicked(Tech t)
{
  Selected_Tech = t;
}

void Tech_Dialog::Research_Percent_Switch_Changed(Gtk::SpinButton* Switch)
{
  research_funds_percent = (int) Switch->get_value();
}

int Tech_Dialog::Get_Research_Percent()
{
  return research_funds_percent;
}

Tech Tech_Dialog::Get_Selected_Tech()
{
  return Selected_Tech;
}
