#include "themed_dialog.h"

Themed_Dialog::Themed_Dialog(string m, string title) : Gtk::Dialog(title), Close_Button("Ok")
{
  message = m;
  Main_Provider.Add_CSS(this);
  auto *Dialog_Label = Gtk::make_managed<Gtk::Label>(message);
  Gtk::Box *Dialog_Box = get_content_area();
  Dialog_Box->pack_start(*Dialog_Label);
  set_decorated(false);
  Add_Close_Button();
}

Themed_Dialog::Themed_Dialog(string title) : Gtk::Dialog(title), Close_Button("Ok")
{
  Main_Provider.Add_CSS(this);
  Add_Close_Button();
}

void Themed_Dialog::Show()
{
  Logger::Log_Info("Showing Dialog...");
  show_all_children();
  run();
}

void Themed_Dialog::Add_Close_Button()
{
  Gtk::Box *Dialog_Box = get_content_area();
  Dialog_Box->pack_end(Close_Button);
  Image_Path Icon_Path("assets/textures/icons/apply-icon.svg.png");
  Icon_Image = make_shared<Scaled_Gtk_Image>(Icon_Path.Get_File_Path().data(), 24 ,24);
  Close_Button.set_image((Icon_Image->Get_Gtk_Image()));
  Close_Button.signal_clicked().connect(sigc::mem_fun(*this, &Themed_Dialog::Close_Button_Clicked));
}


void Themed_Dialog::Close_Button_Clicked()
{
  close();
}
