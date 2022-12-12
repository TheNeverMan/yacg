#pragma once
#include<gtkmm.h>
#include<vector>
#include<tuple>
#include<memory>
#include<algorithm>
#include<vector>
#include<string>
#include<random>

#include "map_generator_data.h"
#include "window_manager.h"
#include "game_css_provider.h"
#include "settings_manager.h"
#include "logger.h"
#include "map_loader_dialog.h"
#include "civ.h"
#include "xml_data_loader.h"
#include "assets_path.h"
#include "scaled_gtk_image.h"
#include "tutorial_dialog.h"
#include "sound_button.h"
#include "apply_button.h"
#include "civ_trait_manager.h"

using std::string;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::get;
using std::array;
using std::shared_ptr;
using std::make_shared;

class Game_Creation_Window : public Gtk::Window
{
  private:
    Civ_Trait_Manager Trait_Manager;
    Map_Generator_Data Map_Data;
    Window_Manager* Main_Manager;
    Game_CSS_Provider Main_Provider;
    Settings_Manager Main_Settings_Manager;
    vector<Civ> Civilizations;
    vector<tuple<string,bool>> Players;
    vector<string> AI_Players;
    vector<string> Human_Players;
    vector<Gtk::Box*> AI_Players_Boxes;
    vector<Gtk::Box*> Human_Players_Boxes;
    void Show_Map_Selection_Dialog();
  protected:
    Gtk::ScrolledWindow Main_Scrolled_Window;
    Gtk::Frame Dialog_Map_Frame;
    Gtk::Box Dialog_Root_Box;
    Gtk::Box Map_UI_Box;
    Gtk::Box Buttons_Box;
    Apply_Button Play_Button;
    Sound_Button Quit_Button;
    Sound_Button Map_Button;
    Gtk::Frame Buttons_Frame;
    Gtk::Label Map_Path_Label;
    Gtk::Frame Dialog_Players_Frame;
    Gtk::Box Players_UI_Box;
    Gtk::Box Main_Player_UI_Box;
    Gtk::Box Map_Data_Box;
    Gtk::Box Other_Players_Box;
    Gtk::Box Root_Box;
    Gtk::ComboBoxText Civs_Chooser_Combo;
    Scaled_Gtk_Image Civs_Color_Image;
    Gtk::Frame Civs_Description_Frame;
    Gtk::Label Civs_Description_Label;
    Gtk::Frame Civs_Trait_Frame;
    Gtk::Box Civs_Trait_Box;
    Gtk::Label Civs_Trait_Explanation_Label;
    Gtk::CheckButton Allow_Duplicate_Civs_Button;
    Gtk::CheckButton Randomize_Starting_Locations_Button;
    Gtk::CheckButton Spectator_Mode_Button;
    Gtk::SpinButton Players_Switch;
    Gtk::SpinButton Human_Players_Switch;
    Gtk::Label Other_Human_Players_Label;
    Gtk::Label Other_Players_Label;
    Gtk::Label X_Label;
    Gtk::SpinButton X_Switch;
    Gtk::Label Y_Label;
    Gtk::SpinButton Y_Switch;
    Gtk::Label Continents_Label;
    Gtk::SpinButton Continents_Switch;
    Gtk::Label Water_Label;
    Gtk::SpinButton Water_Switch;
    Gtk::Frame Civs_Frame;
    Gtk::ScrolledWindow Civs_Scrolled_Window;
    Gtk::Box Civs_Box;
    void Change_Map_Data_X_Value();
    void Change_Map_Data_Y_Value();
    void Change_Map_Data_Water_Value();
    void Change_Map_Data_Continents_Value();
    void Change_Main_Player_Civ();
    void Quit_Button_Clicked();
    void Play_Button_Clicked();
    void Change_Human_Players_Number();
    void Change_AI_Players_Number();
    void Remove_AI_Player();
    void Remove_Human_Player();
    void Map_Button_Clicked();
    void Add_AI_Player();
    void Add_Human_Player();
    void Add_Players_From_Vector(vector<string> Players_To_Add, bool Is_AI);
    void Update_Other_Player_Civ(int index, bool is_ai, shared_ptr<Scaled_Gtk_Image> image, Gtk::ComboBoxText* text);
  public:
    Game_Creation_Window(Window_Manager* m_m, Settings_Manager m_s_m);
};
