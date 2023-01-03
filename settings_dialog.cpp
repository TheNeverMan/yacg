#include "settings_dialog.h"

Settings_Dialog::Settings_Dialog() : Themed_Dialog("Settings", "Apply"), Explanation_Image("assets/textures/dialogs/settings-dialog-texture.svg", 64, 64), Main_Settings_Manager("miniyacg-config-settings.xml")
{
  Logger::Log_Info("Showing Settings Dialog...");
  Gtk::Box* Dialog_Box = get_content_area();
  Dialog_Settings_Frame = Gtk::Frame("Game Settings");
  Dialog_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Autosave_Button = Gtk::CheckButton("Autosave Between Every Turn");
  Autoresize_Button = Gtk::CheckButton("Autoresize tile textures when playing on small maps to avoid tiles being spaced.");
  Startup_Tip_Button = Gtk::CheckButton("Show random tip on launch.");
  Mute_Button = Gtk::CheckButton("Mute All Sounds");
  Mute_Music_Button = Gtk::CheckButton("Mute Music Only");
  Tile_Size_Label = Gtk::Label("Choose size of tile texture (if you play in high resolution on small maps there may be visible empty spaces between map textures if they are too small):");
  Autosave_Button.set_active(Main_Settings_Manager.Get_Autosave_Value());
  Autoresize_Button.set_active(Main_Settings_Manager.Get_Autoresize_Tiles_Value());
  Startup_Tip_Button.set_active(Main_Settings_Manager.Get_Random_Tip_On_Startup_Value());
  Mute_Button.set_active(Main_Settings_Manager.Is_Game_Muted());
  Mute_Music_Button.set_active(Main_Settings_Manager.Is_Music_Muted());
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Here you can set various game settings:");
  Glib::RefPtr<Gtk::Adjustment> Tile_Size_Adjustment;
  Tile_Size_Adjustment = Gtk::Adjustment::create(Main_Settings_Manager.Get_Tile_Size_Value(),32.0,512.0,1.0,10,0.0);
  Tile_Size_Switch = Gtk::SpinButton(Tile_Size_Adjustment);
  Dialog_Box->pack_start(Explanation_Box);
  Explanation_Box.pack_start(*(Explanation_Image.Get_Gtk_Image())); //???
  Explanation_Box.pack_start(Explanation_Label);
  Dialog_Box->pack_start(Dialog_Settings_Frame);
  Dialog_Settings_Frame.add(Dialog_Root_Box);
  Dialog_Root_Box.pack_start(Autosave_Button);
  Dialog_Root_Box.pack_start(Mute_Button);
  Dialog_Root_Box.pack_start(Mute_Music_Button);
  Dialog_Root_Box.pack_start(Autoresize_Button);
  Dialog_Root_Box.pack_start(Startup_Tip_Button);
  Dialog_Root_Box.pack_start(Tile_Size_Switch);
  Tile_Size_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Settings_Dialog::Change_Tile_Size_Value));
  Autosave_Button.signal_toggled().connect(sigc::mem_fun(*this, &Settings_Dialog::Change_Autosave_Value));
  Autoresize_Button.signal_toggled().connect(sigc::mem_fun(*this, &Settings_Dialog::Change_Autoresize_Value));
  Startup_Tip_Button.signal_toggled().connect(sigc::mem_fun(*this, &Settings_Dialog::Change_Startup_Tip_Value));
  Mute_Button.signal_toggled().connect(sigc::mem_fun(*this, &Settings_Dialog::Change_Mute_Value));
  Mute_Music_Button.signal_toggled().connect(sigc::mem_fun(*this, &Settings_Dialog::Change_Music_Mute_Value));

}

void Settings_Dialog::Change_Tile_Size_Value()
{
  Sound_Manager::Play_Sound("assets/sounds/apply-audio.mp3");
  Main_Settings_Manager.Set_Tile_Size_Value((int)Tile_Size_Switch.get_value());
}

void Settings_Dialog::Change_Autosave_Value()
{
  Sound_Manager::Play_Sound("assets/sounds/apply-audio.mp3");
  Main_Settings_Manager.Set_Autosave_Value(Autosave_Button.get_active());
}

void Settings_Dialog::Change_Autoresize_Value()
{
  Sound_Manager::Play_Sound("assets/sounds/apply-audio.mp3");
  Main_Settings_Manager.Set_Autoresize_Tiles_Value(Autoresize_Button.get_active());
}

void Settings_Dialog::Change_Startup_Tip_Value()
{
  Sound_Manager::Play_Sound("assets/sounds/apply-audio.mp3");
  Main_Settings_Manager.Set_Random_Tip_On_Startup_Value(Startup_Tip_Button.get_active());
}

void Settings_Dialog::Change_Mute_Value()
{
  Sound_Manager::Play_Sound("assets/sounds/apply-audio.mp3");
  Main_Settings_Manager.Set_Mute_Value(Mute_Button.get_active());
}

void Settings_Dialog::Change_Music_Mute_Value()
{
  Sound_Manager::Play_Sound("assets/sounds/apply-audio.mp3");
  Main_Settings_Manager.Set_Music_Mute_Value(Mute_Music_Button.get_active());
}
Settings_Dialog::~Settings_Dialog()
{
  Logger::Log_Info("Saving Data!");
  Main_Settings_Manager.Write_To_File();
}
