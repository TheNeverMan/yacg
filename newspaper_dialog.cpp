#include "newspaper_dialog.h"

Newspaper_Dialog::Newspaper_Dialog(vector<string> events) : Themed_Dialog("Newspaper"), Explanation_Image(assets_directory_path + "textures/dialogs/newspaper-dialog-texture.png", 64, 64)
{
  reverse(events.begin(), events.end());
  Gtk::Box *Dialog_Box = get_content_area();
  Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Below are listed all events that have occured from game launch. \n Most notable events that will appear here are:\n -city settlements, \n-wars & conquers, \n-nuclear attacks");
  Dialog_Root_Frame = Gtk::Frame("Events");
  Dialog_Scrolled_Window = Gtk::ScrolledWindow();
  Dialog_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window.set_min_content_height(300);
  Events_List_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Explanation_Box);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  Root_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Dialog_Scrolled_Window);
  Dialog_Scrolled_Window.add(Events_List_Box);
  if(events.size() == 0)
  {
    auto *label = Gtk::make_managed<Gtk::Label>("Events will appear here during game...");
    Events_List_Box.pack_start(*label);
    Main_Provider.Add_CSS(label);
  }
  for(auto &event : events)
  {
    auto *label = Gtk::make_managed<Gtk::Label>(event);
    Events_List_Box.pack_start(*label);
    Main_Provider.Add_CSS(label);
  }
}
