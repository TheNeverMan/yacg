#include "intro_window.h"
#include "assets_path.h"

Intro_Window::Intro_Window(Window_Manager* m_m) : Main_Settings_Manager("miniyacg-config-settings.xml"), Play_Button("Play!"), Quit_Button("Exit"), About_Button("About"), Settings_Button("Settings"), Load_Game_Button("Load Game"), Load_Autosave_Button("Load Autosave"), Civs_Button("Civilizations")
{
  auto provider = Gtk::CssProvider::create();
  Main_Manager = m_m;
  UI_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10);
  Version_Label = Gtk::Label(Display_Version());
  Logo = Gtk::Image(assets_directory_path + "logo.png");
  add(UI_Root_Box);
  string icon_directory = "assets/textures/icons/";
  Play_Button.Change_Icon(icon_directory + "play-icon.svg.png");
  UI_Root_Box.pack_start(Logo);
  UI_Root_Box.pack_start(Play_Button);
  Load_Game_Button.Change_Icon(icon_directory + "load-icon.svg.png");
  UI_Root_Box.pack_start(Load_Game_Button);
  Load_Autosave_Button.Change_Icon(icon_directory + "load-icon.svg.png");
  UI_Root_Box.pack_start(Load_Autosave_Button);
  Civs_Button.Change_Icon(icon_directory + "overview-icon.svg.png");
  UI_Root_Box.pack_start(Civs_Button);
  Settings_Button.Change_Icon(icon_directory + "settings-icon.svg.png");
  UI_Root_Box.pack_start(Settings_Button);
  About_Button.Change_Icon(icon_directory + "about-icon.svg.png");
  UI_Root_Box.pack_start(About_Button);
  Quit_Button.Change_Icon(icon_directory + "exit-icon.svg.png");
  UI_Root_Box.pack_start(Quit_Button);
  UI_Root_Box.pack_start(Version_Label);
  Main_Provider.Add_CSS(&Version_Label);
  Main_Provider.Add_CSS(this);

  Play_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Play_Button_Clicked) );
  Quit_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Quit_Button_Clicked) );
  Settings_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Settings_Button_Clicked) );
  About_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::About_Button_Clicked) );
  Load_Game_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Load_Game_Button_Clicked) );
  Load_Autosave_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Load_Autosave_Button_Clicked) );
  Civs_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Civs_Button_Clicked) );

  set_decorated(false);
  show_all_children();
}


void Intro_Window::Settings_Button_Clicked()
{
  Settings_Dialog Dialog;
  Dialog.Show();
  Main_Settings_Manager.Reload_Settings();
}

void Intro_Window::Play_Button_Clicked()
{
  Main_Manager->Show_Game_Creation_Window(Main_Settings_Manager);
}

void Intro_Window::Quit_Button_Clicked()
{
  Main_Manager->Close_Game();
}

void Intro_Window::Load_Game_Button_Clicked()
{
  Save_Loader_Dialog Dialog;
  Dialog.Show();
  string path = Dialog.Get_File_Path();
  if(path != " ")
    Main_Manager->Show_Game_Window_Load_From_File(Main_Settings_Manager, path, false);
}

void Intro_Window::Civs_Button_Clicked()
{
  Trait_Dialog Dialog;
  Dialog.Show();
}

void Intro_Window::Load_Autosave_Button_Clicked()
{
  Logger::Log_Info("Loading Autosave...");
  Main_Manager->Show_Game_Window_Load_From_File(Main_Settings_Manager, "autosave.sav", false);
}
void Intro_Window::About_Button_Clicked()
{
  Gtk::Dialog dialog("About");
  dialog.add_button("Ok", 0);
  string message = "TheNeverMan - coding & game idea \n Spykli - game idea & civs \n Kamix - textures \n Alex_Alex - civilizations \n fiStach - textures & tech tree \n Flag Textures License Info: \n Flags of currrently existing states have no license and are public domain. \n Other flags have Creative Commons License versions 1.0, 2.5, 3.0, 4.0. \n Github - github.com/theneverman/yacg \n Discord - https://discord.gg/MBxsNUzJGZ";
  Gtk::Label Dialog_Label = Gtk::Label(message);
  Gtk::Box *Dialog_Box = dialog.get_content_area();
  Dialog_Box->pack_start(Dialog_Label);
  dialog.show_all_children();
  Main_Provider.Add_CSS(&dialog);
  dialog.run();
}
