#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<vector>
#include<memory> //guide
#include<chrono> //benchmarking
#include<tuple>
#include<map>
#include<thread>
#include<gtkmm.h>

#include "game.h"
#include "assets_path.h"
#include "window_manager.h"
#include "logger.h"
#include "game_css_provider.h"
#include "settings_manager.h"
#include "themed_dialog.h"
#include "civs_dialog.h"
#include "overview_dialog.h"
#include "economy_dialog.h"
#include "unit_info_dialog.h"
#include "upgrade_info_dialog.h"
#include "tech_dialog.h"
#include "goverment_dialog.h"
#include "newspaper_dialog.h"
#include "tips_manager.h"
#include "magic_thread_communicator.h"
#include "save_loader_dialog.h"
#include "save_saver_dialog.h"
#include "gtk_tile.h"
#include "scaled_gtk_image.h"
#include "tutorial_dialog.h"
#include "gtk_game_map.h"
#include "magic_map_generation_thread_communicator.h"
#include "map_generation_dialog.h"
#include "sound_manager.h"
#include "apply_button.h"
#include "sound_button.h"

using std::string;
using std::map;
using std::vector;
using std::fstream;
using std::shared_ptr;
using std::tuple;
using std::make_tuple;
using std::get;
using std::make_shared;
using std::map;
using std::thread;

class Window_Manager;
class Game;
class Magic_Thread_Communicator;
class Magic_Map_Generation_Thread_Communicator;
class Gtk_Game_Map;

class Game_Window : public Gtk::Window
{
  public:
    Game_Window(Window_Manager *m_m, Settings_Manager m_s_m, string path, bool spectator_mode);
    Game_Window(Window_Manager *m_m, Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>>  players, bool load_starting_positions, bool spectator_mode);
    ~Game_Window();
    void Notify_Game_Window_About_Turn();
  protected:
    bool is_everythin_disabled = false;
    Game_CSS_Provider Main_Provider;
    Gtk::Box Root_Box;
    Gtk::Box Map_Root_Box;
    Gtk::Frame Map_Frame;
    Gtk::Frame UI_Frame;
    Gtk::Label Capital_Label;
    Gtk::Box UI_Root_Box;
    Gtk::Box End_Turn_Box;
    Gtk::Box Action_Buttons_Box;
    Gtk::Frame Action_Buttons_Frame;
    Gtk::Button End_Turn_Button;
    Gtk::Label Economy_Label;
    Gtk::Label Tile_Information_Label;
    Scaled_Gtk_Image Tile_Flag_Image;
  //  Gtk::Button Map_Update_Button;
    Sound_Button Manage_Economy_Button;
    Sound_Button Manage_Techs_Button;
    Sound_Button Show_Civs_Button;
    Sound_Button Manage_Goverments_Button;
    Sound_Button Civ_Overview_Button;
    Sound_Button Save_Button;
    Gtk::Frame Zoom_Frame;
    Gtk::Box Zoom_Box;
    Sound_Button Zoom_In_Button;
    Sound_Button Zoom_Out_Button;
    Sound_Button Load_Button;
    Sound_Button Newspaper_Button;
    Sound_Button Quit_Button;
    Sound_Button Help_Button;
    Gtk::Button Random_Tip_Button;
  //  Gtk::Button Tip_Button;
    Gtk::ScrolledWindow Map_Scrolled_Window;
    shared_ptr<Gtk_Game_Map> Map_Images;
    Gtk::Label ProgressBar_Label;
    void Save_Game();
    void Load_Game();
    bool Tile_Clicked(GdkEventButton* tile_event, vector<int> coords, Gtk::Image *img);
    void Show_Civs_Clicked();
    void Manage_Techs_Clicked();
    void Manage_Goverments_Clicked();
    void Build_Upgrade_By_Name_On_Tile(string upg_name, int x, int y, int owner);
    void Recruit_Unit(string u, int x, int y);
    void Select_Unit(int x, int y);
    void Deselect_Unit();
    void Manage_Overview_Clicked();
    void Disband_Unit(int x, int y);
    void Manage_Economy_Clicked();
    void Heal_Unit(int x, int y);
    void Show_Newspaper_Clicked();
    void Exit_To_Main_Menu();
    void Show_Help_Message();
    void Show_Themed_Dialog(string message);
    void Show_Unit_Info_Dialog(Unit u);
    void Show_Upgrade_Info_Dialog(Upgrade u);
    void Plunder_Tile(int x, int y);
    void Fix_Tile(int x, int y);
    void Detonate_Atomic_Bomb(int x, int y);
    void Show_Random_Tip();
    void Show_Tip();
    void Update_End_Turn_Labels();
    void Zoom_Out();
    void Zoom_In();
  private:
    Window_Manager* Main_Manager;
    Settings_Manager Main_Settings_Manager;
    bool is_delete_of_game_necessary= false;
    Map_Generator_Data Map_Data;
    Tips_Manager Main_Tips_Manager;
    Sound_Manager Main_Sound_Manager;
    int minimum_tile_size = 0;
    void Update_Action_Buttons(int x, int y);
    void Update_Unit_Action_Buttons(int x, int y);
    void Update_Tile_Action_Buttons(int x, int y);
    void Clear_Action_Buttons();
    void Test();
    void Clear_Map_Images();
    void End_Turn();
    void Update_Labels();
    void Update_Economy_Label();
    void Update_Capital_Label();
    void Show_Not_Enough_Actions_Message();
    void Show_Not_Enough_Gold_Message();
    void Update_Tile_Information_Label(int x, int y);
    void Update_Map_Frame();
    bool Is_Unit_Selected();
    void Update_Tile_By_Coords_Only(int x, int y);
    void Player_Has_Lost_Game();
    void Show_Intro_Message();
    Game* Main_Game;
    void Generate_Map_View();
    void Update_Map();
    int last_clicked_x;
    int last_clicked_y;
    bool is_unit_selected;
    int selected_unit_x;
    int selected_unit_y;
    int other_players_player_count;
    string main_player_civ_name;
    Gtk_Tile *Last_Clicked_Tile;
    void Initialize_GTK();
    void Update_Tile(shared_ptr<Gtk_Tile> Tile_Pointer, int x, int y);
    array<int ,2> Get_Screen_Resolution();
    void Set_Tiles_Size_Automatically();
    bool Check_Avoid_Trait_For_Upgrades(string upg_name, int x, int y);
    bool Check_Must_Border_Trait_For_Upgrades(string upg_name, int x, int y);
    string Get_Current_Turn_By_Years();
    void Update_Tiles_From_Game();
    void Player_Has_Won_Game(int player_id);
    void Reset_Tile_Flag_Label();
    void Update_Tile_Flag();
    void Disable_All_Buttons();
    void Enable_All_Buttons();
    Glib::Dispatcher End_Turn_Dispatcher;
    thread* End_Turn_Thread;
    thread* Map_Generation_Thread;
    void Show_Tutorial();
    bool Loop_Background_Music();
    Sound_Manager Background_Sound_Manager;
    shared_ptr<Magic_Thread_Communicator> Thread_Portal_Pointer;
    shared_ptr<Magic_Map_Generation_Thread_Communicator> Map_Generation_Thread_Portal_Pointer;
    sigc::connection Background_Music_Loop_Connection;
};
