#include "game_window.h"

Game_Window::~Game_Window()
{
  //gtkmm guide
}

void Game_Window::Generate_Map_View()
{
  Logger::Log_Info("Generating Map View..." );
  int x = Main_Game.Get_Map()->Get_X_Size();
  int y = Main_Game.Get_Map()->Get_Y_Size();
  int start = 0;
  int start_y = 0;
  //Logger::Log_Info("Map Size is " + x + " " + y );
  auto *root = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 0);
  Map_Scrolled_Window.add(*root);
  root->show();
  while(start < x)
  {
    auto *tmp = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL, 0);
    root->pack_start(*tmp,  Gtk::PACK_SHRINK);
    tmp->show();
    Map_Images.push_back(tmp);
    while(start_y < y)
    {
      auto *event_box = Gtk::make_managed<Gtk::EventBox>();
      auto *i = Gtk::make_managed<Gtk::Image>(assets_directory_path + "textures" + path_delimeter + "tiles" + path_delimeter + "land-tile-texture.png");
      tmp->pack_start(*event_box, Gtk::PACK_SHRINK);
      event_box->add(*i);
      event_box->set_events(Gdk::BUTTON_PRESS_MASK);
      vector<int> coords;
      coords.push_back(start);
      coords.push_back(start_y);
      event_box->signal_button_press_event().connect(sigc::bind<vector<int>>(sigc::mem_fun(*this, &Game_Window::Tile_Clicked), coords, i));
      event_box->show();
      i->show();
      i->set_hexpand(true);
      i->set_vexpand(true);
      start_y++;
    }
    start_y = 0;
    start++;
  }

  show_all_children();
  Logger::Log_Info("Map View Generated!" );
}

void Game_Window::Update_Tile(Gtk::Image *tile_image, int x, int y)
{
  vector<string> textures = Main_Game.Get_Map()->Get_Tile(x,y).Get_Textures_Path(); //this is incredibly slow pls fix
  Glib::RefPtr<Gdk::Pixbuf> tile_pix; //guide
  Glib::RefPtr<Gdk::Pixbuf> upgrade_pix;
  Glib::RefPtr<Gdk::Pixbuf> finished_pix;
  Glib::RefPtr<Gdk::Pixbuf> border_pix;
  Glib::RefPtr<Gdk::Pixbuf> unit_pix;
  Glib::RefPtr<Gdk::Pixbuf> scaled_pix;
  int tile_size = 32;
  int true_tile_size = Main_Settings_Manager.Get_Tile_Size_Value();
  scaled_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, true_tile_size, true_tile_size);
  //fetching unit texture from tile.Get_Textures_Path() is currently not supported
  if(Main_Game.Get_Map()->Get_Tile(x,y).Has_Unit())
  {
    unit_pix = Gdk::Pixbuf::create_from_file(Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Tile(x,y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(x,y).Get_Texture_Path());
  }
  else
  {
    unit_pix = Gdk::Pixbuf::create_from_file(assets_directory_path + "textures" + path_delimeter + "upgrades" + path_delimeter + "none-upgrade-texture.png");
  }
  tile_pix = Gdk::Pixbuf::create_from_file(textures[0]);
  //Logger::Log_Info(textures[0] );
  upgrade_pix = Gdk::Pixbuf::create_from_file(textures[1]);
  finished_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  border_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tile_size, tile_size);
  int border_alpha = 60;
  border_alpha = 120;
  border_pix->fill(Main_Game.Get_Border_Color_By_Player_Id(Main_Game.Get_Map()->Get_Owner(x, y)));
  tile_pix->copy_area(0,0,32,32,finished_pix,0,0);
  upgrade_pix->composite(finished_pix,0,0,32,32,0,0,1,1,Gdk::INTERP_BILINEAR,255);
  unit_pix->composite(finished_pix,0,0,32,32,0,0,1,1,Gdk::INTERP_BILINEAR,255);
  border_pix->composite(finished_pix,0,0,32,32,0.0,0.0,1.0,1.0,Gdk::INTERP_BILINEAR,border_alpha);
  finished_pix->scale(scaled_pix, 0, 0, true_tile_size, true_tile_size, 0, 0, ((double) true_tile_size / (double) tile_size), ((double) true_tile_size / (double) tile_size), Gdk::INTERP_BILINEAR);
  tile_image->set(scaled_pix);
  tile_image->set_size_request(true_tile_size, true_tile_size);
}

void Game_Window::Update_Tile_By_Coords_Only(int x, int y)
{
  int start = 0;
  int start_y = 0;
  for(auto &box : Map_Images)
  {
    if(start == x)
    {
      auto children = box->get_children();
      for(auto &event_box : children)
      {
        Gtk::EventBox *e = dynamic_cast<Gtk::EventBox*>(event_box);
        auto event_cont = e->get_children();
        for(auto &image : event_cont)
        {
          if(start_y == y)
          {
            Gtk::Image *i = dynamic_cast<Gtk::Image*>(image);
            Update_Tile(i,start,start_y);
          }
          start_y++;
        }
      }
    }
    start++;
    start_y = 0;
  }
}

void Game_Window::Update_Map()
{
  unsigned long long tiles = Main_Game.Get_Map()->Get_X_Size() * Main_Game.Get_Map()->Get_Y_Size();
  auto timer_start = chrono::steady_clock::now();
  Logger::Log_Info("Updating Map..." );
  int start = 0;
  int start_y = 0;
  for(auto &box : Map_Images)
  {
    auto children = box->get_children();
    for(auto &event_box : children)
    {
      Gtk::EventBox *e = dynamic_cast<Gtk::EventBox*>(event_box);
      auto event_cont = e->get_children();
      for(auto &image : event_cont)
      {
        Gtk::Image *i = dynamic_cast<Gtk::Image*>(image);
        Update_Tile(i,start,start_y);
        start_y++;
      }
    }
    start++;
    start_y = 0;
  }
  Logger::Log_Info("Map Updated!" );
  auto timer_end = chrono::steady_clock::now();
  auto timer_diff = timer_end - timer_start;
  Logger::Log_Info("Update took: " + to_string(chrono::duration<double, milli>(timer_diff).count()) + " ms" );
  string text = "Map Updated!\n";
  text = text + "Update took: ";
  text = text + to_string(chrono::duration<double, milli>(timer_diff).count());
  text = text + " ms (";
  text = text + to_string(tiles);
  text = text + " tiles)";
  ProgressBar_Label.set_text(text);
}

void Game_Window::Test()
{
  Update_Map();
}

void Game_Window::Show_Not_Enough_Actions_Message()
{
  string message = "You don't have enough actions to do that!";
  ProgressBar_Label.set_text(message);
}

void Game_Window::Show_Not_Enough_Gold_Message()
{
  string message = "You don't have enough gold to do that!";
  ProgressBar_Label.set_text(message);
}

void Game_Window::Build_Upgrade_By_Name_On_Tile(string upg_name, int x, int y, int owner)
{
  int upg_costs = Main_Game.Get_Upgrade_By_Name(upg_name).Get_Cost();
  bool upg_avoid = Main_Game.Get_Upgrade_By_Name(upg_name).Does_Avoid_Same_Type_Upgrades();
  //we assume that tile owner is valid
  if(Main_Game.Get_Player_By_Id(owner)->Get_Gold() < upg_costs)
  {
    Show_Not_Enough_Gold_Message();
    return;
  }
  if(upg_avoid && Main_Game.Get_Map()->Is_Upgrade_In_Radius_By_Name(upg_name,x,y))
  {
    string message = " You can't build " + upg_name + " here there is other " + upg_name + " close!";
    ProgressBar_Label.set_text(message);
    return;
  }
  if(! Main_Game.Has_Currently_Moving_Player_Any_Actions_Left())
  {
    Show_Not_Enough_Actions_Message();
    return;
  }

    string message = upg_name + " built sucessfully!";
    int radius = Main_Game.Get_Player_By_Id(owner)->Get_Upgrade_Border_Radius();
    if(upg_name == "City" && Main_Game.Get_Player_By_Id(owner)->Get_Active_Goverment_Name() == "Theocracy")
    {
      upg_costs = upg_costs - 10;
    }
    if(upg_name == "Farm" && Main_Game.Get_Player_By_Id(owner)->Get_Active_Goverment_Name() == "Monarchy")
    {
      upg_costs = upg_costs + 2;
    }
    Main_Game.Get_Player_By_Id(owner)->Build_Upgrade(upg_costs);
    if(upg_name == "City")
    {
      Main_Game.Build_City(x,y,owner, radius);
      Update_Map();
    }
    else
    {
      Main_Game.Get_Map()->Build_Upgrade(Main_Game.Get_Upgrade_By_Name(upg_name), x, y, owner, radius);
    }

  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  //Update_Tile(Last_Clicked_Tile, last_clicked_x, last_clicked_y);
  Update_Tile_By_Coords_Only(last_clicked_x, last_clicked_y);
  ProgressBar_Label.set_text(message);
  //Update_Tile()
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

void Game_Window::Recruit_Unit(string u, int x, int y)
{
  string message = " ";
  if(Main_Game.Get_Currently_Moving_Player()->Get_Gold() < Main_Game.Get_Unit_By_Name(u).Get_Cost())
  {
    Show_Not_Enough_Gold_Message();
    return;
  }
  if(! Main_Game.Has_Currently_Moving_Player_Any_Actions_Left())
  {
    Show_Not_Enough_Actions_Message();
    return;
  }
  Main_Game.Recruit_Unit(u, x, y);
  message = "Unit " + u + " recruited!";
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile(Last_Clicked_Tile, last_clicked_x, last_clicked_y);
  ProgressBar_Label.set_text(message);
}

void Game_Window::Disband_Unit(int x, int y)
{
  string message = " ";
  if(! Main_Game.Has_Currently_Moving_Player_Any_Actions_Left())
  {
    Show_Not_Enough_Actions_Message();
    return;
  }
  if(Main_Game.Get_Currently_Moving_Player()->Has_Unit_On_Tile(x,y))
  {
    Main_Game.Get_Map()->Get_Tile_Pointer(x,y)->Remove_Unit_From_Tile();
    Main_Game.Get_Currently_Moving_Player()->Remove_Unit_By_Coords(x,y);
    Deselect_Unit();
    message = "Unit disbanded!";
  }
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile(Last_Clicked_Tile, last_clicked_x, last_clicked_y);
  ProgressBar_Label.set_text(message);
}

void Game_Window::Heal_Unit(int x, int y)
{
  string message = " ";
  if(Main_Game.Get_Currently_Moving_Player()->Has_Unit_On_Tile(x,y))
  {
    if(Main_Game.Get_Currently_Moving_Player()->Get_Unit_On_Tile(x,y).Get_Current_Actions() > 1)
    {
      Main_Game.Get_Currently_Moving_Player()->Get_Unit_On_Tile_Pointer(x,y)->Heal();
      message = "Unit healed!";
    }
    else
    {
      message = message + "\n Your unit doesn't have enough actions to do that!";
    }
  }
  Update_Labels();
  Update_Action_Buttons(last_clicked_x, last_clicked_y);
  Update_Tile(Last_Clicked_Tile, last_clicked_x, last_clicked_y);
  ProgressBar_Label.set_text(message);
}

void Game_Window::Update_Unit_Action_Buttons(int x, int y)
{
  auto *button = Gtk::make_managed<Gtk::Button>("Deselect Unit");
  button->signal_clicked().connect(sigc::mem_fun(*this, &Game_Window::Deselect_Unit));
  Action_Buttons_Box.pack_start(*button);
  Main_Provider.Add_CSS(button);
  button->show();
  auto *button_2 = Gtk::make_managed<Gtk::Button>("Disband Unit");
  button_2->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Disband_Unit), x, y));
  Action_Buttons_Box.pack_start(*button_2);
  Main_Provider.Add_CSS(button_2);
  button_2->show();
  if(!Main_Game.Get_Currently_Moving_Player()->Get_Unit_On_Tile(selected_unit_x,selected_unit_y).Has_Full_HP())
  {
    auto *button_3 = Gtk::make_managed<Gtk::Button>("Heal Unit");
    button_3->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Heal_Unit), selected_unit_x, selected_unit_y));
    Action_Buttons_Box.pack_start(*button_3);
    Main_Provider.Add_CSS(button_3);
    button_3->show();
  }
}

void Game_Window::Show_Themed_Dialog(string message)
{
  Gtk::Dialog dialog("Info");
  dialog.add_button("Ok", 0);
  Gtk::Label Dialog_Label = Gtk::Label(message);
  Gtk::Box *Dialog_Box = dialog.get_content_area();
  Dialog_Box->pack_start(Dialog_Label);
  dialog.show_all_children();
  Main_Provider.Add_CSS(&dialog);
  dialog.run();
}

void Game_Window::Update_Tile_Action_Buttons(int x, int y)
{
  if(Main_Game.Get_Currently_Moving_Player()->Has_Unit_On_Tile(x,y))
  {
    auto *button = Gtk::make_managed<Gtk::Button>("Select Unit");
    button->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Window::Select_Unit), x, y));
    Action_Buttons_Box.pack_start(*button);
    Main_Provider.Add_CSS(button);
    button->show();
  }
  if(Main_Game.Get_Map()->Get_Owner(x,y) == Main_Game.Get_Currently_Moving_Player_Id() || (Main_Game.Get_Map()->Get_Owner(x,y) == 0 && Main_Game.Get_Currently_Moving_Player()->Has_Unit_On_Tile(x,y)))
  {
    for(auto &upgrade : Main_Game.Get_Upgrades())
    {
      if(Main_Game.Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Name(upgrade.Get_First_Requirement()) && upgrade.Is_Tile_Allowed_By_Name(Main_Game.Get_Map()->Get_Tile(x,y).Get_Name()) && (Main_Game.Get_Map()->Get_Tile(x,y).Get_Upgrade().Get_Cost() == 0))
      {
        auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
        string message = "Upgrade " + upgrade.Get_Name();
        message = message + "\n Cost " + to_string(upgrade.Get_Cost());
        message = message + "\n Production " + to_string(upgrade.Get_Production());
        message = message + "\n Maintenace " + to_string(upgrade.Get_Maitenance());
        auto *button_info = Gtk::make_managed<Gtk::Button>("?");
        auto *button = Gtk::make_managed<Gtk::Button>("Build " + upgrade.Get_Name());
        button->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Build_Upgrade_By_Name_On_Tile), upgrade.Get_Name(), x,y, Main_Game.Get_Currently_Moving_Player_Id()));
        button_info->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Show_Themed_Dialog), message));
        Action_Buttons_Box.pack_start(*box);
        box->pack_start(*button);
        box->pack_start(*button_info);
        box->show();
        button_info->show();
        Main_Provider.Add_CSS(button_info);
        Main_Provider.Add_CSS(button);
        button->show();
      }
    }
    if(Main_Game.Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Trait("remove_upgrades") && Main_Game.Get_Map()->Get_Tile(x,y).Get_Upgrade().Has_Trait("removable"))
    {
      auto *button = Gtk::make_managed<Gtk::Button>("Remove " + Main_Game.Get_Map()->Get_Tile(x,y).Get_Upgrade().Get_Name());
      button->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Build_Upgrade_By_Name_On_Tile), "none" , x,y, Main_Game.Get_Currently_Moving_Player_Id()));
      Action_Buttons_Box.pack_start(*button);
      Main_Provider.Add_CSS(button);
      button->show();
    }
    if(Main_Game.Get_Map()->Get_Tile(x,y).Get_Upgrade().Has_Trait("recruit"))
    {
      vector<Unit> Units = Main_Game.Get_Currently_Moving_Player()->Get_Units();
      for(auto &unit : Units)
      {
        if( !(Main_Game.Get_Map()->Get_Tile(x,y).Has_Unit()) && Main_Game.Get_Currently_Moving_Player()->Has_Tech_Been_Researched_By_Name(unit.Get_First_Requirement()) && unit.Can_Move_On_Tile_By_Name(Main_Game.Get_Map()->Get_Tile(x,y).Get_Name()))
        {
          auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
          string message = "Recruit " + unit.Get_Name();
          message = message + "\n Cost " + to_string(unit.Get_Cost());
          message = message + "\n Maintenace " + to_string(unit.Get_Maitenance());
          message = message + "\n Attack Power " + to_string(unit.Get_Attack_Power());
          message = message + "\n Defense Power " + to_string(unit.Get_Defense_Power());
          message = message + "\n Movement Points " + to_string(unit.Get_Current_Actions());
          auto *button_info = Gtk::make_managed<Gtk::Button>("?");
          auto *button = Gtk::make_managed<Gtk::Button>("Recruit " + unit.Get_Name());
          button->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Recruit_Unit),unit.Get_Name() ,x,y));
          button_info->signal_clicked().connect(sigc::bind<string>(sigc::mem_fun(*this, &Game_Window::Show_Themed_Dialog), message));
          Action_Buttons_Box.pack_start(*box);
          box->pack_start(*button);
          box->pack_start(*button_info);
          box->show();
          button_info->show();
          Main_Provider.Add_CSS(button_info);
          Main_Provider.Add_CSS(button);
          button->show();
        }
      }
    }
  }
}

void Game_Window::Update_Action_Buttons(int x, int y)
{
  Clear_Action_Buttons();
  if(Main_Game.Get_Currently_Moving_Player()->Get_Max_Actions() == Main_Game.Get_Currently_Moving_Player()->Get_Current_Actions())
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
  vector<int> coords;
  coords.push_back(x);
  coords.push_back(y);
  string text = "Tile ";
  text = text + " x: ";
  text = text + to_string(coords[0]);
  text = text + " ";
  text = text + "y: ";
  text = text + to_string(coords[1]);
  ProgressBar_Label.set_text(text);
  text = text + "\n Type: " + Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Name();
  text = text + "\n Upgrade: " + Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Upgrade().Get_Name();
  if(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Upgrade().Get_Name() == "City")
  {
    text = text + "\n City Name: " + Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Owner(coords[0],coords[1]))->Get_City_Name_By_Coordinates(coords[0],coords[1]);
  }
  if(Main_Game.Get_Map()->Get_Owner(coords[0],coords[1]))
  {
    text = text + "\n Owner: " + Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Owner(coords[0],coords[1]))->Get_Name();
    text = text + " ID: " + to_string(Main_Game.Get_Map()->Get_Owner(coords[0],coords[1]));
  }
  else
    text = text + "\n Owner: Neutral";

  if(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Has_Unit())
  {
    text = text + "\n Unit: " + Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Unit_Owner_Id())->Get_Unit_On_Tile(coords[0],coords[1]).Get_Name() + " (" + Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Unit_Owner_Id())->Get_Name() + " ID: " + to_string(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Unit_Owner_Id()) + ") ";
    text = text + "\n HP: " + to_string(Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Unit_Owner_Id())->Get_Unit_On_Tile(coords[0],coords[1]).Get_HP()) + " / 100";
    if(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Unit_Owner_Id() == Main_Game.Get_Currently_Moving_Player_Id())
    {
      text = text + "\n Actions " + to_string(Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Unit_Owner_Id())->Get_Unit_On_Tile(coords[0],coords[1]).Get_Current_Actions()) + " / " + to_string(Main_Game.Get_Player_By_Id(Main_Game.Get_Map()->Get_Tile(coords[0],coords[1]).Get_Unit_Owner_Id())->Get_Unit_On_Tile(coords[0],coords[1]).Get_Max_Actions());
    }
  }
  else
  {
    text = text + "\n Unit: None";
  }
  Tile_Information_Label.set_text(text);
}

bool Game_Window::Tile_Clicked(GdkEventButton* tile_event, vector<int> coords, Gtk::Image *img)
{
  if(Last_Clicked_Tile != nullptr)
  {
    Update_Tile(Last_Clicked_Tile, last_clicked_x, last_clicked_y);
  }
  Update_Tile_Information_Label(coords[0],coords[1]);
  Update_Action_Buttons(coords[0],coords[1]);
  if(Is_Unit_Selected())
  {
    string message = " ";
    vector<int> out = Main_Game.Get_Map()->Check_If_Path_For_Unit_Exists(selected_unit_x, selected_unit_y, coords[0], coords[1], Main_Game.Get_Currently_Moving_Player()->Get_Unit_On_Tile(selected_unit_x,selected_unit_y));
    if(out[0] == 1)
    {
      bool update_map = Main_Game.Move_Unit_And_Attack_If_Necessary_Or_Take_Cities(selected_unit_x, selected_unit_y, out[2], out[3], out[1], (bool) out[4], out[5], out[6]);
      if(update_map)
        Update_Map();
      Update_Labels();
      Last_Clicked_Tile = img;
      last_clicked_x = coords[0];
      last_clicked_y = coords[1];
      Update_Tile(Last_Clicked_Tile, last_clicked_x, last_clicked_y);
      Update_Tile_By_Coords_Only(selected_unit_x, selected_unit_y);
      Update_Tile_Information_Label(coords[0],coords[1]);
      Update_Tile_By_Coords_Only(out[2], out[3]);
      Deselect_Unit();
    }
    else
    {
      ProgressBar_Label.set_text("You can't move unit here!");
      Update_Tile_By_Coords_Only(selected_unit_x, selected_unit_y);
      Update_Tile(img, coords[0], coords[1]);
    }
  }
  else
  {
    Last_Clicked_Tile = img;
    last_clicked_x = coords[0];
    last_clicked_y = coords[1];
    Glib::RefPtr<Gdk::Pixbuf> tile_image = img->get_pixbuf();
    Glib::RefPtr<Gdk::Pixbuf> selection_texture;
    Glib::RefPtr<Gdk::Pixbuf> scaled_pix;
    int tile_size = 32;//Main_Settings_Manager.Get_Tile_Size_Value();
    selection_texture = Gdk::Pixbuf::create_from_file(assets_directory_path + "textures" + path_delimeter + "other" + path_delimeter + "selection-texture.png");
    int true_tile_size = Main_Settings_Manager.Get_Tile_Size_Value();
    scaled_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, true_tile_size, true_tile_size);
    selection_texture->scale(scaled_pix, 0, 0, true_tile_size, true_tile_size, 0, 0, ((double) true_tile_size / (double) tile_size), ((double) true_tile_size / (double) tile_size), Gdk::INTERP_BILINEAR);
    scaled_pix->composite(tile_image,0,0,true_tile_size,true_tile_size,0,0,1,1,Gdk::INTERP_BILINEAR,255);
    img->set(tile_image);
  }
  Update_Action_Buttons(coords[0],coords[1]);
  return true;
}

void Game_Window::Show_Civs_Clicked()
{
  Gtk::Dialog Civs_Dialog("Foregin Ministry");
  Gtk::Box *Dialog_Box = Civs_Dialog.get_content_area();
  Gtk::Frame Dialog_Root_Frame("Civilizations");
  Gtk::ScrolledWindow Dialog_Scrolled_Window;
  Dialog_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window.set_min_content_height(300);
  Gtk::Box Civs_List_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  int end = Main_Game.Get_Amount_Of_Players() + 1;
  int start = 1;
  Dialog_Box->pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Dialog_Scrolled_Window);
  Dialog_Scrolled_Window.add(Civs_List_Box);
  Dialog_Root_Frame.show();
  Dialog_Scrolled_Window.show();
  Civs_List_Box.show();
  while(start < end)
  {
    auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto *image = Gtk::make_managed<Gtk::Image>();
    auto *label = Gtk::make_managed<Gtk::Label>(Main_Game.Get_Player_By_Id(start)->Get_Full_Name() + " (" + Main_Game.Get_Player_By_Id(start)->Get_Leader_Name() + ") " + " ID: " + to_string(start) + " Points: " + to_string(Main_Game.Get_Player_By_Id(start)->Get_Score()) + " Army Size: " + to_string(Main_Game.Get_Player_By_Id(start)->Get_Army_Manpower()) + " Population: " + to_string(Main_Game.Get_Player_By_Id(start)->Get_Population()));
    Glib::RefPtr<Gdk::Pixbuf> color_pix;
    label->show();
    color_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 32, 32);
    color_pix->fill(Main_Game.Get_Border_Color_By_Player_Id(start));
    image->set(color_pix);
    box->pack_start(*image);
    box->pack_start(*label);
    box->show();
    image->show();
    Main_Provider.Add_CSS(box);
    Main_Provider.Add_CSS(image);
    Main_Provider.Add_CSS(label);
    Civs_List_Box.pack_start(*box);
    start++;
  }
  Main_Provider.Add_CSS(&Civs_Dialog);
  Civs_Dialog.run();
  //return true;
}

void Game_Window::Update_Economy_Label()
{
  string text = Main_Game.Get_Currently_Moving_Player()->Get_Full_Name() + " ID: " + to_string(Main_Game.Get_Currently_Moving_Player_Id());
  text = text + "\n Gold: " + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Gold()) + " Actions: " + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Current_Actions()) + "/" + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Max_Actions());
  text = text + "\n Tech in research: " + Main_Game.Get_Currently_Moving_Player()->Get_Currently_Researched_Tech()->Get_Name() + " " + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Currently_Researched_Tech()->Get_Current_Cost()) + "/" + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Currently_Researched_Tech()->Get_Cost());
  text = text + "\n Research Fund: " + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Research_Percent()) + " %";
  Economy_Label.set_text(text);
}

void Game_Window::Update_Capital_Label()
{
  string capital_label_text = "Your Capital is located at ";
  capital_label_text = capital_label_text + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Capital_Location()[0]);
  capital_label_text = capital_label_text + " ";
  capital_label_text = capital_label_text + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Capital_Location()[1]);
  Capital_Label.set_text(capital_label_text);
}

void Game_Window::Update_Map_Frame()
{
  Map_Frame.set_label("Map of " + Main_Game.Get_Currently_Moving_Player()->Get_Full_Name() + " " + Main_Game.Get_Current_Turn_By_Years());
}

void Game_Window::Update_Labels()
{
  Update_Economy_Label();
  Update_Capital_Label();
  Update_Map_Frame();
  Update_Tile_Information_Label(last_clicked_x, last_clicked_y);
}

void Game_Window::Player_Has_Lost_Game()
{
  Logger::Log_Info("Player has lost the game!");
  Gtk::Dialog dialog("Player has lost the game!");
  dialog.add_button("Ok", 0);
  string message = "You have lost and your civilization has been defeated!";
  Gtk::Label Dialog_Label = Gtk::Label(message);
  Gtk::Box *Dialog_Box = dialog.get_content_area();
  Dialog_Box->pack_start(Dialog_Label);
  dialog.show_all_children();
  Main_Provider.Add_CSS(&dialog);
  dialog.run();
  Logger::Log_Info("Closing Main Window...");
  Main_Manager->Show_Intro_Window(); //intro window
}

void Game_Window::End_Turn()
{
  Deselect_Unit();
  //Logger::Log_Info(Main_Game.Get_Currently_Moving_Player()->Get_Possible_Research_Techs().size() );
  bool are_all_techs_researched = false;
  if(Main_Game.Get_Currently_Moving_Player()->Get_Possible_Research_Techs().size() == 0)
  {
    are_all_techs_researched = true;
  }
  if(Main_Game.Get_Currently_Moving_Player()->Get_Currently_Researched_Tech()->Is_Reseached() && !are_all_techs_researched)
  {
    ProgressBar_Label.set_text(Main_Game.Get_Currently_Moving_Player()->Get_Currently_Researched_Tech()->Get_Name() + " is researched! Select other tech to complete turn!");
  }
  else
  {
    auto timer_start = chrono::steady_clock::now();
    bool out = Main_Game.End_Player_Turn();
    Logger::Log_Info("Turn finished!" );
    auto timer_end = chrono::steady_clock::now();
    auto timer_diff = timer_end - timer_start;
    Logger::Log_Info("Turn took: " + to_string(chrono::duration<double, milli>(timer_diff).count()) + " ms" );
    if(!out)
      Player_Has_Lost_Game();
    if(Main_Game.Is_Map_Update_Required())
      Update_Map();
    vector<Coords> tiles_to_update = Main_Game.Get_Tiles_To_Update();
    for_each(tiles_to_update.begin(), tiles_to_update.end(), [this](Coords& var){Update_Tile_By_Coords_Only(var.x, var.y);});
    Update_Labels();
    Update_Action_Buttons(last_clicked_x, last_clicked_y);
    Update_Map();

  }
}

void Game_Window::Change_Technology_Goal(Tech new_goal)
{
  Main_Game.Get_Currently_Moving_Player()->Set_Research_Tech_By_Name(new_goal.Get_Name());
  Update_Labels();
}

void Game_Window::Set_Research_Funds_Percentage(Gtk::SpinButton *spin)
{
  Main_Game.Get_Currently_Moving_Player()->Set_Research_Funds_Percentage((int)spin->get_value());
  Update_Labels();
}

void Game_Window::Change_Goverment(Gov new_gov)
{
  Main_Game.Change_Goverment_For_Currently_Moving_Player_By_Name(new_gov.Get_Name());
  Update_Labels();
}

void Game_Window::Manage_Goverments_Clicked()
{
  Gtk::Dialog Gov_Dialog("Change Goverment");
  Gtk::Box *Dialog_Box = Gov_Dialog.get_content_area();
  Gtk::Box Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Gtk::Frame Dialog_Root_Frame("Unlocked Goverments");
  Gtk::Box Gov_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  vector<Gov> govs = Main_Game.Get_Currently_Moving_Player()->Get_Possible_Goverments();
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Gov_Box);
  Dialog_Root_Frame.show();
  Gov_Box.show();
  if(govs.size() == 1)
  {
    auto *label = Gtk::make_managed<Gtk::Label>("Research technologies to get access to different goverments");
    label->show();
    Main_Provider.Add_CSS(label);
    Gov_Box.pack_start(*label);
  }
  for(auto &gov : govs)
  {
    if(gov.Get_Name() != Main_Game.Get_Currently_Moving_Player()->Get_Active_Goverment_Name())
    {
      auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
      auto *image = Gtk::make_managed<Gtk::Image>(gov.Get_Texture_Path());
      auto *button = Gtk::make_managed<Gtk::Button>("Revolt to " + gov.Get_Name());
      Gov_Box.pack_start(*box);
      box->pack_start(*image);
      box->pack_start(*button);
      box->show();
      image->show();
      button->show();
      auto *label = Gtk::make_managed<Gtk::Label>(gov.Info());
      box->pack_start(*label);
      label->show();
      Main_Provider.Add_CSS(box);
      Main_Provider.Add_CSS(image);
      Main_Provider.Add_CSS(button);
      Main_Provider.Add_CSS(label);
      button->signal_clicked().connect(sigc::bind<Gov>(sigc::mem_fun(*this, &Game_Window::Change_Goverment), gov ));
    }
  }
  Root_Box.show();
  Main_Provider.Add_CSS(&Gov_Dialog);
  Gov_Dialog.run();
}

void Game_Window::Manage_Techs_Clicked()
{
  Gtk::Dialog Research_Dialog("Science Ministry");
  Research_Dialog.add_button("Apply", 0);
  Gtk::Box *Dialog_Box = Research_Dialog.get_content_area();
  Gtk::Box Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Gtk::Frame Dialog_Root_Frame("Set Research Goal");
  //Gtk::ScrolledWindow Dialog_Scrolled_Window;
  //Dialog_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
  Gtk::Box Research_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  vector<Tech> techs = Main_Game.Get_Currently_Moving_Player()->Get_Possible_Research_Techs();
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Research_Box);
  Dialog_Root_Frame.show();
  Research_Box.show();
  for(auto &tech : techs)
  {
    auto *box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto *image = Gtk::make_managed<Gtk::Image>(tech.Get_Texture_Path());
    auto *button = Gtk::make_managed<Gtk::Button>("Research " + tech.Get_Name());
    auto *label = Gtk::make_managed<Gtk::Label>(tech.Info());
    Research_Box.pack_start(*box);
    box->pack_start(*image);
    box->pack_start(*button);
    box->pack_start(*label);
    box->show();
    label->show();
    image->show();
    button->show();
    Main_Provider.Add_CSS(box);
    Main_Provider.Add_CSS(image);
    Main_Provider.Add_CSS(button);
    Main_Provider.Add_CSS(label);
    button->signal_clicked().connect(sigc::bind<Tech>(sigc::mem_fun(*this, &Game_Window::Change_Technology_Goal), tech ));
  }
  Glib::RefPtr<Gtk::Adjustment> Research_Adjustment;
  Research_Adjustment = Gtk::Adjustment::create((double) Main_Game.Get_Currently_Moving_Player()->Get_Research_Percent() ,0.0,100.0,1.0,10,0.0);
  Gtk::SpinButton Research_Percent_Switch(Research_Adjustment);
  Gtk::Label info("Set Research Funds Percent:");
  //Gtk::Button Research_Percent_Button("Accept Funds");
  Root_Box.show();
  Root_Box.pack_start(info);
  Root_Box.pack_start(Research_Percent_Switch);
  //Root_Box.pack_start(Research_Percent_Button);
  info.show();
  Research_Percent_Switch.show();
  //Research_Percent_Button.show();
  Main_Provider.Add_CSS(&Research_Dialog);
  Research_Percent_Switch.signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Game_Window::Set_Research_Funds_Percentage), &Research_Percent_Switch ));
  Research_Dialog.run();
  Main_Game.Get_Currently_Moving_Player()->Set_Research_Funds_Percentage(static_cast<int>(Research_Percent_Switch.get_value()));
}

void Game_Window::Manage_Overview_Clicked()
{
  Gtk::Dialog Overview_Dialog("Goverment Report");
  Gtk::Box *Dialog_Box = Overview_Dialog.get_content_area();
  Gtk::Box Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Gtk::Frame Dialog_Root_Frame("Statistical Yearbook for " + Main_Game.Get_Current_Turn_By_Years());
  Gtk::Box Stats_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Gtk::Label Stats_Label;
  string text;
  text = Main_Game.Get_Currently_Moving_Player()->Get_Full_Name();
  text = text + "\n Leader: " + Main_Game.Get_Currently_Moving_Player()->Get_Leader_Name();
  text = text + "\n Capital: " + Main_Game.Get_Currently_Moving_Player()->Get_Capital_Name();
  text = text + "\n Goverment: " + Main_Game.Get_Currently_Moving_Player()->Get_Active_Goverment_Name();
  text = text + "\n Population: " + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Population());
  text = text + "\n Army Size: " + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Army_Manpower());
  vector<int> money = Main_Game.Get_Map()->Get_Netto_Income_For_Player_By_Id(Main_Game.Get_Currently_Moving_Player_Id(), *Main_Game.Get_Currently_Moving_Player());
  double gnppc = money[0] * 1000000;
  gnppc = gnppc / (double) Main_Game.Get_Currently_Moving_Player()->Get_Population();
  text = text + "\n GNP per capita " + to_string(gnppc) + " $";
  Stats_Label = Gtk::Label(text);
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Stats_Box);
  Stats_Box.pack_start(Stats_Label);
  Stats_Label.show();
  Dialog_Root_Frame.show();
  Stats_Box.show();
  Root_Box.show();
  Main_Provider.Add_CSS(&Overview_Dialog);
  Overview_Dialog.run();
}

void Game_Window::Manage_Economy_Clicked()
{
  Gtk::Dialog Finance_Dialog("Finance Ministry");
  Gtk::Box *Dialog_Box = Finance_Dialog.get_content_area();
  Gtk::Box Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Gtk::Frame Dialog_Root_Frame("Economy of " + Main_Game.Get_Currently_Moving_Player()->Get_Full_Name());
  Gtk::Box Finance_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Gtk::Label Finance_Label;
  string text;
  vector<int> money = Main_Game.Get_Map()->Get_Netto_Income_For_Player_By_Id(Main_Game.Get_Currently_Moving_Player_Id(), *Main_Game.Get_Currently_Moving_Player());
  text = " Brutto Income " + to_string(money[0]);
  text = text + "\n Tile Maitenance -" + to_string(money[1]);
  text = text + "\n Unit Maitenance -" + to_string(Main_Game.Get_Currently_Moving_Player()->Get_Unit_Maitenance());
  text = text + "\n Research Funds -" + to_string(money[0] * ((double) Main_Game.Get_Currently_Moving_Player()->Get_Research_Percent() / 100.0 ));
  Finance_Label = Gtk::Label(text);
  Dialog_Box->pack_start(Root_Box);
  Root_Box.pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Finance_Box);
  Finance_Box.pack_start(Finance_Label);
  Finance_Label.show();
  Dialog_Root_Frame.show();
  Finance_Box.show();
  Root_Box.show();
  Main_Provider.Add_CSS(&Finance_Dialog);
  Finance_Dialog.run();
}

void Game_Window::Select_Unit(int x, int y)
{
  is_unit_selected = true;
  selected_unit_x = x;
  selected_unit_y = y;
  Update_Action_Buttons(x,y);
}

void Game_Window::Deselect_Unit()
{
  is_unit_selected = false;
  Update_Action_Buttons(selected_unit_x, selected_unit_y);
  selected_unit_x = 0;
  selected_unit_y = 0;
}

bool Game_Window::Is_Unit_Selected()
{
  return is_unit_selected;
}

void Game_Window::Change_Map_Data_X_Value(Gtk::SpinButton *spin)
{
  Map_Data.size_x = (int)spin->get_value();
}

void Game_Window::Change_Map_Data_Y_Value(Gtk::SpinButton *spin)
{
  Map_Data.size_y = (int)spin->get_value();
}

void Game_Window::Change_Map_Data_Continents_Value(Gtk::SpinButton *spin)
{
  Map_Data.continents_number = (int)spin->get_value();
}

void Game_Window::Change_Map_Data_Water_Value(Gtk::SpinButton *spin)
{
  Map_Data.oceans_precentage = (int)spin->get_value();
}

void Game_Window::Change_Player_Count_Value(Gtk::SpinButton *spin)
{
  other_players_player_count = (int)spin->get_value();
}

void Game_Window::Change_Main_Player_Civ(Gtk::Label *info_label, Gtk::Image *civ_color, Gtk::ComboBoxText *combo)
{
  vector<Civ> civs = Main_Game.Get_All_Civs();
  Civ *selection;
  for(auto &var : civs)
    if(var.Get_Raw_Name() == combo->get_active_text())
      selection = &var;
  Glib::RefPtr<Gdk::Pixbuf> color_pix;
  main_player_civ_name = combo->get_active_text();
  color_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 64, 64);
  color_pix->fill(selection->Get_Civ_Color());
  civ_color->set(color_pix);
  info_label->set_text(selection->Info());
}

void Game_Window::Show_Game_Creation_Dialog()
{
  Gtk::Dialog Game_Gen_Dialog("Create New Game");
  Main_Provider.Add_CSS(&Game_Gen_Dialog);
  Game_Gen_Dialog.add_button("Generate Map", 0);
  Gtk::Box *Dialog_Box = Game_Gen_Dialog.get_content_area();
  Gtk::Frame Dialog_Map_Frame("World Settings");
  Gtk::Box Dialog_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Gtk::Box Map_UI_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Gtk::Frame Dialog_Players_Frame("Select Players");
  Gtk::Box Players_UI_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Gtk::Box Main_Player_UI_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Gtk::ComboBoxText Civs_Chooser_Combo;
  Gtk::Image Civs_Color_Image;
  Gtk::Frame Civs_Description_Frame = Gtk::Frame("Civ Description");
  Gtk::Label Civs_Description_Label = Gtk::Label(" ");
  Gtk::CheckButton Allow_Duplicate_Civs_Button = Gtk::CheckButton("Allow Duplicate Civilizations Existing In The Game");

  Gtk::Label Other_Players_Label = Gtk::Label("Choose how many other players will participate in game:");
  Glib::RefPtr<Gtk::Adjustment> Players_Adjustment;
  Players_Adjustment = Gtk::Adjustment::create(5.0,0.0,1000.0,1.0,10,0.0);
  Gtk::SpinButton Players_Switch(Players_Adjustment);

  Gtk::Label X_Label = Gtk::Label("Map Creation Preferences\nSet Map Size\n X Size:");
  Glib::RefPtr<Gtk::Adjustment> X_Adjustment;
  X_Adjustment = Gtk::Adjustment::create(50.0,0.0,1000.0,1.0,10,0.0);
  Gtk::SpinButton X_Switch(X_Adjustment);

  Gtk::Label Y_Label = Gtk::Label("Y Size:");
  Glib::RefPtr<Gtk::Adjustment> Y_Adjustment;
  Y_Adjustment = Gtk::Adjustment::create(50.0,0.0,1000.0,1.0,10,0.0);
  Gtk::SpinButton Y_Switch(Y_Adjustment);

  Gtk::Label Continents_Label = Gtk::Label("Amount of Continents (To create interesting maps select values around 50-60):");
  Glib::RefPtr<Gtk::Adjustment> Continents_Adjustment;
  Continents_Adjustment = Gtk::Adjustment::create(10.0,0.0,1000.0,1.0,10,0.0);
  Gtk::SpinButton Continents_Switch(Continents_Adjustment);

  Gtk::Label Water_Label = Gtk::Label("Amount of Water (To create balanced maps choose values around 60-70 to create landfilled pangea-like maps choose values higher than 500):");
  Glib::RefPtr<Gtk::Adjustment> Water_Adjustment;
  Water_Adjustment = Gtk::Adjustment::create(50.0,0.0,1000.0,1.0,10,0.0);
  Gtk::SpinButton Water_Switch(Water_Adjustment);



  vector<Civ> civs = Main_Game.Get_All_Civs();
  for(auto &civ : civs)
    Civs_Chooser_Combo.append(civ.Get_Raw_Name());

  Dialog_Box->pack_start(Dialog_Root_Box);
  Civs_Chooser_Combo.set_active(true);
  Dialog_Root_Box.pack_start(Dialog_Map_Frame);
  Dialog_Map_Frame.add(Map_UI_Box);
  Map_UI_Box.pack_start(X_Label);
  X_Label.show();
  Map_UI_Box.pack_start(X_Switch);
  X_Switch.show();
  Map_UI_Box.pack_start(Y_Label);
  Y_Label.show();
  Map_UI_Box.pack_start(Y_Switch);
  Y_Switch.show();
  Map_UI_Box.pack_start(Continents_Label);
  Continents_Label.show();
  Map_UI_Box.pack_start(Continents_Switch);
  Continents_Switch.show();
  Map_UI_Box.pack_start(Water_Label);
  Water_Label.show();
  Map_UI_Box.pack_start(Water_Switch);
  Water_Switch.show();
  Dialog_Root_Box.pack_start(Dialog_Players_Frame);
  Dialog_Players_Frame.show();
  Dialog_Players_Frame.add(Players_UI_Box);
  Players_UI_Box.show();
  Players_UI_Box.pack_start(Main_Player_UI_Box);
  Main_Player_UI_Box.show();
  Main_Player_UI_Box.pack_start(Civs_Color_Image);
  Civs_Color_Image.show();
  Main_Player_UI_Box.pack_start(Civs_Chooser_Combo);
  Civs_Chooser_Combo.show();
  Main_Player_UI_Box.pack_start(Civs_Description_Frame);
  Civs_Description_Frame.show();
  Civs_Description_Frame.add(Civs_Description_Label);
  Civs_Description_Label.show();
  Players_UI_Box.pack_start(Other_Players_Label);
  Other_Players_Label.show();
  Players_UI_Box.pack_start(Players_Switch);
  Players_Switch.show();
  Players_UI_Box.pack_start(Allow_Duplicate_Civs_Button);
  Allow_Duplicate_Civs_Button.show();
  Map_UI_Box.show();
  Dialog_Root_Box.show();
  Dialog_Map_Frame.show();

  X_Switch.signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Game_Window::Change_Map_Data_X_Value), &X_Switch ));
  Y_Switch.signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Game_Window::Change_Map_Data_Y_Value), &Y_Switch ));
  Continents_Switch.signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Game_Window::Change_Map_Data_Continents_Value), &Continents_Switch ));
  Water_Switch.signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Game_Window::Change_Map_Data_Water_Value), &Water_Switch ));
  Players_Switch.signal_value_changed().connect(sigc::bind<Gtk::SpinButton*>(sigc::mem_fun(*this, &Game_Window::Change_Player_Count_Value), &Players_Switch ));
  Civs_Chooser_Combo.signal_changed().connect(sigc::bind<Gtk::Label*>(sigc::mem_fun(*this, &Game_Window::Change_Main_Player_Civ), &Civs_Description_Label, &Civs_Color_Image, &Civs_Chooser_Combo ));

  main_player_civ_name = Civs_Chooser_Combo.get_active_text();
  Change_Main_Player_Civ(&Civs_Description_Label, &Civs_Color_Image, &Civs_Chooser_Combo);
  Game_Gen_Dialog.run();
  Logger::Log_Info("Dialog has finished!" );
  Logger::Log_Info("Generating Game..." );
  Logger::Log_Info("Passing Map Data!" );
  Main_Game.Generate_Map(Map_Data);
  Logger::Log_Info("Map Generated!" );
  Logger::Log_Info("Adding players..." );
  int player_civ_id = 0;
  for(auto &civ : civs)
  {
    if(civ.Get_Raw_Name() == main_player_civ_name)
    {
      break;
    }
    player_civ_id++;
  }
  Logger::Log_Info("Player choose " + main_player_civ_name + " " + to_string(player_civ_id) );
  Main_Game.Add_Players(player_civ_id, other_players_player_count, (bool) Allow_Duplicate_Civs_Button.get_active());
  Logger::Log_Info("Setting Starting Postitions..." );
  Main_Game.Assign_Starting_Positions();
  Logger::Log_Info("Starting Positions Set!" );
  Logger::Log_Info("Game Generated!" );
}

void Game_Window::Save_Game()
{
  Gtk::FileChooserDialog Save_Game_File_Chooser_Dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
  Save_Game_File_Chooser_Dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  Save_Game_File_Chooser_Dialog.add_button("_Open", Gtk::RESPONSE_OK);
  int result = Save_Game_File_Chooser_Dialog.run();

 //Handle the response:
 switch(result)
 {
   case(Gtk::RESPONSE_OK):
   {
     string path = Save_Game_File_Chooser_Dialog.get_filename();
     Logger::Log_Info("Path to save is " + path );
     bool return_value = Main_Game.Save_Game(path);
     if(return_value)
      ProgressBar_Label.set_text("Game saved to " + path);
    else
      ProgressBar_Label.set_text("Saving game failed!");
     break;
   }
   default:
   {
     Logger::Log_Error("Unexpected button clicked." );
     break;
   }
 }
}

void Game_Window::Clear_Map_Images()
{
  for(auto &box : Map_Images)
  {
    auto children = box->get_children();
    for( auto &var : children)
    {
      auto eventbox = dynamic_cast<Gtk::EventBox*>(var);
      auto images = eventbox->get_children();
      for(auto &image : images)
      {
        image->hide();
        eventbox->remove();
      }
      var->hide();
      box->remove(*var);
    }
    box->hide();
  }
  Map_Images.clear();
}

void Game_Window::Load_Game()
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
     tuple<bool, Game*> return_value = Main_Game.Load_Game(path);
     if(get<0>(return_value))
     {
       ProgressBar_Label.set_text("Game loaded from " + path);
       Main_Game = *get<1>(return_value);
       Main_Game.Set_Autosave(Main_Settings_Manager.Get_Autosave_Value());
       Clear_Map_Images();
       Map_Scrolled_Window.remove();
       Generate_Map_View();
       Update_Labels();
       Update_Action_Buttons(last_clicked_x, last_clicked_y);
       Update_Map();
     }
     else
     {
       ProgressBar_Label.set_text("Game loading failed");
     }
     break;
   }
   default:
   {
    Logger::Log_Error("Unexpected button clicked." );
     break;
   }
 }
}

void Game_Window::Show_Newspaper_Clicked()
{
  Gtk::Dialog Newspaper_Dialog("Newspaper");
  Gtk::Box *Dialog_Box = Newspaper_Dialog.get_content_area();
  Gtk::Frame Dialog_Root_Frame("Events");
  Gtk::ScrolledWindow Dialog_Scrolled_Window;
  Dialog_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window.set_min_content_height(300);
  Gtk::Box Events_List_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Dialog_Box->pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Dialog_Scrolled_Window);
  Dialog_Scrolled_Window.add(Events_List_Box);
  Dialog_Root_Frame.show();
  Dialog_Scrolled_Window.show();
  Events_List_Box.show();
  vector<string> events = Main_Game.Get_Newspaper_Events();
  for(auto &event : events)
  {
    auto *label = Gtk::make_managed<Gtk::Label>(event);
    label->show();
    Events_List_Box.pack_start(*label);
    Main_Provider.Add_CSS(label);
  }
  Main_Provider.Add_CSS(&Newspaper_Dialog);
  Newspaper_Dialog.run();
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
  string message = "You lead the civilization of " + Main_Game.Get_Currently_Moving_Player()->Get_Name() + ". ";
  message = message + "\n Your civilization may colapse after few years or survive thousands. Who knows?";
  message = message + "\n Your civlization has following traits: ";
  vector<string> traits = Main_Game.Get_Currently_Moving_Player()->Get_Traits();
  for(string &trait : traits)
  {
    if(trait == "M")
      message = message + "\n Militaristic - Your units have one more attack strength";
    if(trait == "E")
      message = message + "\n Economic - You can build upgrades one gold cheaper";
    if(trait == "S")
      message = message + "\n Scientific - Your research funds are 10% bigger";
    if(trait == "N")
      message = message + "\n Nautical - Your naval units have two more movement";
    if(trait == "C")
      message = message + "\n Cultural - Changing a goverment costs no actions";
    if(trait == "R")
      message = message + "\n Religious - You have one more actions from the start of the game";
    if(trait == "X")
      message = message + "\n Expansive - Cities you settle start with bigger teritorry";
    if(trait == "A")
      message = message + "\n Agricultural - Farms produce one more gold";
    if(trait == "O")
      message = message + "\n Commercial - Production boost from roads is doubled";
    if(trait == "D")
      message = message + "\n Nomadic - All your units have one more movement";
  }
  Gtk::Label Dialog_Label = Gtk::Label(message);
  Dialog_Box->pack_start(Dialog_Label);
  Main_Provider.Add_CSS(&dialog);
  dialog.show_all_children();
  dialog.run();
}

void Game_Window::Show_Help_Message()
{
  Gtk::Dialog dialog("Help");
  dialog.add_button("Ok", 0);
  fstream Help_File(assets_directory_path + "help_file.txt");
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
  dialog.set_default_size(600,600);
  Dialog_Box->pack_start(Main_Scrolled_Window);
  Main_Scrolled_Window.add(Dialog_Label);
  dialog.show_all_children();
  Main_Provider.Add_CSS(&dialog);
  dialog.run();
}

Game_Window::Game_Window(Window_Manager *m_m, Settings_Manager m_s_m, string path) : Root_Box(Gtk::ORIENTATION_HORIZONTAL,2)
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
        Main_Game = Game(Root_Node);
        Logger::Log_Info("Loading Game Successfull!");
      }
      catch(...)
      {
        Logger::Log_Error("Loading XML Game Data Failed...");
        Logger::Log_Warning("Creating New Game!");
        path = " ";
      }
    }
    else
    {
      Logger::Log_Error("Opening file " + path + " failed!");
      Logger::Log_Warning("Creating New Game!");
      path = " ";
    }
  }
  if(path == " ")
    Main_Game = Game(Main_Settings_Manager.Get_Autosave_Value());
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
  Map_Update_Button = Gtk::Button("Update Map");
  Show_Civs_Button = Gtk::Button("Foregin Ministry");
  End_Turn_Button = Gtk::Button("End Turn");
  Civ_Overview_Button = Gtk::Button("Overview");
  Manage_Techs_Button = Gtk::Button("Science Ministry");
  Manage_Economy_Button = Gtk::Button("Finance Ministry");
  Newspaper_Button = Gtk::Button("Newspaper");
  Manage_Goverments_Button = Gtk::Button("Revolution!");
  Save_Button = Gtk::Button("Save Game");
  Load_Button = Gtk::Button("Load Game");
  Quit_Button = Gtk::Button("Exit To Main Menu");
  Help_Button = Gtk::Button("Help");
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
  End_Turn_Box.pack_start(Map_Update_Button, Gtk::PACK_SHRINK);
  End_Turn_Box.pack_start(End_Turn_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Economy_Label, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Tile_Information_Label, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Manage_Techs_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Manage_Economy_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Show_Civs_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Manage_Goverments_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Civ_Overview_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Newspaper_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Action_Buttons_Frame, Gtk::PACK_SHRINK);
  Action_Buttons_Frame.add(Action_Buttons_Box);
  UI_Root_Box.pack_start(End_Turn_Box, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Save_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Load_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Quit_Button, Gtk::PACK_SHRINK);
  UI_Root_Box.pack_start(Help_Button, Gtk::PACK_SHRINK);
  Map_Root_Box.pack_start(Map_Frame);
  Map_Root_Box.pack_start(ProgressBar_Label, Gtk::PACK_SHRINK);
  Map_Root_Box.pack_start(Capital_Label, Gtk::PACK_SHRINK);
  Map_Frame.add(Map_Scrolled_Window);
  Main_Game.Confirm_Pass_To_Game_Window();
  Map_Update_Button.signal_clicked().connect( sigc::mem_fun(*this, &Game_Window::Test) );
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
  Main_Provider.Add_CSS(this);
  Main_Provider.Add_CSS(&End_Turn_Button);
  Main_Provider.Add_CSS(&Map_Update_Button);
  Main_Provider.Add_CSS(&Manage_Economy_Button);
  Main_Provider.Add_CSS(&Manage_Techs_Button);
  Main_Provider.Add_CSS(&Show_Civs_Button);
  Main_Provider.Add_CSS(&Manage_Goverments_Button);
  Main_Provider.Add_CSS(&Civ_Overview_Button);
  Main_Provider.Add_CSS(&Save_Button);
  Main_Provider.Add_CSS(&Load_Button);
  Main_Provider.Add_CSS(&Newspaper_Button);
  Main_Provider.Add_CSS(&Quit_Button);
  Main_Provider.Add_CSS(&Help_Button);
  if(path == " ")
    Show_Game_Creation_Dialog();
  Generate_Map_View();
  show_all_children();
  set_default_size(800,800);
  maximize();
  Update_Map();
  Update_Labels();
  Main_Game.Set_Autosave(Main_Settings_Manager.Get_Autosave_Value());
  Show_Intro_Message();
}
