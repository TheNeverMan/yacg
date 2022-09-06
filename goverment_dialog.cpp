#include "goverment_dialog.h"

Goverment_Dialog::Goverment_Dialog(Civ p) : Themed_Dialog("Change Goverment"), Player(p), Selected_Goverment(Player.Get_Active_Goverment_Name())
{
  Gtk::Box *Dialog_Box = get_content_area();
  add_button("Revolt!", 0);
  auto* Root_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 2);
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Unlocked Goverments");
  auto* Gov_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL,2);
  vector<Gov> govs = Player.Get_Possible_Goverments();
  Dialog_Box->pack_start(*Root_Box);
  Root_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Gov_Box);
  if(govs.size() == 1)
  {
    auto *label = Gtk::make_managed<Gtk::Label>("Research technologies to get access to different goverments");
    Main_Provider.Add_CSS(label);
    Gov_Box->pack_start(*label);
  }
  for(auto &gov : govs)
  {
    if(gov.Get_Name() != Player.Get_Active_Goverment_Name())
    {
      auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
      auto *image = Gtk::make_managed<Gtk::Image>(gov.Get_Texture_Path());
      auto *button = Gtk::make_managed<Gtk::Button>("Revolt to " + gov.Get_Name());
      Gov_Box->pack_start(*box);
      box->pack_start(*image, Gtk::PACK_SHRINK);
      box->pack_start(*button, Gtk::PACK_SHRINK);
      auto *label = Gtk::make_managed<Gtk::Label>(gov.Info(), Gtk::ALIGN_START, Gtk::ALIGN_FILL);
      box->pack_start(*label);
      Main_Provider.Add_CSS(box);
      Main_Provider.Add_CSS(image);
      Main_Provider.Add_CSS(button);
      Main_Provider.Add_CSS(label);
      button->signal_clicked().connect(sigc::bind<Gov>(sigc::mem_fun(*this, &Goverment_Dialog::Goverment_Button_Clicked), gov ));
    }
  }
}

void Goverment_Dialog::Goverment_Button_Clicked(Gov s_g)
{
  Selected_Goverment = s_g.Get_Name();
}

string Goverment_Dialog::Get_Selected_Goverment()
{
  return Selected_Goverment;
}
