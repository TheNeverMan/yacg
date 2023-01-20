#include "help_manager.h"

Help_Manager::Help_Manager() : tutorial_text_path(string{assets_directory_path} + "tutorial/"), tutorial_image_path(string(assets_directory_path) + "textures/tutorial/")
{

}

void Help_Manager::Show_Basic_Tutorial() const
{
  Tutorial_Dialog General_Tutorial(tutorial_image_path+ "general-tutorial.png", tutorial_text_path + "general-tutorial.txt");
  Tutorial_Dialog Economy_Tutorial(tutorial_image_path+ "economy-tutorial.png", tutorial_text_path + "economy-tutorial.txt");
  Tutorial_Dialog Units_Tutorial(tutorial_image_path+ "units-tutorial.png", tutorial_text_path + "units-tutorial.txt");
  Tutorial_Dialog Goverments_Tutorial(tutorial_image_path+ "goverments-tutorial.png", tutorial_text_path + "goverments-tutorial.txt");
  Tutorial_Dialog Other_Tutorial(tutorial_image_path+ "other-tutorial.png", tutorial_text_path + "other-tutorial.txt");
  Tutorial_Dialog Stability_Tutorial(tutorial_image_path + "stability-tutorial.png", tutorial_text_path + "stability-tutorial.txt");
  Tutorial_Dialog Help_Tutorial(tutorial_image_path + "help-tutorial.png", tutorial_text_path + "help-tutorial.txt");
  General_Tutorial.Show();
  Economy_Tutorial.Show();
  Units_Tutorial.Show();
  Goverments_Tutorial.Show();
  Other_Tutorial.Show();
  Stability_Tutorial.Show();
  Help_Tutorial.Show();
}

void Help_Manager::Show_Keybindings_Dialog() const
{
  Tutorial_Dialog Keybindings_Tutorial(tutorial_image_path + "keybindings-tutorial.png", tutorial_text_path + "keybindings-tutorial.txt");
  Keybindings_Tutorial.Show();
}

void Help_Manager::Show_All_Traits_Dialog() const
{

}

void Help_Manager::Show_Game_Help() const
{
  Game_CSS_Provider Main_Provider;
  Gtk::Dialog dialog("Help");
  dialog.add_button("Ok", 0);
  fstream Help_File(string(assets_directory_path) + "help_file.txt");
  string message = "Loading Help Text Failed!";
  if(Help_File.is_open())
  {
    std::ostringstream sstr;
    sstr << Help_File.rdbuf();
    message = sstr.str();
  }
  else
  {
    Logger::Log_Error("Loading Help File failed!");
  }
  Gtk::Label Dialog_Label = Gtk::Label(message);
  Gtk::Box *Dialog_Box = dialog.get_content_area();
  Gtk::ScrolledWindow Main_Scrolled_Window;// = Gtk::ScrolledWindow(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  Main_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Main_Scrolled_Window.set_min_content_height(600);
  dialog.set_default_size(600,300);
  Dialog_Box->pack_start(Main_Scrolled_Window);
  Main_Scrolled_Window.add(Dialog_Label);
  dialog.show_all_children();
  Main_Provider.Add_CSS(&dialog);
  dialog.run();
}
