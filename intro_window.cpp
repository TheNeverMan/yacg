#include "intro_window.h"
#include "globals.h"

Intro_Window::Intro_Window(Window_Manager* m_m) : Main_Settings_Manager("miniyacg-config-settings.xml")
{
  auto provider = Gtk::CssProvider::create();
  Main_Manager = m_m;
  UI_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10);
  Play_Button = Gtk::Button("Play!");
  Quit_Button = Gtk::Button("Exit");
  About_Button = Gtk::Button("About");
  Load_Game_Button = Gtk::Button("Load Game");
  Settings_Button = Gtk::Button("Settings");
  Load_Autosave_Button = Gtk::Button("Load Autosave");
  Version_Label = Gtk::Label(Display_Version());
  Logo = Gtk::Image(assets_directory_path + "logo.png");
  add(UI_Root_Box);
  UI_Root_Box.pack_start(Logo);
  UI_Root_Box.pack_start(Play_Button);
  UI_Root_Box.pack_start(Load_Game_Button);
  UI_Root_Box.pack_start(Load_Autosave_Button);
  UI_Root_Box.pack_start(Settings_Button);
  UI_Root_Box.pack_start(About_Button);
  UI_Root_Box.pack_start(Quit_Button);
  UI_Root_Box.pack_start(Version_Label);
  Main_Provider.Add_CSS(&Play_Button);
  Main_Provider.Add_CSS(&Load_Game_Button);
  Main_Provider.Add_CSS(&Settings_Button);
  Main_Provider.Add_CSS(&About_Button);
  Main_Provider.Add_CSS(&Version_Label);
  Main_Provider.Add_CSS(&Quit_Button);
  Main_Provider.Add_CSS(&Load_Autosave_Button);
  Main_Provider.Add_CSS(this);

  Play_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Play_Button_Clicked) );
  Quit_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Quit_Button_Clicked) );
  Settings_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Settings_Button_Clicked) );
  About_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::About_Button_Clicked) );
  Load_Game_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Load_Game_Button_Clicked) );
  Load_Autosave_Button.signal_clicked().connect( sigc::mem_fun(*this, &Intro_Window::Load_Autosave_Button_Clicked) );

  show_all_children();
}

void Intro_Window::Change_Tile_Size_Value(Gtk::SpinButton* Tile_Size_Button)
{
  Main_Settings_Manager.Set_Tile_Size_Value((int)Tile_Size_Button->get_value());
}

void Intro_Window::Change_Autosave_Value(Gtk::CheckButton* Autosave_Button)
{
  Main_Settings_Manager.Set_Autosave_Value(Autosave_Button->get_active());
}

void Intro_Window::Change_Autoresize_Value(Gtk::CheckButton* Autoresize_Button)
{
  Main_Settings_Manager.Set_Autoresize_Tiles_Value(Autoresize_Button->get_active());
}


void Intro_Window::Settings_Button_Clicked()
{
  Logger::Log_Info("Showing Settings Dialog...");
  Gtk::Dialog Settings_Dialog("Settings");
  Main_Provider.Add_CSS(&Settings_Dialog);
  Settings_Dialog.add_button("Apply", 0);
  Gtk::Box *Dialog_Box = Settings_Dialog.get_content_area();
  Gtk::Frame Dialog_Settings_Frame("Game Settings");
  Gtk::Box Dialog_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Gtk::CheckButton Autosave_Button = Gtk::CheckButton("Autosave Between Every Turn");
  Gtk::CheckButton Autoresize_Button = Gtk::CheckButton("Autoresize tile textures when playing on small maps to avoid tiles being spaced.");
  Gtk::Label Tile_Size_Label = Gtk::Label("Choose size of tile texture (if you play in high resolution on small maps there may be visible empty spaces between map textures if they are too small):");
  Glib::RefPtr<Gtk::Adjustment> Tile_Size_Adjustment;
  Tile_Size_Adjustment = Gtk::Adjustment::create(Main_Settings_Manager.Get_Tile_Size_Value(),0.0,512.0,1.0,10,0.0);
  Gtk::SpinButton Tile_Size_Switch(Tile_Size_Adjustment);
  Dialog_Box->add(Dialog_Settings_Frame);
  Dialog_Settings_Frame.add(Dialog_Root_Box);
  Dialog_Root_Box.pack_start(Autosave_Button);
  Dialog_Root_Box.pack_start(Autoresize_Button);
  Dialog_Root_Box.pack_start(Tile_Size_Switch);
  Tile_Size_Switch.signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Intro_Window::Change_Tile_Size_Value), &Tile_Size_Switch ));
  Autosave_Button.signal_toggled().connect(sigc::bind<Gtk::CheckButton*>(sigc::mem_fun(*this, &Intro_Window::Change_Autosave_Value), &Autosave_Button));
  Autoresize_Button.signal_toggled().connect(sigc::bind<Gtk::CheckButton*>(sigc::mem_fun(*this, &Intro_Window::Change_Autoresize_Value), &Autoresize_Button));

  Settings_Dialog.show_all_children();
  Settings_Dialog.run();
}

void Intro_Window::Play_Button_Clicked()
{
  Main_Settings_Manager.Write_To_File();
  Main_Manager->Show_Game_Creation_Window(Main_Settings_Manager);
}

void Intro_Window::Quit_Button_Clicked()
{
  Main_Manager->Close_Game();
}

void Intro_Window::Load_Game_Button_Clicked()
{
  Gtk::FileChooserDialog Load_Game_File_Chooser_Dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  Load_Game_File_Chooser_Dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  Load_Game_File_Chooser_Dialog.add_button("_Open", Gtk::RESPONSE_OK);
  int result = Load_Game_File_Chooser_Dialog.run();

 //Handle the response:
 switch(result)
 {
   case(Gtk::RESPONSE_OK):
   {
     string path = Load_Game_File_Chooser_Dialog.get_filename();
     Logger::Log_Info("Path to load is " + path );
     Main_Settings_Manager.Write_To_File();
     Main_Manager->Show_Game_Window_Load_From_File(Main_Settings_Manager, path);
     break;
   }
   default:
   {
    Logger::Log_Error("Unexpected button clicked." );
     break;
   }
 }
}

void Intro_Window::Load_Autosave_Button_Clicked()
{
  Logger::Log_Info("Loading Autosave...");
  Main_Manager->Show_Game_Window_Load_From_File(Main_Settings_Manager, "autosave.sav");
}
void Intro_Window::About_Button_Clicked()
{
  Gtk::Dialog dialog("About");
  dialog.add_button("Ok", 0);
  string message = "TheNeverMan - coding & game idea \n Spykli - game idea \n Kamix - textures \n Alex_Alex - civilizations \n Github - github.com/theneverman/yacg \n Discord - https://discord.gg/MBxsNUzJGZ";
  Gtk::Label Dialog_Label = Gtk::Label(message);
  Gtk::Box *Dialog_Box = dialog.get_content_area();
  Dialog_Box->pack_start(Dialog_Label);
  dialog.show_all_children();
  Main_Provider.Add_CSS(&dialog);
  dialog.run();
}
