#include "newspaper_dialog.h"

Newspaper_Dialog::Newspaper_Dialog(vector<string> events) : Themed_Dialog("Newspaper")
{
  reverse(events.begin(), events.end());
  Gtk::Box *Dialog_Box = get_content_area();
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Events");
  auto* Dialog_Scrolled_Window = Gtk::make_managed<Gtk::ScrolledWindow>();
  Dialog_Scrolled_Window->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window->set_min_content_height(300);
  auto* Events_List_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL,2);
  Dialog_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Dialog_Scrolled_Window);
  Dialog_Scrolled_Window->add(*Events_List_Box);
  for(auto &event : events)
  {
    auto *label = Gtk::make_managed<Gtk::Label>(event);
    Events_List_Box->pack_start(*label);
    Main_Provider.Add_CSS(label);
  }
}
