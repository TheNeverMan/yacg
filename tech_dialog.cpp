#include "tech_dialog.h"

Tech_Dialog::Tech_Dialog(Civ p) : Themed_Dialog("Science Ministry"), Player(p), Selected_Tech(*Player.Get_Currently_Researched_Tech()), Explanation_Image(assets_directory_path + "textures/dialogs/tech-dialog-texture.svg", 64, 64)
{
  Gtk::Box *Dialog_Box = get_content_area();
  Close_Button.set_label("Apply");
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Here you can set technology that is currently researched.\n Your empire can research one technology at a time. \n Every turn given percent of your income is substracted from researched technology cost.\n When researched technology cost hits 0 it is researched and gives access to other technologies. \n You can read here short descriptions of technologies that are currently \n possible to be researched.");
  Dialog_Root_Frame = Gtk::Frame("Set Research Goal");
  vector<Tech> techs = Player.Get_Possible_Research_Techs();
  research_funds_percent = Player.Get_Research_Percent();
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Explanation_Box);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  Root_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Research_Box);
  Selected_Tech = *p.Get_Currently_Researched_Tech();
  int index = 0;
  for(auto &tech : techs)
  {
    shared_ptr<Scaled_Gtk_Image> Tech_Image = make_shared<Scaled_Gtk_Image>(tech.Get_Texture_Path(), 64, 64);
    Tech_Images.push_back(Tech_Image);
    shared_ptr<Sound_Button> button = make_shared<Sound_Button>("Research " + tech.Get_Name());
    button->Change_Icon("assets/textures/icons/science-icon.svg.png");
    button->set_margin_bottom(3);
    auto *label = Gtk::make_managed<Gtk::Label>(tech.Info(), Gtk::ALIGN_START, Gtk::ALIGN_FILL);
    Research_Box.attach(*(Tech_Image->Get_Gtk_Image()), 0, index);
    Research_Box.attach(*button, 1, index);
    Research_Box.attach(*label, 2, index);
    Main_Provider.Add_CSS(button);
    Main_Provider.Add_CSS_With_Class(label, "big_label");
    tuple<string, shared_ptr<Sound_Button>> tmp (tech.Get_Name(), button);
    Tech_Buttons.push_back(tmp);
    index++;
    button->signal_clicked().connect(sigc::bind<Tech>(sigc::mem_fun(*this, &Tech_Dialog::Tech_Button_Clicked), tech ));
  }
  Glib::RefPtr<Gtk::Adjustment> Research_Adjustment;
  Research_Adjustment = Gtk::Adjustment::create((double) research_funds_percent ,0.0,100.0,1.0,10,0.0);
  Research_Percent_Switch = Gtk::SpinButton(Research_Adjustment);
  Research_Percent_Info = Gtk::Label("Set Research Funds Percent:");
  Root_Box.pack_start(Research_Percent_Info);
  Main_Provider.Add_CSS(&Research_Percent_Switch);
  Root_Box.pack_start(Research_Percent_Switch);
  Set_Currently_Researched_Button();
  Research_Percent_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Tech_Dialog::Research_Percent_Switch_Changed));
}

void Tech_Dialog::Tech_Button_Clicked(Tech t)
{
  Selected_Tech = t;
  Set_Currently_Researched_Button();
}

void Tech_Dialog::Set_Currently_Researched_Button()
{
  for_each(Tech_Buttons.begin(), Tech_Buttons.end(), [](tuple<string, shared_ptr<Gtk::Button>> val){get<1>(val)->set_sensitive(true);});
  for(auto& Tup : Tech_Buttons)
  {
    if(Selected_Tech.Get_Name() == get<0>(Tup))
      get<1>(Tup)->set_sensitive(false);
  }
}

void Tech_Dialog::Research_Percent_Switch_Changed()
{
  research_funds_percent = (int) Research_Percent_Switch.get_value();
}

int Tech_Dialog::Get_Research_Percent()
{
  return research_funds_percent;
}

Tech Tech_Dialog::Get_Selected_Tech()
{
  return Selected_Tech;
}

Tech_Dialog::~Tech_Dialog()
{
  Tech_Buttons.clear();
}
