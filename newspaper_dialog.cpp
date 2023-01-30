#include "newspaper_dialog.h"

Newspaper_Dialog::Newspaper_Dialog(const vector<Newspaper_Event>&  events, int currently_moving_player_id) : Themed_Dialog("Newspaper"), Explanation_Image(string(assets_directory_path) + "textures/dialogs/newspaper-dialog-texture.svg", 64, 64), Events(events)
{
  this->currently_moving_player_id = currently_moving_player_id;
  Gtk::Box *Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Below are listed all events that have occured from game launch. \n Most notable events that will appear here are:\n -city settlements, \n-wars & conquers, \n-nuclear attacks");
  Dialog_Root_Frame = Gtk::Frame("Events");
  Show_Only_Currently_Moving_Player_Events_Button = Gtk::CheckButton("Show only events about your empire");
  Show_Only_Currently_Moving_Player_Events_Button.set_active(false);
  Show_Only_Currently_Moving_Player_Events_Button.signal_toggled().connect(sigc::mem_fun(*this, &Newspaper_Dialog::Check_Button_Clicked));
  Dialog_Scrolled_Window = Gtk::ScrolledWindow();
  Dialog_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window.set_min_content_height(300);
  Events_List_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Explanation_Box);
  Explanation_Box.pack_start((Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  Root_Box.pack_start(Show_Only_Currently_Moving_Player_Events_Button);
  Root_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Dialog_Scrolled_Window);
  Dialog_Scrolled_Window.add(Events_List_Box);
  Update_Events(false);
}

void Newspaper_Dialog::Update_Events(bool only_currently_moving_player)
{
  auto children = Events_List_Box.get_children();
  for( auto &var : children)
  {
    var->hide();
    Events_List_Box.remove(*var);
  }
  Event_Images.clear();
  if(Events.size() == 0)
  {
    auto *label = Gtk::make_managed<Gtk::Label>("Events will appear here during game...");
    Events_List_Box.pack_start(*label);
    Main_Provider.Add_CSS(label);
  }
  int start = Events.size() -1;
  while(start > -1)
  {
    string event_text = Events[start].Get_Event().data();
    string color = "white";
    if(Events[start].Get_Player_Id() != currently_moving_player_id && only_currently_moving_player)
      continue;
    if(Events[start].Get_Player_Id() == currently_moving_player_id)
      color = "blue";
    if(Events[start].Is_Decorative())
      color = "#CCCCCC";
    event_text = "<span foreground =\"" + color + "\">" + event_text + "</span>";
    auto *label = Gtk::make_managed<Gtk::Label>("");
    shared_ptr<Scaled_Gtk_Image> image = make_shared<Scaled_Gtk_Image>(Events[start].Get_Texture_Path().data(), 24 ,24);
    Event_Images.push_back(image);
    label->set_markup(event_text);
    auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    box->pack_start((image->Get_Gtk_Image()), Gtk::PACK_SHRINK);
    box->pack_start(*label);
    Events_List_Box.pack_start(*box);
    Main_Provider.Add_CSS(label);
    start--;
  }
  show_all_children();
}

void Newspaper_Dialog::Check_Button_Clicked()
{
  Update_Events(Show_Only_Currently_Moving_Player_Events_Button.get_active());
}
