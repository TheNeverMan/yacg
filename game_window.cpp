#include "game_window.h"

Game_Window::~Game_Window()
{
  //gtkmm guide
  delete Main_Game;
}

void Game_Window::Reset_Tile_Flag_Label()
{
  Tile_Flag_Image.Change_Path(assets_directory_path + "textures/flags/neutral-flag.png");
}

void Game_Window::Generate_Map_View()
{
  auto timer_start = std::chrono::steady_clock::now();
  Logger::Log_Info("Generating Map View..." );
  int x = Main_Game->Get_Map().Get_X_Size();
  int y = Main_Game->Get_Map().Get_Y_Size();
  Logger::Log_Info("Map Size is " + to_string(x) + " " + to_string(y) );
  auto *root = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 0);
  Map_Scrolled_Window.add(*root);
  Map_Generation_Dialog Dialog;
  Map_Generation_Thread_Portal_Pointer = make_shared<Magic_Map_Generation_Thread_Communicator>(&Dialog, Map_Images);
  Map_Generation_Thread = new std::thread(
    [this]
    {
      Map_Generation_Thread_Portal_Pointer->Do_Task(Main_Game);
    });
  Dialog.Show();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  Logger::Log_Info("Map View Generated!" );
  auto timer_end =std::chrono::steady_clock::now();
  auto timer_diff = timer_end - timer_start;
  Logger::Log_Info("Generation took: " + to_string(std::chrono::duration<double, milli>(timer_diff).count()) + " ms" );
  root->pack_start(*Map_Images);
  add_events(Gdk::BUTTON_PRESS_MASK);
  Map_Images->signal_button_press_event().connect(sigc::mem_fun(*this, &Game_Window::Game_Map_Clicked));
  show_all_children();

}

bool Game_Window::Game_Map_Clicked(GdkEventButton* key_event)
{
  int x = 0;
  int y = 0;
  if((key_event->type == GDK_BUTTON_PRESS) && (key_event->button == 1))
  {
    x = key_event->x;
    y = key_event->y;
    int tile_x = x / Map_Images->Get_Tile_Size();
    int tile_y = y / Map_Images->Get_Tile_Size();
    Tile_Clicked(nullptr, {tile_x, tile_y}, nullptr);
  }
  return true;
}

void Game_Window::Update_Tile(shared_ptr<Gtk_Tile> Tile_Pointer, int x, int y)
{
  Update_Tile_By_Coords_Only(x,y);
}

void Game_Window::Update_Tile_By_Coords_Only(int x, int y)
{
  if(!(Map_Images->Get_Gtk_Tile(x,y)->Has_City_Set()) && Main_Game->Get_Map().Get_Upgrade(x,y) == "City")
  {
    Map_Images->Set_City_Overlay({x,y}, Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(x,y)).Get_City_Name_By_Coordinates(x,y));
  }
  string_view tile_texture = Main_Game->Get_Map().Get_Tile_Pointer(x,y).Get_Texture_Path(); //this is incredibly slow pls fix
  string unit_texture = string(assets_directory_path) + "textures/upgrades/none-upgrade-texture.png";
  if(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Has_Unit())
  {
    unit_texture = Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Get_Unit_Owner_Id()).Get_Unit_On_Tile_Pointer(x,y).Get_Texture_Path();
    if(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Get_Name() == "Sea" && Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Get_Unit_Owner_Id()).Get_Unit_On_Tile(x,y).How_Many_Times_Has_Trait("naval") == 0)
      unit_texture = "assets/textures/units/embarked-unit-texture.svg";
  }

  string upgrade_texture = " ";
  if( !(Main_Game->Get_Map().Is_Tile_Neutral(x,y)) && !(Main_Game->Get_Map().Is_Tile_Upgraded(x,y)) && Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(x,y)).Find_Upgrade_By_Name(Main_Game->Get_Map().Get_Upgrade(x,y)).Has_Trait("culture"))
  {
    upgrade_texture = Main_Game->Get_Texture_Path_For_Cultured_Upgrade(x,y, Main_Game->Get_Map().Get_Upgrade(x,y));
  }
  else
    upgrade_texture = Main_Game->Get_Upgrade_By_Name(Main_Game->Get_Map().Get_Upgrade(x,y)).Get_Texture_Path();
  guint32 border_color = Main_Game->Get_Border_Color_By_Player_Id(Main_Game->Get_Map().Get_Owner(x, y));
  Map_Images->Update_Tile({tile_texture, upgrade_texture, unit_texture}, border_color, x, y);
}

void Game_Window::Update_Map()
{
  unsigned long long tiles = Main_Game->Get_Map().Get_X_Size() * Main_Game->Get_Map().Get_Y_Size();
  auto timer_start =std::chrono::steady_clock::now();
  Logger::Log_Info("Updating Map..." );
  int start = 0;
  int start_y = 0;
  int x = Main_Game->Get_Map().Get_X_Size();
  int y = Main_Game->Get_Map().Get_Y_Size();
  while(start < x)
  {
    while(start_y < y)
    {
      Update_Tile_By_Coords_Only(start,start_y);
      start_y++;
    }
    start++;
    start_y = 0;
  }
  Logger::Log_Info("Map Updated!" );
  auto timer_end =std::chrono::steady_clock::now();
  auto timer_diff = timer_end - timer_start;
  Logger::Log_Info("Update took: " + to_string(std::chrono::duration<double, milli>(timer_diff).count()) + " ms" );
  string text = "Map Updated!\n";
  text = text + "Update took: ";
  text = text + to_string(std::chrono::duration<double, milli>(timer_diff).count());
  text = text + " ms (";
  text = text + to_string(tiles);
  text = text + " tiles)";
  Set_Progress_Bar_Label_Text(text);
}

void Game_Window::Test()
{
  Update_Map();
}

void Game_Window::Show_Not_Enough_Actions_Message()
{
  Sound_Manager::Play_Sound("assets/sounds/broken-audio.mp3");
  string message = "You don't have enough actions to do that!";
  Set_Progress_Bar_Label_Text_Red(message);
}

void Game_Window::Show_Not_Enough_Gold_Message()
{
  Sound_Manager::Play_Sound("assets/sounds/broken-audio.mp3");
  string message = "You don't have enough gold to do that!";
  Set_Progress_Bar_Label_Text_Red(message);
}

bool Game_Window::Check_Avoid_Trait_For_Upgrades(string_view upg_name, int x, int y)
{
  if(!Main_Game->Get_Upgrade_By_Name(upg_name).Has_Trait("avoid"))
    return true;
  vector<string> avoids = Main_Game->Get_Upgrade_By_Name(upg_name).Get_All_Arguments_For_Trait("avoid");

  for(string &upg : avoids)
  {
    if(Main_Game->Get_Map().Is_Upgrade_In_Radius_By_Name(upg,x,y))
      return false;
  }
  return true;
}

bool Game_Window::Check_Must_Border_Trait_For_Upgrades(string_view upg_name, int x, int y)
{
  if(!Main_Game->Get_Upgrade_By_Name(upg_name).Has_Trait("mustborder"))
    return true;
  vector<string> avoids = Main_Game->Get_Upgrade_By_Name(upg_name).Get_All_Arguments_For_Trait("mustborder");
  bool out = false;
  for(string &upg : avoids)
  {
    if(Main_Game->Get_Map().Is_Upgrade_In_Radius_By_Name(upg,x,y))
      out = true;
  }
  return out;
}

void Game_Window::Update_Tiles_From_Game()
{
  vector<array<int, 2>> Tiles_To_Update = Main_Game->Get_Tiles_To_Update();
  for(auto &tile : Tiles_To_Update)
    Update_Tile_By_Coords_Only(tile[0], tile[1]);
}

void Game_Window::Build_Upgrade_By_Name_On_Tile(string_view upg_name, int x, int y, int owner)
{
  if(!Main_Game->Get_Currently_Moving_Player().Has_Enough_Gold_To_Build_Upgrade(upg_name))
  {
    Show_Not_Enough_Gold_Message();
    return;
  }
  if(!Check_Avoid_Trait_For_Upgrades(upg_name, x, y))
  {
    string message = " You can't build " + string(upg_name) + " here there is other " + string(upg_name) + " close!";
    Set_Progress_Bar_Label_Text_Red(message);
    return;
  }
  if(!Check_Must_Border_Trait_For_Upgrades(upg_name, x, y))
  {
    string message = " You can't build " + string(upg_name) + " here!";
    Set_Progress_Bar_Label_Text_Red(message);
    return;
  }
  if(! Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
  {
    Show_Not_Enough_Actions_Message();
    return;
  }
  string message = string(upg_name) + " built sucessfully!";
  int radius = Main_Game->Get_Player_By_Id(owner).Get_Upgrade_Border_Radius();
  if(upg_name == "City")
  {
    Main_Game->Build_City(x,y,owner, radius);
  }
  else
  {
    Main_Game->Build_Upgrade(upg_name, x, y, owner);
  }
  Update_Tiles_From_Game();
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Set_Progress_Bar_Label_Text(message);
}

void Game_Window::Clear_Action_Buttons()
{
  auto children = Action_Buttons_Box.get_children();
  for( auto &var : children)
  {
    var->hide();
    Action_Buttons_Box.remove(*var);
  }
}

void Game_Window::Recruit_Unit(string_view u, int x, int y)
{
  string message = " ";
  if(Main_Game->Get_Currently_Moving_Player().Get_Gold() < Main_Game->Get_Unit_By_Name(u).Get_Cost())
  {
    Show_Not_Enough_Gold_Message();
    return;
  }
  if(! Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
  {
    Show_Not_Enough_Actions_Message();
    return;
  }
  Main_Game->Recruit_Unit(u, x, y);
  message = "Unit " + string(u) + " recruited!";
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Set_Progress_Bar_Label_Text(message);
}

void Game_Window::Disband_Unit(int x, int y)
{
  string message = " ";
  if(! Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
  {
    Show_Not_Enough_Actions_Message();
    return;
  }
  if(Main_Game->Get_Currently_Moving_Player().Has_Unit_On_Tile(x,y))
  {
    Main_Game->Disband_Unit(x,y);
    Deselect_Unit();
    message = "Unit disbanded!";
  }
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Set_Progress_Bar_Label_Text(message);
}

void Game_Window::Heal_Unit(int x, int y)
{
  string message = " ";
  if(Main_Game->Get_Currently_Moving_Player().Has_Unit_On_Tile(x,y))
  {
    if(Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(x,y).Get_Current_Actions() > 1)
    {
      Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile_Pointer(x,y).Heal(Main_Game->Get_Upgrade_Of_Currently_Moving_Player(Main_Game->Get_Map().Get_Upgrade(x,y)).How_Many_Times_Has_Trait("increasehealrate") * 10);
      message = "Unit healed!";
      Sound_Manager::Play_Sound("assets/sounds/heal-audio.mp3");
    }
    else
    {
      Sound_Manager::Play_Sound("assets/sounds/broken-audio.mp3");
      message = message + "\n Your unit doesn't have enough actions to do that!";
    }
  }
  Deselect_Unit();
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Set_Progress_Bar_Label_Text(message);
}

void Game_Window::Plunder_Tile(int x, int y)
{
  string message = " ";
  if(Main_Game->Get_Currently_Moving_Player().Has_Unit_On_Tile(x,y))
  {
    if(Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(x,y).Get_Current_Actions() > 1)
    {
      Main_Game->Plunder_Tile(x,y);
      message = "Tile plundered!";
    }
    else
    {
      Sound_Manager::Play_Sound("assets/sounds/broken-audio.mp3");
      message = message + "\n Your unit doesn't have enough actions to do that!";
    }
  }
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Set_Progress_Bar_Label_Text(message);
}

void Game_Window::Fix_Tile(int x, int y)
{
  Build_Upgrade_By_Name_On_Tile("plundered", x, y, Main_Game->Get_Currently_Moving_Player_Id());
}

void Game_Window::Detonate_Atomic_Bomb(int x, int y)
{
  string message = " ";
  if(Main_Game->Get_Currently_Moving_Player().Has_Unit_On_Tile(x,y))
  {
    Main_Game->Detonate_Atomic_Bomb(x,y);
    message = "Bomb Detonated!";
  }
  Deselect_Unit();
  Update_Map();
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Set_Progress_Bar_Label_Text(message);
}

void Game_Window::Cut_Down(int x, int y)
{
  if(Main_Game->Get_Currently_Moving_Player().Get_Gold() < 5)
  {
    Show_Not_Enough_Gold_Message();
    return;
  }
  if(! Main_Game->Has_Currently_Moving_Player_Any_Actions_Left())
  {
    Show_Not_Enough_Actions_Message();
    return;
  }
  string message = string(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Get_Name()) + " chopped!";
  Main_Game->Cut_Down_Tile(x,y);
  Deselect_Unit();
  Update_Map();
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Set_Progress_Bar_Label_Text(message);
}

void Game_Window::Update_Unit_Action_Buttons(int x, int y)
{
  auto button = Create_Pixlabel_Button("assets/textures/icons/apply-icon.svg.png", "Deselect Unit");
  button->signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Deselect_Unit));
  Action_Buttons_Box.pack_start(*button);
  button->show();
  auto button_2 = Create_Pixlabel_Button("assets/textures/icons/exit-icon.svg.png", "Disband Unit");
  button_2->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Disband_Unit), x, y));
  Action_Buttons_Box.pack_start(*button_2);
  button_2->show();
  if(!Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(selected_unit_x,selected_unit_y).Has_Full_HP())
  {
    if((Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(selected_unit_x,selected_unit_y).Get_All_Arguments_For_Trait("class")[0] == "flying" && !Main_Game->Get_Upgrade_Of_Currently_Moving_Player(Main_Game->Get_Map().Get_Upgrade(x, y)).Has_Trait("allowflyingunitsheal")))
      return;
    auto button_3 = Create_Pixlabel_Button("assets/textures/icons/minus-icon.svg.png", "Heal Unit");
    button_3->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Heal_Unit), selected_unit_x, selected_unit_y));
    Action_Buttons_Box.pack_start(*button_3);
    button_3->show();
  }
  if(Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(selected_unit_x,selected_unit_y).Has_Trait("plunder") && Main_Game->Get_Map().Can_Tile_Plundered(selected_unit_x,selected_unit_y) && !Main_Game->Get_Upgrade_Of_Currently_Moving_Player(Main_Game->Get_Map().Get_Upgrade(x, y)).Has_Trait("cannotbeplundered"))
  {
    auto button_4 = Create_Pixlabel_Button("assets/textures/icons/riots-icon.svg.png", "Plunder");
    button_4->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Plunder_Tile), selected_unit_x, selected_unit_y));
    Action_Buttons_Box.pack_start(*button_4);
    button_4->show();
  }
  if(Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(selected_unit_x,selected_unit_y).Has_Trait("atomic"))
  {
    auto button_5 = Create_Pixlabel_Button("assets/textures/icons/nuclear-icon.svg.png", "Detonate!");
    button_5->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Detonate_Atomic_Bomb), selected_unit_x, selected_unit_y));
    Action_Buttons_Box.pack_start(*button_5);
    button_5->show();
  }
}

void Game_Window::Show_Themed_Dialog(string_view message)
{
  Themed_Dialog Dialog(string(message), "Info");
  Dialog.Show();
}

void Game_Window::Show_Unit_Info_Dialog(Unit u)
{
  Unit_Info_Dialog Dialog(u);
  Dialog.Show();
}

void Game_Window::Show_Upgrade_Info_Dialog(Upgrade u)
{
  Upgrade_Info_Dialog Dialog(u);
  Dialog.Show();
}

Gtk::Button* Game_Window::Create_Pixlabel_Button(string label, string icon_path)
{
  auto *button = Gtk::make_managed<Gtk::Button>();
  Image_Path Icon_Path(label);
  button->add_pixlabel(Icon_Path.Get_File_Path().data(), icon_path);
  Main_Provider.Add_CSS(button);
  return button;
}

void Game_Window::Update_Tile_Action_Buttons(int x, int y)
{
  if(Main_Game->Get_Currently_Moving_Player().Has_Unit_On_Tile(x,y))
  {
    auto button = Create_Pixlabel_Button("assets/textures/icons/load-icon.svg.png", "Select Unit");
    button->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Select_Unit), x, y));
    Action_Buttons_Box.pack_start(*button);
    show_all_children();
  }
  if(!(Main_Game->Get_Map().Get_Owner(x,y) == Main_Game->Get_Currently_Moving_Player_Id() || (Main_Game->Get_Map().Get_Owner(x,y) == 0 && Main_Game->Get_Currently_Moving_Player().Has_Unit_On_Tile(x,y))))
    return;
  if(!Main_Game->Get_Map().Can_Tile_Plundered(x,y))
  {
    auto button = Create_Pixlabel_Button("assets/textures/icons/settings-icon.svg.png", "Fix Upgrade");
    button->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Fix_Tile), x,y));
    Action_Buttons_Box.pack_start(*button);
  }
  if(Main_Game->Get_Map().Get_Tile_Pointer(x,y).How_Many_Times_Has_Trait("removable") != 0 && (Main_Game->Get_Map().Is_Tile_Upgraded(x,y)))
  {
    auto button = Create_Pixlabel_Button("assets/textures/icons/construct-icon.svg.png", "Cut Down " + string(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Get_Name()));
    button->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Cut_Down), x,y));
    Action_Buttons_Box.pack_start(*button);
  }
  for(auto &upgrade : Main_Game->Get_Currently_Moving_Player().Get_Upgrades())
  {
    if(Main_Game->Get_Map().Get_Owner(x,y) == Main_Game->Get_Currently_Moving_Player_Id() && upgrade.How_Many_Times_Has_Trait("onlyneutral"))
      continue;
    if(Main_Game->Get_Currently_Moving_Player().Has_Tech_Been_Researched_By_Name(upgrade.Get_First_Requirement()) && upgrade.Is_Tile_Allowed_By_Name(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Get_Name()) && (Main_Game->Get_Map().Is_Tile_Upgraded(x,y)))
    {
      auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
      auto button_info = Create_Pixlabel_Button("assets/textures/icons/about-icon.svg.png", "Info");
      auto button = Create_Pixlabel_Button("assets/textures/icons/construct-icon.svg.png", "Build " + string(upgrade.Get_Name()));
      button->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Build_Upgrade_By_Name_On_Tile), upgrade.Get_Name().data(), x,y, Main_Game->Get_Currently_Moving_Player_Id()));
      button_info->signal_clicked().connect(sigc::bind<Upgrade>(sigc::mem_fun(*this, &Game_Window::Show_Upgrade_Info_Dialog), upgrade));
      Action_Buttons_Box.pack_start(*box);
      box->pack_start(*button);
      box->pack_start(*button_info);
    }
  }
  if(Main_Game->Get_Upgrade_Of_Currently_Moving_Player(Main_Game->Get_Map().Get_Upgrade(x,y)).Has_Trait("removable"))
  {
    auto button = Create_Pixlabel_Button("assets/textures/icons/exit-icon.svg.png", "Remove " + string(Main_Game->Get_Map().Get_Upgrade(x,y)));
    button->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Build_Upgrade_By_Name_On_Tile), "none" , x,y, Main_Game->Get_Currently_Moving_Player_Id()));
    Action_Buttons_Box.pack_start(*button);
  }
  if(Main_Game->Get_Upgrade_Of_Currently_Moving_Player(Main_Game->Get_Map().Get_Upgrade(x,y)).Has_Trait("recruit"))
  {
    vector<Unit> Units = Main_Game->Get_Currently_Moving_Player().Get_Units();
    vector<string> Classes = Main_Game->Get_Upgrade_Of_Currently_Moving_Player(Main_Game->Get_Map().Get_Upgrade(x,y)).Get_All_Arguments_For_Trait("recruit");
    for(auto &unit : Units)
    {
      if(!(Main_Game->Get_Map().Get_Tile_Pointer(x,y).Has_Unit()) && Main_Game->Get_Currently_Moving_Player().Has_Tech_Been_Researched_By_Name(unit.Get_First_Requirement()) && (find(Classes.begin(), Classes.end(), unit.Get_All_Arguments_For_Trait("class")[0]) != Classes.end()) && !(Main_Game->Get_Currently_Moving_Player().Is_Unit_Obsolete(unit.Get_Name())))
      {
        auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
        auto button_info = Create_Pixlabel_Button("assets/textures/icons/about-icon.svg.png", "Info");
        auto button = Create_Pixlabel_Button("assets/textures/icons/revolt-icon.svg.png" , "Recruit " + string(unit.Get_Name()));
        button->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Recruit_Unit), string(unit.Get_Name()) ,x,y));
        button_info->signal_clicked().connect(sigc::bind<Unit>(sigc::mem_fun(*this, &Game_Window::Show_Unit_Info_Dialog), unit));
        Action_Buttons_Box.pack_start(*box);
        box->pack_start(*button);
        box->pack_start(*button_info);
      }
    }
  }
  show_all_children();
}

void Game_Window::Update_Action_Buttons(int x, int y)
{
  if(Main_Game->Get_Currently_Moving_Player().Has_Tech_Been_Researched_By_Trait("unlockforeign"))
    Show_Civs_Button.set_sensitive(true);
  else
    Show_Civs_Button.set_sensitive(false);
  Clear_Action_Buttons();
  if(Main_Game->Get_Currently_Moving_Player().Get_Max_Actions() == Main_Game->Get_Currently_Moving_Player().Get_Current_Actions())
    Manage_Goverments_Button.set_sensitive(true);
  else
    Manage_Goverments_Button.set_sensitive(false);
  if(Is_Unit_Selected())
    Update_Unit_Action_Buttons(x,y);
  else
    Update_Tile_Action_Buttons(x,y);
}

void Game_Window::Update_Tile_Information_Label(int x, int y)
{
  array<int, 2> coords;
  coords[0] = x;
  coords[1] = y;
  string text = "Tile ";
  text = text + " X: ";
  text = text + to_string(coords[0]);
  text = text + " ";
  text = text + "Y: ";
  text = text + to_string(coords[1]);
  if(!Main_Game->Is_Currently_Moving_Player_AI())
    Set_Progress_Bar_Label_Text(text);
  text = text + "\n Type: " + string(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Name());
  text = text + "\n Upgrade: " + string(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Upgrade());
  if(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Upgrade() == "City")
  {
    text = text + "\n City Name: " + string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_City_Name_By_Coordinates(coords[0],coords[1]));
    text = text + "\n City Nationality: " + string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_City_By_Coordinates(coords).Get_Nationality());
    text = text + "\n City Stability: " + to_string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_City_By_Coordinates(coords).Get_Stability());
    text = text + "\n City Founder: " + string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_City_By_Coordinates(coords).Get_Founder_Name());
    text = text + "\n City Founding Date: " + string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_City_By_Coordinates(coords).Get_Founding_Date());
    if(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_City_By_Coordinates(coords).Is_Connected())
      text = text + "\n Connected To Other Cities";
    if(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_Capital_Location() == coords)
      text = text + "\n Capital";
  }
  if(Main_Game->Get_Map().Get_Owner(coords[0],coords[1]))
  {
    text = text + "\n Owner: " + string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(coords[0],coords[1])).Get_Name());
    text = text + " ID: " + to_string(Main_Game->Get_Map().Get_Owner(coords[0],coords[1]));
  }
  else
    text = text + "\n Owner: Neutral";

  if(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Has_Unit())
  {
    text = text + "\n Unit: " + string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Unit_Owner_Id()).Get_Unit_On_Tile(coords[0],coords[1]).Get_Name()) + " (" + string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Unit_Owner_Id()).Get_Name()) + " ID: " + to_string(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Unit_Owner_Id()) + ") ";
    text = text + "\n HP: " + to_string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Unit_Owner_Id()).Get_Unit_On_Tile(coords[0],coords[1]).Get_HP()) + " / 100";
    if(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Unit_Owner_Id() == Main_Game->Get_Currently_Moving_Player_Id())
    {
      text = text + "\n Actions " + to_string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Unit_Owner_Id()).Get_Unit_On_Tile(coords[0],coords[1]).Get_Current_Actions()) + " / " + to_string(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(coords[0],coords[1]).Get_Unit_Owner_Id()).Get_Unit_On_Tile(coords[0],coords[1]).Get_Max_Actions());
    }
  }
  else
  {
    text = text + "\n Unit: None";
  }
  Tile_Information_Label.set_text(text);
}

void Game_Window::Update_Tile_Flag()
{
  Reset_Tile_Flag_Label();
  if(Main_Game->Get_Map().Get_Tile_Pointer(last_clicked_x, last_clicked_y).Has_Unit())
  {
    Tile_Flag_Image.Change_Path(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Tile_Pointer(last_clicked_x,last_clicked_y).Get_Unit_Owner_Id()).Get_Texture_Path());
    return;
  }
  if(Main_Game->Get_Map().Get_Owner(last_clicked_x,last_clicked_y) != 0)
    Tile_Flag_Image.Change_Path(Main_Game->Get_Player_By_Id(Main_Game->Get_Map().Get_Owner(last_clicked_x,last_clicked_y)).Get_Texture_Path());
}

void Game_Window::Add_Combat_Overlay(array<int, 2> Coords)
{
  Map_Images->Add_Combat_Overlay({Coords[0], Coords[1]});
  Tiles_To_Update.push_back(Coords);
  Logger::Log_Info("Adding Combat Overlay at: " + to_string(Coords[0]) + " " + to_string(Coords[1]));
  if(!is_remove_combat_overlays_timeout_set)
  {
    Logger::Log_Info("Scheluding Combat Overlays Removal!");
    is_remove_combat_overlays_timeout_set = true;
    sigc::slot<bool> Remove_Combat_Overlays_Slot = sigc::mem_fun(*this, &Game_Window::Remove_Combat_Overlays);
    Remove_Combat_Overlays_Connection = Glib::signal_timeout().connect(Remove_Combat_Overlays_Slot, 1000);
  }
}

bool Game_Window::Remove_Combat_Overlays()
{
  Logger::Log_Info("Removing Combat Overlays...");
  Remove_Combat_Overlays_Connection.disconnect();
  is_remove_combat_overlays_timeout_set = false;
  for(auto Coord : Tiles_To_Update)
  {
    Update_Tile_By_Coords_Only(Coord[0], Coord[1]);
  }
  Tiles_To_Update.clear();
  return true;
}

void Game_Window::Focus_On_Capital(bool click_capital)
{
  Logger::Log_Info("Refocusing on Capital...");
  array<int, 2> Coords = Main_Game->Get_Currently_Moving_Player().Get_Capital_Location();
  if(Coords[0] > 9990)
    return;
  if(click_capital)
    Tile_Clicked(nullptr, {Coords[0], Coords[1]}, nullptr);
  else
    Map_Images->Add_Selection_Overlay(Coords);
  Coords[0] = Coords[0] * Map_Images->Get_Tile_Size();
  Coords[1] = Coords[1] * Map_Images->Get_Tile_Size();
  Logger::Log_Info("Setting to X: " + to_string(Coords[0]) + " Y: " + to_string(Coords[1]));
  Map_Scrolled_Window.get_hscrollbar()->set_value(Coords[0]);
  Map_Scrolled_Window.get_vscrollbar()->set_value(Coords[1]);
}

bool Game_Window::Tile_Clicked(GdkEventButton* tile_event, vector<int> coords, Gtk::Image *img)
{
  if(is_in_thread){return true;}
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  Update_Tile_Information_Label(coords[0],coords[1]);
  Update_Tile_Flag();
  if(Main_Game->Is_Currently_Moving_Player_AI())
    return true;
  Update_Action_Buttons(coords[0],coords[1]);
  if(Is_Unit_Selected())
  {
    string message = " ";
    vector<int> out = Main_Game->Get_Map().Check_If_Path_For_Unit_Exists(selected_unit_x, selected_unit_y, coords[0], coords[1], Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(selected_unit_x,selected_unit_y));
    if(out[0] == 1)
    {
      bool combat = Main_Game->Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(selected_unit_x, selected_unit_y, out[2], out[3], out[1], (bool) out[4], out[5], out[6]);
      Update_Labels();
      //Last_Clicked_Tile = img;
      last_clicked_x = coords[0];
      last_clicked_y = coords[1];
      Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
      Update_Tile_By_Coords_Only(selected_unit_x, selected_unit_y);
      Update_Tile_Information_Label(coords[0],coords[1]);
      Update_Tile_By_Coords_Only(out[2], out[3]);
      if(combat)
      {
        Add_Combat_Overlay({coords[0], coords[1]});
        Add_Combat_Overlay({selected_unit_x, selected_unit_y});
      }
      Deselect_Unit();
    }
    else
    {
      Sound_Manager::Play_Sound("assets/sounds/broken-audio.mp3");
      Set_Progress_Bar_Label_Text_Red("You can't move unit here!");
      Update_Tile_By_Coords_Only(selected_unit_x, selected_unit_y);
      Update_Tile_By_Coords_Only(coords[0], coords[1]);
    }
  }
  else
  {
    Sound_Manager::Play_Sound("assets/sounds/tileclicked-audio.mp3");
    last_clicked_x = coords[0];
    last_clicked_y = coords[1];
    Map_Images->Add_Selection_Overlay({coords[0], coords[1]});
  }
  Update_Action_Buttons(coords[0],coords[1]);
  Update_Tile_Flag();
  Update_Tiles_From_Game();
  return true;
}

void Game_Window::Show_Civs_Clicked()
{
  Civs_Dialog Dialog(Main_Game->Get_Players());
  Dialog.Show();
}

void Game_Window::Update_Economy_Label()
{
  string civ_name_text = Main_Game->Get_Currently_Moving_Player().Get_Full_Name() + " ID: " + to_string(Main_Game->Get_Currently_Moving_Player_Id());
  string economy_text = "Gold: " + to_string(Main_Game->Get_Currently_Moving_Player().Get_Gold());
  if(Main_Game->Get_Currently_Moving_Player().Get_Gold() == 0)
    economy_text = "<span foreground=\"red\">" + economy_text + "</span>";
  string actions_text = "Actions: " + to_string(Main_Game->Get_Currently_Moving_Player().Get_Current_Actions()) + "/" + to_string(Main_Game->Get_Currently_Moving_Player().Get_Max_Actions());
  if(Main_Game->Get_Currently_Moving_Player().Get_Current_Actions() == 0)
    actions_text = "<span foreground=\"red\">" + actions_text + "</span>";
  string tech_in_research_text = "Tech in research: " + string(Main_Game->Get_Currently_Moving_Player().Get_Currently_Researched_Tech().Get_Name()) + " " + to_string(Main_Game->Get_Currently_Moving_Player().Get_Currently_Researched_Tech().Get_Current_Cost()) + "/" + to_string(Main_Game->Get_Currently_Moving_Player().Get_Currently_Researched_Tech().Get_Cost());
  if(Main_Game->Get_Currently_Moving_Player().Get_Currently_Researched_Tech().Get_Current_Cost() <= 0)
    tech_in_research_text = "<span foreground=\"red\">" + tech_in_research_text + "</span>";
  string research_funds_text = "Research Fund: " + to_string(Main_Game->Get_Currently_Moving_Player().Get_Research_Percent()) + " %";
  if(Main_Game->Get_Currently_Moving_Player().Get_Research_Percent() == 0)
    research_funds_text = "<span foreground=\"red\">" + research_funds_text + "</span>";
  Economy_Label.set_markup(economy_text);
  Civ_Name_Label.set_text(civ_name_text);
  Actions_Label.set_markup(actions_text);
  Tech_In_Research_Label.set_markup(tech_in_research_text);
  Research_Funds_Label.set_markup(research_funds_text);
}

string Game_Window::Get_Current_Turn_By_Years()
{
  string out = Main_Game->Get_Current_Turn_By_Years();
  if(Main_Game->Get_Currently_Moving_Player().Has_Tech_Been_Researched_By_Trait("unlockdate"))
    return out;
  return "Unknown Year";
}

void Game_Window::Update_Capital_Label()
{
  string capital_label_text = "Your Capital is located at ";
  capital_label_text = capital_label_text + to_string(Main_Game->Get_Currently_Moving_Player().Get_Capital_Location()[0]);
  capital_label_text = capital_label_text + " ";
  capital_label_text = capital_label_text + to_string(Main_Game->Get_Currently_Moving_Player().Get_Capital_Location()[1]);
  Capital_Label.set_text(capital_label_text);
}

void Game_Window::Update_Map_Frame()
{
  Map_Frame.set_label("Map of " + Main_Game->Get_Currently_Moving_Player().Get_Full_Name() + " " + Get_Current_Turn_By_Years());
}

void Game_Window::Update_Labels()
{
  Update_Economy_Label();
  Update_Capital_Label();
  Update_Map_Frame();
  Update_Tile_Information_Label(last_clicked_x, last_clicked_y);
  Update_Tile_Flag();
}

void Game_Window::Player_Has_Lost_Game()
{
  Logger::Log_Info("Player has lost the game!");
  Themed_Dialog Dialog("You have lost and your civlization has been defeated!", "You have lost!");
  Dialog.Show();
  Logger::Log_Info("Closing Main Window...");
  Main_Manager->Show_Intro_Window(); //intro window
}

void Game_Window::Player_Has_Won_Game(int player_id)
{
  player_id = Main_Game->Get_Currently_Moving_Player_Id();
  string name = Main_Game->Get_Player_By_Id(player_id).Get_Full_Name();
  Logger::Log_Info(name + " has won the game!");
  Themed_Dialog Dialog(name + " has won the game and defeated all enemies!", "End the Game");
  Dialog.Show();
  Logger::Log_Info("Closing Main Window...");
  Main_Manager->Show_Intro_Window(); //intro window
}

void Game_Window::Disable_All_Buttons()
{
  is_in_thread = true;
  End_Turn_Button.set_sensitive(false);
//  Map_Update_Button.set_sensitive(false);
  Manage_Economy_Button.set_sensitive(false);
  Manage_Techs_Button.set_sensitive(false);
  Manage_Stability_Button.set_sensitive(false);
  Show_Civs_Button.set_sensitive(false);
  Manage_Goverments_Button.set_sensitive(false);
  Civ_Overview_Button.set_sensitive(false);
  Newspaper_Button.set_sensitive(false);
  Save_Button.set_sensitive(false);
  Load_Button.set_sensitive(false);
  Quit_Button.set_sensitive(false);
  Help_Button.set_sensitive(false);
  Random_Tip_Button.set_sensitive(false);
  //Tip_Button.set_sensitive(false);
}

void Game_Window::Enable_All_Buttons()
{
  is_in_thread = false;
  End_Turn_Button.set_sensitive(true);
//  Map_Update_Button.set_sensitive(true);
  Manage_Economy_Button.set_sensitive(true);
  Manage_Techs_Button.set_sensitive(true);
  Manage_Stability_Button.set_sensitive(true);
  if(Main_Game->Get_Currently_Moving_Player().Has_Tech_Been_Researched_By_Trait("unlockforeign"))
    Show_Civs_Button.set_sensitive(true);
  Manage_Goverments_Button.set_sensitive(true);
  Civ_Overview_Button.set_sensitive(true);
  Newspaper_Button.set_sensitive(true);
  Save_Button.set_sensitive(true);
  Load_Button.set_sensitive(true);
  Quit_Button.set_sensitive(true);
  Help_Button.set_sensitive(true);
  Random_Tip_Button.set_sensitive(true);
//  Tip_Button.set_sensitive(true);
}

void Game_Window::End_Turn()
{
  Deselect_Unit();
  Sound_Manager::Play_Sound("assets/sounds/endturn-audio.mp3");
  //Logger::Log_Info(Main_Game->Get_Currently_Moving_Player().Get_Possible_Research_Techs().size() );
  bool are_all_techs_researched = false;
  if(Main_Game->Get_Currently_Moving_Player().Get_Possible_Research_Techs().size() == 0)
  {
    are_all_techs_researched = true;
  }
  if(Main_Game->Get_Currently_Moving_Player().Get_Currently_Researched_Tech().Is_Reseached() && !are_all_techs_researched)
  {
    Set_Progress_Bar_Label_Text(string(Main_Game->Get_Currently_Moving_Player().Get_Currently_Researched_Tech().Get_Name()) + " is researched! Select other tech to complete turn!");
  }
  else
  {
    //here be dragons
    Thread_Portal_Pointer = make_shared<Magic_Thread_Communicator>(Main_Game, this);
    End_Turn_Thread = new std::thread(
      [this]
      {
        Thread_Portal_Pointer->Do_Task();
      });
    Disable_All_Buttons();
  }
}

void Game_Window::Manage_Goverments_Clicked()
{
  Goverment_Dialog Dialog(Main_Game->Get_Currently_Moving_Player());
  Dialog.Show();
  if(Main_Game->Get_Currently_Moving_Player().Get_Active_Goverment_Name() != Dialog.Get_Selected_Goverment())
    Main_Game->Change_Goverment_For_Currently_Moving_Player_By_Name(Dialog.Get_Selected_Goverment());
  Update_Labels();
}

void Game_Window::Manage_Techs_Clicked()
{
  Tech_Dialog Dialog(Main_Game->Get_Currently_Moving_Player());
  Dialog.Show();
  Main_Game->Get_Currently_Moving_Player().Set_Research_Tech_By_Name(Dialog.Get_Selected_Tech().Get_Name());
  Main_Game->Get_Currently_Moving_Player().Set_Research_Funds_Percentage(Dialog.Get_Research_Percent());
  Update_Labels();
}

void Game_Window::Manage_Overview_Clicked()
{
  Overview_Dialog Dialog(Main_Game->Get_Map().Get_Netto_Income_For_Player_By_Id(Main_Game->Get_Currently_Moving_Player_Id(), Main_Game->Get_Currently_Moving_Player()), Main_Game->Get_Currently_Moving_Player(), Main_Game->Get_Current_Turn_By_Years());
  Dialog.Show();
}

void Game_Window::Manage_Economy_Clicked()
{
  Economy_Dialog Dialog(Main_Game->Get_Map().Get_Netto_Income_For_Player_By_Id(Main_Game->Get_Currently_Moving_Player_Id(), Main_Game->Get_Currently_Moving_Player()), Main_Game->Get_Currently_Moving_Player());
  Dialog.Show();
}

void Game_Window::Select_Unit(int x, int y)
{
  Sound_Manager::Play_Sound("assets/sounds/unitselected-audio.mp3");
  is_unit_selected = true;
  selected_unit_x = x;
  selected_unit_y = y;
  Highlight_Tiles();
  Update_Action_Buttons(x,y);
  Set_Progress_Bar_Label_Text("Click on tile to move unit!");
}

void Game_Window::Highlight_Tiles()
{
  vector<tuple<array<int, 2>, int>> Tiles;
  //0 - Self
  //1 - Move
  //2 - Border Move
  //3 - Combat
  Tiles.push_back({{selected_unit_x, selected_unit_y}, 0});
  int unit_movement = Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(selected_unit_x, selected_unit_y).Get_Current_Actions();
  int upper_corner_left_x = selected_unit_x - unit_movement;
  int upper_corner_left_y = selected_unit_y - unit_movement;
  int down_corner_right_x = selected_unit_x + unit_movement;
  int down_corner_right_y = selected_unit_y + unit_movement;
  int tile_owner_id = Main_Game->Get_Map().Get_Owner(selected_unit_x, selected_unit_y);
  vector<string> Allowed_Tiles = Main_Game->Get_Currently_Moving_Player().Get_Unit_On_Tile(selected_unit_x, selected_unit_y).Get_Allowed_Tiles();
  clamp(upper_corner_left_x, 0, Main_Game->Get_Map().Get_X_Size());
  clamp(upper_corner_left_y, 0, Main_Game->Get_Map().Get_Y_Size());
  clamp(down_corner_right_x, 0, Main_Game->Get_Map().Get_X_Size());
  clamp(down_corner_right_y, 0, Main_Game->Get_Map().Get_Y_Size());
  int start = upper_corner_left_x;
  int start_y = upper_corner_left_y;
  while(start <= down_corner_right_x)
  {
    while(start_y <= down_corner_right_y)
    {
      if(start == selected_unit_x && start_y == selected_unit_y)
      {
        start_y++;
        continue;
      }
      if(find(Allowed_Tiles.begin(), Allowed_Tiles.end(), Main_Game->Get_Map().Get_Tile_Pointer(start, start_y).Get_Name().data()) != Allowed_Tiles.end())
      {
        int type = 1;
        if(Main_Game->Get_Map().Get_Owner(start, start_y) != tile_owner_id)
          type = 2;
        if(Main_Game->Get_Map().Get_Tile_Pointer(start, start_y).Has_Unit())
        {
          if(Main_Game->Get_Map().Get_Tile_Pointer(start, start_y).Get_Unit_Owner_Id() == Main_Game->Get_Currently_Moving_Player_Id())
          {
            start_y++;
            continue;
          }
          else
          {
            type = 3;
          }
        }
        Tiles.push_back({{start, start_y}, type});
        Tiles_To_Update.push_back({start, start_y});
      }
      start_y++;
    }
    start++;
    start_y = upper_corner_left_y;
  }
  Map_Images->Add_Unit_Move_Selection(Tiles);
}

void Game_Window::Deselect_Unit()
{
  Remove_Combat_Overlays();
  is_unit_selected = false;
  Update_Action_Buttons(selected_unit_x, selected_unit_y);
  selected_unit_x = 0;
  selected_unit_y = 0;
}

bool Game_Window::Is_Unit_Selected()
{
  return is_unit_selected;
}

void Game_Window::Save_Game()
{
  Save_Saver_Dialog Dialog;
  Dialog.Show();
  string path = Dialog.Get_File_Path();
  path = path + ".sav";
  bool return_value = Main_Game->Save_Game(path);
  if(return_value)
    ProgressBar_Label.set_text("Game saved to " + path);
  else
    Set_Progress_Bar_Label_Text_Red("Saving game failed!");
}

void Game_Window::Clear_Map_Images()
{
  Map_Images = make_shared<Gtk_Game_Map>(Main_Game->Get_Map().Get_X_Size(), Main_Game->Get_Map().Get_Y_Size(), Main_Settings_Manager.Get_Tile_Size_Value());
}

void Game_Window::Load_Game()
{
  Save_Loader_Dialog Dialog;
  Dialog.Show();
  string path = Dialog.Get_File_Path();
  if(path == " ")
    return;
  Logger::Log_Info("Path to load is " + path );
  tuple<bool, Game*> return_value = Main_Game->Load_Game(path);
  if(get<0>(return_value))
  {
    Set_Progress_Bar_Label_Text("Game loaded from " + path);
    delete Main_Game;
    Main_Game = get<1>(return_value);
    Main_Game->Set_Autosave(Main_Settings_Manager.Get_Autosave_Value());
    Clear_Map_Images();
    Map_Scrolled_Window.remove();
    Generate_Map_View();
    Update_Labels();
    Update_Action_Buttons(last_clicked_x, last_clicked_y);
    Update_Map();
  }
  else
  {
    Set_Progress_Bar_Label_Text_Red("Game loading failed");
  }
}

void Game_Window::Set_Progress_Bar_Label_Text(string text)
{
  ProgressBar_Label.set_text(text);
}

void Game_Window::Set_Progress_Bar_Label_Text_Red(string text)
{
  ProgressBar_Label.set_markup("<span foreground=\"red\">" + text + "</span>");
}


void Game_Window::Show_Newspaper_Clicked()
{
  Newspaper_Dialog Dialog(Main_Game->Get_Newspaper_Events(), Main_Game->Get_Currently_Moving_Player_Id());
  Dialog.Show();
}

void Game_Window::Exit_To_Main_Menu()
{
  Logger::Log_Info("Closing Game Window!");
  Main_Manager->Show_Intro_Window();
}

void Game_Window::Show_Intro_Message()
{
  Gtk::Dialog dialog("Start the game");
  Gtk::Box *Dialog_Box = dialog.get_content_area();
  dialog.add_button("Lets start!", 0);
  string message = "You lead the civilization of " + string(Main_Game->Get_Currently_Moving_Player().Get_Name()) + ". ";
  message = message + "\n Your civilization may colapse after few years or survive thousands. Who knows?";
  message = message + "\n Your civlization has following traits: ";
  Gtk::Box Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  vector<string> traits = Main_Game->Get_Currently_Moving_Player().Get_Trait_Names();
  Gtk::Label Dialog_Label = Gtk::Label(message);
  Root_Box.pack_start(Dialog_Label);
  Civ_Trait_Manager Trait_Manager;
  for(string &trait : traits)
  {
    auto* Trait_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    Trait_Box->pack_start((Trait_Manager.Get_Trait_Icon(trait)->Get_Gtk_Image()), Gtk::PACK_SHRINK);
    auto* Trait_Label = Gtk::make_managed<Gtk::Label>(string(Trait_Manager.Get_Trait_Full_Name(trait)) + " - " + string(Trait_Manager.Get_Trait_Full_Explanation(trait)));
    Trait_Box->pack_start(*Trait_Label);
    Root_Box.pack_start(*Trait_Box);
  }
  Dialog_Box->pack_start(Root_Box);
  Main_Provider.Add_CSS(&dialog);
  dialog.show_all_children();
  dialog.run();
}

void Game_Window::Show_Help_Message()
{
  Help_Dialog Dialog;
  Dialog.Show();
}

void Game_Window::Show_Random_Tip()
{
  Main_Tips_Manager.Show_Random_Tip();
}

void Game_Window::Show_Tip()
{
  Main_Tips_Manager.Show_Tip_In_Order();
}

void Game_Window::Notify_Game_Window_About_Turn()
{
  End_Turn_Dispatcher.emit();
}

void Game_Window::Check_Is_Game_Lost_Or_Won()
{
  if(!Main_Game->Get_Currently_Moving_Player().Get_Owned_Cities_Not_Pointer().size())
    Player_Has_Lost_Game();
  if(Main_Game->Is_Only_One_Player_Alive())
    Player_Has_Won_Game(0);
}

void Game_Window::Update_End_Turn_Labels()
{
  if(!Main_Game->Is_Currently_Moving_Player_AI())
  {
    Logger::Log_Info("Turn has finished!");
    Enable_All_Buttons();
    Update_Labels();
    Update_Action_Buttons(last_clicked_x, last_clicked_y);
    Update_Tiles_From_Game();
    Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
    Check_Is_Game_Lost_Or_Won();
    if(Main_Settings_Manager.Get_Autosave_Value())
      Main_Game->Save_Game("autosave.sav");
  }
  else
  {
    Set_Progress_Bar_Label_Text(string(Main_Game->Get_Currently_Moving_Player().Get_Name()) + " is currently moving...");
    Update_Tiles_From_Game();
  }
}

void Game_Window::Zoom_In()
{
  Main_Settings_Manager.Set_Tile_Size_Value(Main_Settings_Manager.Get_Tile_Size_Value() + 4);
  Map_Images->Zoom_In();
}

void Game_Window::Zoom_Out()
{
  if(Main_Settings_Manager.Get_Tile_Size_Value() > minimum_tile_size)
  {
    Main_Settings_Manager.Set_Tile_Size_Value(Main_Settings_Manager.Get_Tile_Size_Value() - 4);
    Map_Images->Zoom_Out();
  }
  else
  {
    Logger::Log_Warning("Tile size is already minimum! Not resizing");
  }
}

void Game_Window::Show_Tutorial()
{
  Logger::Log_Info("Game is launched first time! Showing tutorial...");
  Help_Manager Tutorial_Manager;
  Tutorial_Manager.Show_Basic_Tutorial();
}

bool Game_Window::Loop_Background_Music()
{
  Logger::Log_Info("Looping Background Theme...");
  if(!Main_Settings_Manager.Is_Music_Muted())
    Sound_Manager::Play_Sound(Main_Game->Get_Currently_Moving_Player().Get_Audio_Path());
  return true;
}

bool Game_Window::on_key_press_event(GdkEventKey* key_event)
{
  if(key_event->keyval == GDK_KEY_equal)
  {
    Logger::Log_Info("Pressed Plus");
    Zoom_In();
    return true;
  }
  if(key_event->keyval == GDK_KEY_minus)
  {
    Logger::Log_Info("Pressed Minus");
    Zoom_Out();
    return true;
  }
  if(key_event->keyval == GDK_KEY_Return)
  {
    Logger::Log_Info("Pressed Enter");
    if(End_Turn_Button.get_sensitive())
      End_Turn();
    return true;
  }
  if(key_event->keyval == GDK_KEY_space)
  {
    Logger::Log_Info("Pressed Space");
    Focus_On_Capital(!is_in_thread);
    return true;
  }
  if(key_event->keyval == GDK_KEY_Left)
  {
    Logger::Log_Info("Pressed Left Arrow");
    Map_Scrolled_Window.get_hscrollbar()->set_value(Map_Scrolled_Window.get_hscrollbar()->get_value() - Map_Images->Get_Tile_Size());
    return true;
  }
  if(key_event->keyval == GDK_KEY_Right)
  {
    Logger::Log_Info("Pressed Right Arrow");
    Map_Scrolled_Window.get_hscrollbar()->set_value(Map_Scrolled_Window.get_hscrollbar()->get_value() + Map_Images->Get_Tile_Size());
    return true;
  }
  if(key_event->keyval == GDK_KEY_Up)
  {
    Logger::Log_Info("Pressed Top Arrow");
    Map_Scrolled_Window.get_vscrollbar()->set_value(Map_Scrolled_Window.get_vscrollbar()->get_value() - Map_Images->Get_Tile_Size());
    return true;
  }
  if(key_event->keyval == GDK_KEY_Down)
  {
    Logger::Log_Info("Pressed Down Arrow");
    Map_Scrolled_Window.get_vscrollbar()->set_value(Map_Scrolled_Window.get_vscrollbar()->get_value() + Map_Images->Get_Tile_Size());
    return true;
  }
  if(key_event->keyval == GDK_KEY_t)
  {
    Logger::Log_Info("Pressed T");
    Show_Random_Tip();
    return true;
  }
  if(key_event->keyval == GDK_KEY_s)
  {
    Logger::Log_Info("Pressed S");
    if(Manage_Techs_Button.get_sensitive())
      Manage_Techs_Clicked();
    return true;
  }
  if(key_event->keyval == GDK_KEY_f)
  {
    Logger::Log_Info("Pressed F");
    if(Manage_Economy_Button.get_sensitive())
      Manage_Economy_Clicked();
    return true;
  }
  if(key_event->keyval == GDK_KEY_o)
  {
    Logger::Log_Info("Pressed O");
    if(Show_Civs_Button.get_sensitive())
      Show_Civs_Clicked();
    return true;
  }
  if(key_event->keyval == GDK_KEY_r)
  {
    Logger::Log_Info("Pressed R");
    if(Manage_Goverments_Button.get_sensitive())
      Manage_Goverments_Clicked();
    return true;
  }
  if(key_event->keyval == GDK_KEY_v)
  {
    Logger::Log_Info("Pressed V");
    if(Civ_Overview_Button.get_sensitive())
      Manage_Overview_Clicked();
    return true;
  }
  if(key_event->keyval == GDK_KEY_n)
  {
    Logger::Log_Info("Pressed N");
    if(Newspaper_Button.get_sensitive())
      Show_Newspaper_Clicked();
    return true;
  }

  return Gtk::Window::on_key_press_event(key_event);
}

void Game_Window::Manage_Stability_Clicked()
{
  Internal_Dialog Dialog(Main_Game->Get_Goverment_By_Name(Main_Game->Get_Currently_Moving_Player().Get_Active_Goverment_Name()), Main_Game->Get_Currently_Moving_Player().Get_Owned_Cities());
  Dialog.Show();
}

void Game_Window::Initialize_GTK()
{
  Last_Clicked_Tile = nullptr;
  is_delete_of_game_necessary = false;
  is_unit_selected = false;
  selected_unit_x = 0;
  selected_unit_y = 0;
  last_clicked_x = 0;
  last_clicked_y = 0;
  Map_Data.continents_number = 10;
  Map_Data.size_x = 50;
  Map_Data.size_y = 50;
  Map_Data.oceans_precentage = 50;
  other_players_player_count = 4;
  //set_size(600,600);
  set_title(Display_Version());
  ProgressBar_Label = Gtk::Label(" ");
  Tile_Information_Label = Gtk::Label(" ");
  Economy_Label = Gtk::Label(" ");
  Civ_Name_Label = Gtk::Label(" ");
  Actions_Label = Gtk::Label(" ");
  Research_Funds_Label = Gtk::Label(" ");
  Tech_In_Research_Label = Gtk::Label(" ");

  Info_Labels_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Economy_Label_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Civ_Name_Label_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Economy_Actions_Label_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Actions_Label_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Tech_In_Research_Label_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Research_Funds_Label_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);

  Civ_Name_Label_Icon = make_shared<Scaled_Gtk_Image>("assets/textures/icons/overview-icon.svg.png", 24,24);
  Economy_Label_Icon = make_shared<Scaled_Gtk_Image>("assets/textures/icons/traits/economic-icon.svg.png", 24,24);
  Actions_Label_Icon = make_shared<Scaled_Gtk_Image>("assets/textures/icons/play-icon.svg.png", 24,24);
  Research_Funds_Label_Icon = make_shared<Scaled_Gtk_Image>("assets/textures/icons/foreign-icon.svg.png", 24,24);
  Tech_In_Research_Label_Icon = make_shared<Scaled_Gtk_Image>("assets/textures/icons/science-icon.svg.png", 24,24);

  End_Turn_Button = Gtk::Button("End Turn");
  Zoom_Frame = Gtk::Frame("Zoom");
  Zoom_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Tile_Flag_Image.Change_Path(assets_directory_path + "textures/flags/neutral-flag.png");
  Reset_Tile_Flag_Label();
  Random_Tip_Button = Gtk::Button("Random Tip");
//  Tip_Button = Gtk::Button("Tip");
  Map_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  UI_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  End_Turn_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Map_Scrolled_Window.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_ALWAYS);
  add(Root_Box);
  Map_Frame = Gtk::Frame("Map of ");
  Action_Buttons_Frame = Gtk::Frame("Actions");
  Action_Buttons_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  UI_Frame = Gtk::Frame("Game");
  Capital_Label = Gtk::Label(" ");
  Root_Box.pack_start(Map_Root_Box);
  Root_Box.pack_start(UI_Frame, Gtk::PACK_SHRINK);
  UI_Frame.add(UI_Root_Box);
  string icon_directory = "assets/textures/icons/";
  Image_Path End_Turn_Icon_Path(icon_directory + "apply-icon.svg.png");
  End_Turn_Icon = make_shared<Scaled_Gtk_Image>(End_Turn_Icon_Path.Get_File_Path().data(), 24 ,24);
  End_Turn_Button.remove();
  End_Turn_Button.add_pixlabel(End_Turn_Icon_Path.Get_File_Path().data(), "End Turn");
  End_Turn_Box.pack_start(End_Turn_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Info_Labels_Root_Box, Gtk::PACK_SHRINK);
  Info_Labels_Root_Box.pack_start(Civ_Name_Label_Box);
  Civ_Name_Label_Box.pack_start(Civ_Name_Label);
  Civ_Name_Label_Box.pack_start((Civ_Name_Label_Icon->Get_Gtk_Image()));
  Civ_Name_Label_Icon->Get_Gtk_Image().set_halign(Gtk::ALIGN_START);
  Info_Labels_Root_Box.pack_start(Economy_Actions_Label_Box);
  Info_Labels_Root_Box.pack_start(Economy_Label_Box);
  Economy_Label_Box.pack_start(Economy_Label);
  Economy_Label.set_halign(Gtk::ALIGN_END);
  Economy_Label_Box.pack_start((Economy_Label_Icon->Get_Gtk_Image()));
  Economy_Label_Icon->Get_Gtk_Image().set_halign(Gtk::ALIGN_START);
  Info_Labels_Root_Box.pack_start(Actions_Label_Box);
  Actions_Label_Box.pack_start(Actions_Label);
  Actions_Label.set_halign(Gtk::ALIGN_END);
  Actions_Label_Box.pack_start((Actions_Label_Icon->Get_Gtk_Image()));
  Actions_Label_Icon->Get_Gtk_Image().set_halign(Gtk::ALIGN_START);
  Info_Labels_Root_Box.pack_start(Tech_In_Research_Label_Box);
  Tech_In_Research_Label_Box.pack_start(Tech_In_Research_Label);
  Tech_In_Research_Label.set_halign(Gtk::ALIGN_END);
  Tech_In_Research_Label_Box.pack_start((Tech_In_Research_Label_Icon->Get_Gtk_Image()));
  Tech_In_Research_Label_Icon->Get_Gtk_Image().set_halign(Gtk::ALIGN_START);
  Info_Labels_Root_Box.pack_start(Research_Funds_Label_Box);
  Research_Funds_Label_Box.pack_start(Research_Funds_Label);
  Research_Funds_Label.set_halign(Gtk::ALIGN_END);
  Research_Funds_Label_Box.pack_start((Research_Funds_Label_Icon->Get_Gtk_Image()));
  Research_Funds_Label_Icon->Get_Gtk_Image().set_halign(Gtk::ALIGN_START);
  UI_Root_Box.pack_start(Tile_Information_Label, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start((Tile_Flag_Image.Get_Gtk_Image()), Gtk::PACK_SHRINK);
  Manage_Techs_Button.Change_Icon(icon_directory + "science-icon.svg.png");
  UI_Root_Box.pack_start(Manage_Techs_Button, Gtk::PACK_SHRINK);
  Manage_Economy_Button.Change_Icon(icon_directory + "economy-icon.svg.png");
  UI_Root_Box.pack_start(Manage_Economy_Button, Gtk::PACK_SHRINK);
  Show_Civs_Button.Change_Icon(icon_directory + "foreign-icon.svg.png");
  UI_Root_Box.pack_start(Show_Civs_Button, Gtk::PACK_SHRINK);
  Manage_Stability_Button.Change_Icon(icon_directory + "internal-icon.svg.png");
  UI_Root_Box.pack_start(Manage_Stability_Button, Gtk::PACK_SHRINK);
  Manage_Goverments_Button.Change_Icon(icon_directory + "revolution-icon.svg.png");
  UI_Root_Box.pack_start(Manage_Goverments_Button, Gtk::PACK_SHRINK);
  Civ_Overview_Button.Change_Icon(icon_directory + "overview-icon.svg.png");
  UI_Root_Box.pack_start(Civ_Overview_Button, Gtk::PACK_SHRINK);
  Newspaper_Button.Change_Icon(icon_directory + "newspaper-icon.svg.png");
  UI_Root_Box.pack_start(Newspaper_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Action_Buttons_Frame, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Zoom_Frame, Gtk::PACK_SHRINK);
  Zoom_Frame.add(Zoom_Box);
  Zoom_In_Button.Change_Icon(icon_directory + "plus-icon.svg.png");
  Zoom_Out_Button.Change_Icon(icon_directory + "minus-icon.svg.png");
  Zoom_Box.pack_start(Zoom_In_Button);
  Zoom_Box.pack_start(Zoom_Out_Button);
  Action_Buttons_Frame.add(Action_Buttons_Box);
  UI_Root_Box.pack_start(End_Turn_Box, Gtk::PACK_SHRINK);
  Save_Button.Change_Icon(icon_directory + "load-icon.svg.png");
  UI_Root_Box.pack_start(Save_Button, Gtk::PACK_SHRINK);
  Load_Button.Change_Icon(icon_directory + "load-icon.svg.png");
  UI_Root_Box.pack_start(Load_Button, Gtk::PACK_SHRINK);
  Quit_Button.Change_Icon(icon_directory + "exit-icon.svg.png");
  UI_Root_Box.pack_start(Quit_Button, Gtk::PACK_SHRINK);
  Image_Path Tip_Icon_Path(icon_directory + "about-icon.svg.png");
  Tip_Icon = make_shared<Scaled_Gtk_Image>(Tip_Icon_Path.Get_File_Path().data(), 24 ,24);
  Random_Tip_Button.remove();
  Random_Tip_Button.add_pixlabel(Tip_Icon_Path.Get_File_Path().data(), "Random Tip");
  UI_Root_Box.pack_start(Random_Tip_Button, Gtk::PACK_SHRINK);
  Help_Button.Change_Icon(icon_directory + "about-icon.svg.png");
  UI_Root_Box.pack_start(Help_Button, Gtk::PACK_SHRINK);
  Map_Root_Box.pack_start(Map_Frame);
  Map_Root_Box.pack_start(ProgressBar_Label, Gtk::PACK_SHRINK);
  Map_Root_Box.pack_start(Capital_Label, Gtk::PACK_SHRINK);
  Map_Frame.add(Map_Scrolled_Window);

  Show_Civs_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Show_Civs_Clicked));
  End_Turn_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::End_Turn));
  Manage_Techs_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Manage_Techs_Clicked));
  Manage_Goverments_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Manage_Goverments_Clicked));
  Manage_Economy_Button.signal_clicked().connect(sigc::mem_fun(*this,&Game_Window::Manage_Economy_Clicked));
  Civ_Overview_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Manage_Overview_Clicked) );
  Save_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Save_Game) );
  Load_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Load_Game) );
  Newspaper_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Show_Newspaper_Clicked) );
  Quit_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Exit_To_Main_Menu) );
  Help_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Show_Help_Message) );
  Random_Tip_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Show_Random_Tip) );
  End_Turn_Dispatcher.connect(sigc::mem_fun(*this, &Game_Window::Update_End_Turn_Labels));
  Zoom_In_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Zoom_In));
  Zoom_Out_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Zoom_Out));
  Manage_Stability_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Manage_Stability_Clicked));
  Main_Provider.Add_CSS(this);
  Main_Provider.Add_CSS(&End_Turn_Button);
  Main_Provider.Add_CSS(&Random_Tip_Button);
  Set_Tiles_Size_Automatically();
  Map_Images = make_shared<Gtk_Game_Map>(Main_Game->Get_Map().Get_X_Size(), Main_Game->Get_Map().Get_Y_Size(), Main_Settings_Manager.Get_Tile_Size_Value());
  Generate_Map_View();
  show_all_children();
  set_default_size(800,800);
  maximize();
  Update_Map();
  Update_Labels();
  add_events(Gdk::KEY_PRESS_MASK);
  Show_Intro_Message();
  if(Main_Game->Get_Currently_Moving_Player().Has_Tech_Been_Researched_By_Trait("unlockforeign"))
    Show_Civs_Button.set_sensitive(true);
  else
    Show_Civs_Button.set_sensitive(false);
  Clear_Action_Buttons();
  if(Main_Game->Get_Currently_Moving_Player().Get_Max_Actions() == Main_Game->Get_Currently_Moving_Player().Get_Current_Actions())
    Manage_Goverments_Button.set_sensitive(true);
  else
    Manage_Goverments_Button.set_sensitive(false);
  if(Main_Settings_Manager.Get_Random_Tip_On_Startup_Value())
    Main_Tips_Manager.Show_Random_Tip();
//Map_Update_Button.hide();
//  Tip_Button.hide();
  if(Main_Settings_Manager.Check_If_Game_Is_Launched_First_Time())
    Show_Tutorial();
  Main_Settings_Manager.Launch_Game_First_Time();
  Loop_Background_Music();
  sigc::slot<bool> Background_Music_Loop_Slot = sigc::mem_fun(*this, &Game_Window::Loop_Background_Music);
// This is where we connect the slot to the Glib::signal_timeout()
  Background_Music_Loop_Connection = Glib::signal_timeout().connect(Background_Music_Loop_Slot, 60000);
}

void Game_Window::Set_Tiles_Size_Automatically()
{
  array<int, 2> Resolution = Get_Screen_Resolution();
  Logger::Log_Info("Screen resolution is " + to_string(Resolution[0]) + "x" + to_string(Resolution[1]));
  int tiles_in_x = Main_Game->Get_Map().Get_X_Size() * Main_Settings_Manager.Get_Tile_Size_Value();
  int tiles_in_y = Main_Game->Get_Map().Get_Y_Size() * Main_Settings_Manager.Get_Tile_Size_Value();
  int new_tile_size = Resolution[0] / Main_Game->Get_Map().Get_X_Size();
  minimum_tile_size = new_tile_size;
  if(tiles_in_x < Resolution[0] || tiles_in_y < Resolution[1])
  {
    Logger::Log_Warning("Map may not be big enough to fit on screen...");
    if(Main_Settings_Manager.Get_Autoresize_Tiles_Value())
    {
      Logger::Log_Info("Resizing tile size to " + to_string(new_tile_size));
      Main_Settings_Manager.Set_Tile_Size_Value(new_tile_size);
    }
  }
  //Focus_On_Capital(true);
}

Game_Window::Game_Window(Window_Manager *m_m, Settings_Manager m_s_m, Map_Generator_Data Map_Data, vector<tuple<string, bool>> players, bool load_starting_positions, bool spectator_mode) : End_Turn_Thread(nullptr), Tile_Flag_Image(128, 64), Show_Civs_Button("Foregin Ministry"), Manage_Techs_Button("Science Ministry"), Civ_Overview_Button("Overview"), Manage_Economy_Button("Finance Ministry"), Newspaper_Button("Newspaper"), Manage_Goverments_Button("Revolution"), Save_Button("Save Game"), Load_Button("Load Game"), Zoom_In_Button("Zoom In"), Zoom_Out_Button("Zoom Out"), Help_Button("Help"), Quit_Button("Exit to Main Menu"), Manage_Stability_Button("Internal Ministry")
{
  Logger::Log_Info("Showing Game Window...");
  Main_Manager = m_m;
  Main_Settings_Manager = m_s_m;
  Main_Game = new Game(Main_Settings_Manager.Get_Autosave_Value(), Map_Data, players, load_starting_positions, spectator_mode);
  Initialize_GTK();
}

array<int ,2> Game_Window::Get_Screen_Resolution()
{
  array<int, 2> out;
  Glib::RefPtr<Gdk::Screen> Main_Screen = this->get_screen();
  out[0] = Main_Screen->get_width();
  out[1] = Main_Screen->get_height();
  return out;
}

Game_Window::Game_Window(Window_Manager *m_m, Settings_Manager m_s_m, string path = " ", bool spectator_mode = false) : Root_Box(Gtk::ORIENTATION_HORIZONTAL,2), End_Turn_Thread(nullptr), Tile_Flag_Image(128, 64), Map_Generation_Thread(nullptr), Show_Civs_Button("Foregin Ministry"), Manage_Techs_Button("Science Ministry"), Civ_Overview_Button("Overview"), Manage_Economy_Button("Finance Ministry"), Newspaper_Button("Newspaper"), Manage_Goverments_Button("Revolution"), Save_Button("Save Game"), Load_Button("Load Game"), Zoom_In_Button("Zoom In"), Zoom_Out_Button("Zoom Out"), Help_Button("Help"), Quit_Button("Exit to Main Menu"), Manage_Stability_Button("Internal Ministry")
{
  Logger::Log_Info("Showing Game Window...");
  Main_Manager = m_m;
  Main_Settings_Manager = m_s_m;
  if(path != " ")
  {
    xml_document<> doc;
    ifstream file (path);
    if(file.is_open())
    {
      vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
      buffer.push_back('\0');
      file.close();
      try
      {
        doc.parse<0>(&buffer[0]);
        xml_node<> *Root_Node = doc.first_node()->first_node();
        Main_Game = new Game(Root_Node);
        Main_Game->Set_Autosave(Main_Settings_Manager.Get_Autosave_Value());
        Logger::Log_Info("Loading Game Successfull!");
        Initialize_GTK();
      }
      catch(...)
      {
        Logger::Log_Error("Loading XML Game Data Failed...");
        Logger::Log_Warning("Moving back to Intro Window!");
        path = " ";
      }
    }
    else
    {
      Logger::Log_Error("Opening file " + path + " failed!");
      Logger::Log_Warning("Moving back to Intro Window!");
      path = " ";
    }
  }
  if(path == " ")
  {
    Show_Themed_Dialog("Loading Save File failed!");
    Main_Manager->Loading_Save_File_Failed();
    Main_Manager->Show_Intro_Window();
  }
}
