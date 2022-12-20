#include "internal_dialog.h"

Internal_Dialog::Internal_Dialog(Gov a_g, vector<City>* c) : Themed_Dialog("Internal Ministry"), Active_Goverment(a_g), Explanation_Image(assets_directory_path + "textures/dialogs/internal-dialog-texture.svg", 64, 64)
{
  Cities = c;
  auto* Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Here you can check stability of all your cities. \n Low stability means revolts and increased city maitenance. \n Keep your stability high by connecting cities with roads and placing military \n units in cities.");
  Goverment_Label = Gtk::Label("Goverment: " + Active_Goverment.Get_Name() + "\n Maximum City Stability: " + to_string(Active_Goverment.Get_Max_Stability()) + "\n Stability in every city after each turn increases by " + to_string(Active_Goverment.Get_Passive_Stability()) + " points. \n Putting military unit in city gives additional " + to_string(Active_Goverment.Get_Army_Stability()) + " points per turn.");
  Cities_Frame = Gtk::Frame("List of Cities");
  Cities_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Cities_Window.set_min_content_height(300);
  Cities_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  for(auto& City : *Cities)
  {
    auto* City_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    string state = City.Get_Status();
    string icon_path = "assets/textures/icons/";
    if(state == "Fine")
      icon_path = icon_path + "apply-icon.svg.png";
    if(state == "Drought")
      icon_path = icon_path + "drought-icon.svg.png";
    if(state == "Flood")
      icon_path = icon_path + "flood-icon.svg.png";
    if(state == "Epidemy")
      icon_path = icon_path + "epidemy-icon.svg.png";
    if(state == "Anarchy")
      icon_path = icon_path + "anarchy-icon.svg.png";
    if(state == "Riots")
      icon_path = icon_path + "riots-icon.svg.png";
    shared_ptr<Scaled_Gtk_Image> City_Icon = make_shared<Scaled_Gtk_Image>(icon_path, 24, 24);
    Cities_Icons.push_back(City_Icon);
    string city_name = City.Get_Name();
    if(City.Is_Connected())
      city_name = city_name + " (Connected To Other Cities)";
    auto* City_Name_Label = Gtk::make_managed<Gtk::Label>(city)name);
    if(state == "Drought")
      state = "<span foreground=\"yellow\">" + state + "</span>";
    if(state == "Flood")
      state = "<span foreground=\"yellow\">" + state + "</span>";
    if(state == "Epidemy")
      state = "<span foreground=\"yellow\">" + state + "</span>";
    if(state == "Anarchy")
      state = "<span foreground=\"red\">" + state + "</span>";
    if(state == "Riots")
      state = "<span foreground=\"yellow\">" + state + "</span>";
    auto* Status_Label = Gtk::make_managed<Gtk::Label>(" ");
    Status_Label->set_markup(state);
    int stability = City.Get_Stability();
    string stability_label_text = " ";
    if(stability < 25)
      stability_label_text = "<span foreground=\"red\">" + to_string(stability) + "(Rebellion Risk)" + "</span>";
    else if(stability < 50)
      stability_label_text = "<span foreground=\"yellow\">" + to_string(stability)  + " (+5 Maitenance Cost)" + "</span>";
    else
      stability_label_text = to_string(stability);
    auto* Stability_Label = Gtk::make_managed<Gtk::Label>(" ");
    Stability_Label->set_markup(stability_label_text);
    City_Box->pack_start(*(City_Icon->Get_Gtk_Image()));
    City_Box->pack_start(*City_Name_Label);
    City_Box->pack_start(*Status_Label);
    City_Box->pack_start(*Stability_Label);
    Cities_Box.pack_start(*City_Box);
  }
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Explanation_Box);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  Root_Box.pack_start(Goverment_Label);
  Root_Box.pack_start(Cities_Frame);
  Cities_Frame.add(Cities_Window);
  Cities_Window.add(Cities_Box);
}
