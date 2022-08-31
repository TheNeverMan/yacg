#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<gtkmm.h>
#include<vector>
#include<memory> //guide
#include<chrono> //benchmarking
#include<tuple>

#include "game.h"
#include "globals.h"
#include "window_manager.h"
#include "logger.h"
#include "game_css_provider.h"
#include "settings_manager.h"

class Window_Manager;

class Game_Window : public Gtk::Window
{
  public:
    Game_Window(Window_Manager *m_m, Settings_Manager m_s_m, string path = " ");
    ~Game_Window();
  protected:
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
    Gtk::Button Map_Update_Button;
    Gtk::Button Manage_Economy_Button;
    Gtk::Button Manage_Techs_Button;
    Gtk::Button Show_Civs_Button;
    Gtk::Button Manage_Goverments_Button;
    Gtk::Button Civ_Overview_Button;
    Gtk::Button Save_Button;
    Gtk::Button Load_Button;
    Gtk::Button Newspaper_Button;
    Gtk::Button Quit_Button;
    Gtk::Button Help_Button;
    Gtk::ScrolledWindow Map_Scrolled_Window;
    vector<Gtk::Box*> Map_Images;
    Gtk::Label ProgressBar_Label;
    void Save_Game();
    void Load_Game();
    bool Tile_Clicked(GdkEventButton* tile_event, vector<int> coords, Gtk::Image *img);
    void Show_Civs_Clicked();
    void Manage_Techs_Clicked();
    void Manage_Goverments_Clicked();
    void Change_Technology_Goal(Tech new_goal);
    void Set_Research_Funds_Percentage(Gtk::SpinButton *spin);
    void Build_Upgrade_By_Name_On_Tile(string upg_name, int x, int y, int owner);
    void Change_Goverment(Gov new_gov);
    void Recruit_Unit(string u, int x, int y);
    void Select_Unit(int x, int y);
    void Deselect_Unit();
    void Manage_Overview_Clicked();
    void Disband_Unit(int x, int y);
    void Manage_Economy_Clicked();
    void Heal_Unit(int x, int y);
    void Show_Newspaper_Clicked();
    void Change_Map_Data_X_Value(Gtk::SpinButton *spin);
    void Change_Map_Data_Y_Value(Gtk::SpinButton *spin);
    void Change_Map_Data_Continents_Value(Gtk::SpinButton *spin);
    void Change_Map_Data_Water_Value(Gtk::SpinButton *spin);
    void Change_Player_Count_Value(Gtk::SpinButton *spin);
    void Change_Main_Player_Civ(Gtk::Label *info_label, Gtk::Image *civ_color, Gtk::ComboBoxText *combo);
    void Exit_To_Main_Menu();
    void Show_Help_Message();
    void Show_Themed_Dialog(string message);
    //Gtk::ProgressBar Refresh_Bar;
  private:
    Window_Manager* Main_Manager;
    Settings_Manager Main_Settings_Manager;
    bool is_delete_of_game_necessary= false;
    void Show_Game_Creation_Dialog();
    Map_Generator_Data Map_Data;
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
    Game Main_Game;
    void Generate_Map_View();
    void Update_Map();
    int last_clicked_x;
    int last_clicked_y;
    bool is_unit_selected;
    int selected_unit_x;
    int selected_unit_y;
    int other_players_player_count;
    string main_player_civ_name;
    Gtk::Image *Last_Clicked_Tile;
    void Update_Tile(Gtk::Image *tile_image, int x, int y);
};
