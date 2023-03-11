#include "civs_dialog.h"

Civs_Dialog::Civs_Dialog(vector<Civ>& p) : Themed_Dialog("Foregin Ministry"), Explanation_Image(string(assets_directory_path) + "textures/dialogs/civs-dialog-texture.svg", 70, 70), Players(p)
{
  Gtk::Box *Dialog_Box = get_content_area();
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Here is information about every civilization currently in the game. \n Each civilization has score in points which measures its overall performance.");
  Dialog_Root_Frame = Gtk::Frame("Civilizations");
  Dialog_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window.set_min_content_height(300);
  Dialog_Box->pack_start(Explanation_Box);
  Hide_Dead_Button = Gtk::CheckButton("Hide eliminated players");
  Hide_Dead_Button.set_active(true);
  Explanation_Box.pack_start((Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  //Dialog_Box->pack_start(Hide_Dead_Button);
  Dialog_Box->pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Dialog_Scrolled_Window);
  Dialog_Scrolled_Window.add(Civs_List_Box);
  rows = 0;
  Hide_Dead_Button.signal_toggled().connect(sigc::mem_fun(*this, &Civs_Dialog::Update_Players));
  Update_Players();
}

void Civs_Dialog::Update_Players()
{
  Logger::Log_Info("Updating Players...");
  /*if(rows != 0)
  {
    int start = 0;
    while(start <= rows)
    {
      Civs_List_Box.remove_row(start);
      start++;
    }
    rows = 0;
  }*/
  int index = 1;
  for(auto &player : Players)
  {
    if(Hide_Dead_Button.get_active() && player.Get_Capital_Name() == "exile")
    {
      Logger::Log_Info("Player " + string(player.Get_Name()) + " is dead, skipping...");
      continue;
    }
    auto *image = Gtk::make_managed<Gtk::Image>();
    Scaled_Pixbuf tmp(player.Get_Texture_Path(), 96, 48);
    auto *color_image = Gtk::make_managed<Gtk::Image>();
    Glib::RefPtr<Gdk::Pixbuf> color_pix;
    color_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 48, 48);
    color_pix->fill(player.Get_Civ_Color());
    color_image->set(color_pix);
    color_image->set_margin_bottom(3);
    auto *Name_Label = Gtk::make_managed<Gtk::Label>(string(player.Get_Full_Name()) + " (" + string(player.Get_Leader_Name()) + ") " + " ID: " + to_string(index));
    auto *Points_Label = Gtk::make_managed<Gtk::Label>(" Points: " + to_string(player.Get_Score()));
    auto *Army_Label = Gtk::make_managed<Gtk::Label>(" Army Size: " + to_string(player.Get_Army_Manpower()));
    auto *Population_Label = Gtk::make_managed<Gtk::Label>(" Population: " + to_string(player.Get_Population()));
    auto *Capital_Label = Gtk::make_managed<Gtk::Label>(" Capital: " + string(player.Get_Capital_Name()));
    rows++;
    image->set(tmp.Get_Pixbuf());
    image->set_margin_end(3);
    Civs_List_Box.attach(*image, 0,index-1);
    Civs_List_Box.attach(*color_image, 1,index-1);
    Civs_List_Box.attach(*Name_Label, 2,index-1);
    Civs_List_Box.attach(*Points_Label, 3,index-1);
    Civs_List_Box.attach(*Army_Label, 4,index-1);
    Civs_List_Box.attach(*Population_Label, 5,index-1);
    Civs_List_Box.attach(*Capital_Label, 6,index-1);
    Main_Provider.Add_CSS(image);
    Main_Provider.Add_CSS(Name_Label);
    Main_Provider.Add_CSS(Points_Label);
    Main_Provider.Add_CSS(Army_Label);
    Main_Provider.Add_CSS(Population_Label);
    Main_Provider.Add_CSS(Points_Label);
    index++;
  }
}
