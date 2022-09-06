#include "themed_dialog.h"

Themed_Dialog::Themed_Dialog(string m, string title) : Gtk::Dialog(title)
{
  message = m;
  Main_Provider.Add_CSS(this);
  add_button("Ok", 0);
  auto *Dialog_Label = Gtk::make_managed<Gtk::Label>(message);
  Gtk::Box *Dialog_Box = get_content_area();
  Dialog_Box->pack_start(*Dialog_Label);
}

Themed_Dialog::Themed_Dialog(string title) : Gtk::Dialog(title)
{
  Main_Provider.Add_CSS(this);
}

void Themed_Dialog::Show()
{
  Logger::Log_Info("Showing Dialog...");
  show_all_children();
  run();
}
