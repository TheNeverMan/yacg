#include "game_creation_window.h"

Game_Creation_Window::Game_Creation_Window(Window_Manager* m_m, Settings_Manager m_s_m) : Civs_Color_Image(96, 48), Play_Button("Play!"), Quit_Button("Exit to Main Menu"), Map_Button("Load Map From File")
{
  Map_Data.continents_number = 10;
  Map_Data.size_x = 50;
  Map_Data.size_y = 50;
  Map_Data.oceans_precentage = 50;
  Map_Data.map_path = " ";
  XML_Data_Loader Loader("./");
  Main_Manager = m_m;
  Main_Settings_Manager = m_s_m;
  Main_Provider.Add_CSS(this);
  set_title("Create Game");
  Dialog_Map_Frame = Gtk::Frame("World Settings");
  Dialog_Root_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL,2);
  Map_UI_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Dialog_Players_Frame = Gtk::Frame("Select Your Civilization");
  Spectator_Mode_Button = Gtk::CheckButton("Spectator Mode (Observe AI Gameplay)");
  Players_UI_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Main_Player_UI_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Root_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Map_Data_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Other_Players_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);
  Buttons_Frame = Gtk::Frame("Actions");
  Buttons_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Map_Path_Label = Gtk::Label(" ");
  Civs_Scrolled_Window = Gtk::ScrolledWindow();
  Civs_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Main_Scrolled_Window = Gtk::ScrolledWindow();
  Main_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Civs_Scrolled_Window.set_min_content_height(200);
  Main_Scrolled_Window.set_min_content_height(500);
  Civs_Trait_Explanation_Label = Gtk::Label("Every Civilization has traits which \n determine its strengths and weaknesses. \n Selected civ has following traits:");
  Civs_Trait_Frame = Gtk::Frame("Traits");
  Civs_Trait_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2);

  Civs_Frame = Gtk::Frame("Other Players");
  Civs_Box = Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0);

  Randomize_Starting_Locations_Button = Gtk::CheckButton("Randomize Starting Locations On Loaded Map");

  Civs_Description_Frame = Gtk::Frame("Civ Description");
  Civs_Description_Label = Gtk::Label(" ");
  Allow_Duplicate_Civs_Button = Gtk::CheckButton("Allow Duplicate Civilizations Existing In The Game");

  Other_Players_Label = Gtk::Label("Choose how many AI players will participate in game:");
  Glib::RefPtr<Gtk::Adjustment> Players_Adjustment;
  Players_Adjustment = Gtk::Adjustment::create(0.0,0.0,1000.0,1.0,10,0.0);
  Players_Switch = Gtk::SpinButton(Players_Adjustment);

  Other_Human_Players_Label = Gtk::Label("Choose how many other human players will participate in game:");
  Glib::RefPtr<Gtk::Adjustment> Human_Players_Adjustment;
  Human_Players_Adjustment = Gtk::Adjustment::create(1.0,1.0,1000.0,1.0,10,0.0);
  Human_Players_Switch = Gtk::SpinButton(Human_Players_Adjustment);

  X_Label = Gtk::Label("Map Creation Preferences\nSet Map Size\n X Size:");
  Glib::RefPtr<Gtk::Adjustment> X_Adjustment;
  X_Adjustment = Gtk::Adjustment::create(50.0,10.0,1000.0,1.0,10,0.0);
  X_Switch = Gtk::SpinButton(X_Adjustment);

  Y_Label = Gtk::Label("Y Size:");
  Glib::RefPtr<Gtk::Adjustment> Y_Adjustment;
  Y_Adjustment = Gtk::Adjustment::create(50.0,10.0,1000.0,1.0,10,0.0);
  Y_Switch = Gtk::SpinButton(Y_Adjustment);

  Continents_Label = Gtk::Label("Amount of Continents \n(Higher the value, the more fractured the land will be):");
  Glib::RefPtr<Gtk::Adjustment> Continents_Adjustment;
  Continents_Adjustment = Gtk::Adjustment::create(10.0,1.0,50.0,1.0,10,0.0);
  Continents_Switch = Gtk::SpinButton(Continents_Adjustment);

  Water_Label = Gtk::Label("Amount of Water \n(Higher the value, less land will be one the map):");
  Glib::RefPtr<Gtk::Adjustment> Water_Adjustment;
  Water_Adjustment = Gtk::Adjustment::create(50.0,10.0,110.0,1.0,10,0.0);
  Water_Switch = Gtk::SpinButton(Water_Adjustment);



  Civilizations = Loader.Load_Civs();
  for(auto &civ : Civilizations)
    Civs_Chooser_Combo.append(civ.Get_Raw_Name().data());

  add(Dialog_Root_Box);
  Civs_Chooser_Combo.set_active(true);
  Dialog_Root_Box.pack_start(Root_Box);
  Root_Box.pack_start(Map_Data_Box);
  Root_Box.pack_start(Other_Players_Box);
  Map_Data_Box.pack_start(Dialog_Map_Frame);
//  Main_Scrolled_Window.add(Dialog_Map_Frame);
  Dialog_Map_Frame.add(Map_UI_Box);
  Map_UI_Box.pack_start(X_Label, Gtk::PACK_SHRINK);
  Map_UI_Box.pack_start(X_Switch, Gtk::PACK_SHRINK);
  Map_UI_Box.pack_start(Y_Label, Gtk::PACK_SHRINK);
  Map_UI_Box.pack_start(Y_Switch, Gtk::PACK_SHRINK);
  Map_UI_Box.pack_start(Continents_Label, Gtk::PACK_SHRINK);
  Map_UI_Box.pack_start(Continents_Switch, Gtk::PACK_SHRINK);
  Map_UI_Box.pack_start(Water_Label, Gtk::PACK_SHRINK);
  Map_UI_Box.pack_start(Water_Switch, Gtk::PACK_SHRINK);
  Map_Data_Box.pack_start(Dialog_Players_Frame, Gtk::PACK_SHRINK);
  Dialog_Players_Frame.add(Players_UI_Box);
  Players_UI_Box.pack_start(Main_Player_UI_Box, Gtk::PACK_SHRINK);
  Main_Player_UI_Box.pack_start(Civs_Color_Image.Get_Gtk_Image(), Gtk::PACK_SHRINK);
  Main_Player_UI_Box.pack_start(Civs_Chooser_Combo, Gtk::PACK_SHRINK);
  Main_Player_UI_Box.pack_start(Civs_Description_Frame, Gtk::PACK_SHRINK);
  Civs_Description_Frame.add(Civs_Description_Label);
  Players_UI_Box.pack_start(Civs_Trait_Frame);
  Civs_Trait_Frame.add(Civs_Trait_Box);
  Civs_Trait_Box.pack_start(Civs_Trait_Explanation_Label);
  Other_Players_Box.pack_start(Other_Players_Label, Gtk::PACK_SHRINK);
  Other_Players_Box.pack_start(Players_Switch, Gtk::PACK_SHRINK);
  Other_Players_Box.pack_start(Other_Human_Players_Label, Gtk::PACK_SHRINK);
  Other_Players_Box.pack_start(Human_Players_Switch, Gtk::PACK_SHRINK);
  Other_Players_Box.pack_start(Allow_Duplicate_Civs_Button, Gtk::PACK_SHRINK);
  Other_Players_Box.pack_start(Spectator_Mode_Button, Gtk::PACK_SHRINK);
  Other_Players_Box.pack_start(Civs_Frame);
  //Players_UI_Box.pack_start(Civs_Frame, Gtk::PACK_SHRINK);
  Civs_Frame.add(Civs_Scrolled_Window);
  Civs_Scrolled_Window.add(Civs_Box);
  Other_Players_Box.pack_start(Map_Path_Label);
  Other_Players_Box.pack_start(Randomize_Starting_Locations_Button);
  Dialog_Root_Box.pack_start(Buttons_Frame);
  Buttons_Frame.add(Buttons_Box);
  string icon_directory = "assets/textures/icons/";
  Quit_Button.Change_Icon(icon_directory + "exit-icon.svg.png");
  Buttons_Box.pack_start(Quit_Button);
  Map_Button.Change_Icon(icon_directory + "load-icon.svg.png");
  Buttons_Box.pack_start(Map_Button);
  Play_Button.Change_Icon(icon_directory + "play-icon.svg.png");
  Buttons_Box.pack_start(Play_Button);



  X_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Game_Creation_Window::Change_Map_Data_X_Value));
  Y_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Game_Creation_Window::Change_Map_Data_Y_Value));
  Continents_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Game_Creation_Window::Change_Map_Data_Continents_Value));
  Water_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Game_Creation_Window::Change_Map_Data_Water_Value));
  Civs_Chooser_Combo.signal_changed().connect(sigc::mem_fun(*this, &Game_Creation_Window::Change_Main_Player_Civ));
  Quit_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Creation_Window::Quit_Button_Clicked));
  Play_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Creation_Window::Play_Button_Clicked));
  Human_Players_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Game_Creation_Window::Change_Human_Players_Number));
  Players_Switch.signal_value_changed().connect(sigc::mem_fun(*this, &Game_Creation_Window::Change_AI_Players_Number));
  Map_Button.signal_clicked().connect(sigc::mem_fun(*this, &Game_Creation_Window::Map_Button_Clicked));

  Main_Provider.Add_CSS(&Civs_Box);
  Main_Provider.Add_CSS_With_Class(&Civs_Description_Label, "medium_label");
  Main_Provider.Add_CSS_With_Class(&Civs_Chooser_Combo, "combobox");
  Main_Provider.Add_CSS(&X_Switch);
  Main_Provider.Add_CSS(&Y_Switch);
  Main_Provider.Add_CSS(&Continents_Switch);
  Main_Provider.Add_CSS(&Water_Switch);
  Main_Provider.Add_CSS(&Human_Players_Switch);
  Main_Provider.Add_CSS(&Players_Switch);
  auto Children = Civs_Chooser_Combo.get_children();
  for(auto& item : Children)
    Main_Provider.Add_CSS_With_Class(item, "combobox");
  Change_Main_Player_Civ();
  //set_decorated(false);
  show_all_children();
  Randomize_Starting_Locations_Button.hide();
  if(Main_Settings_Manager.Check_If_Game_Is_Launched_First_Time())
  {
    Tutorial_Dialog Dialog(string(assets_directory_path) + "textures/tutorial/game-creation-tutorial.png", string(assets_directory_path) + "tutorial/game-creation-tutorial.txt");
    Dialog.Show();
  }
}

void Game_Creation_Window::Show_Map_Selection_Dialog()
{
  Map_Loader_Dialog Dialog;
  Dialog.Show();
  string path = Dialog.Get_File_Path();
  Map_Data.map_path = path;
  Map_Button.set_label("Unload map");
  Randomize_Starting_Locations_Button.show();
  Map_Path_Label.set_text("Map loaded from " + path);
}

void Game_Creation_Window::Map_Button_Clicked()
{
  if(Map_Data.map_path == " ")
  {
    Show_Map_Selection_Dialog();
  }
  else
  {
    Map_Data.map_path = " ";
    Randomize_Starting_Locations_Button.hide();
    Map_Button.set_label("Load Map from File");
    Map_Path_Label.set_text(" ");
  }
}

void Game_Creation_Window::Change_Map_Data_X_Value()
{
  Map_Data.size_x = (int) X_Switch.get_value();
}

void Game_Creation_Window::Change_Map_Data_Y_Value()
{
  Map_Data.size_y = (int) Y_Switch.get_value();
}

void Game_Creation_Window::Change_Map_Data_Continents_Value()
{
  Map_Data.continents_number = (int) Continents_Switch.get_value();
}

void Game_Creation_Window::Change_Map_Data_Water_Value()
{
  Map_Data.oceans_precentage = (int) Water_Switch.get_value();
}

void Game_Creation_Window::Quit_Button_Clicked()
{
  Main_Manager->Show_Intro_Window();
}

void Game_Creation_Window::Update_Other_Player_Civ(int index, bool is_ai, shared_ptr<Scaled_Gtk_Image> image, Gtk::ComboBoxText* text)
{
  string selected_civ_name = text->get_active_text();
  Civ *selected_civ;
  for(auto& civ : Civilizations)
  {
    if(selected_civ_name == civ.Get_Raw_Name())
    {
        selected_civ = &civ;
    }
  }
  if(selected_civ == nullptr)
  {
    Logger::Log_Error("Error civ not found!");
    return;
  }
  Glib::RefPtr<Gdk::Pixbuf> color_pix;
  //color_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 32, 32);
  if(selected_civ_name != "Random Civilization")
  {
    image->Change_Path(selected_civ->Get_Texture_Path());
  }
  else
  {
    image->Change_Path(string(assets_directory_path) + "textures/flags/neutral-flag.png");
  }
  if(is_ai)
    AI_Players[index] = selected_civ_name;
  else
    Human_Players[index] = selected_civ_name;

}

void Game_Creation_Window::Add_Human_Player()
{
  auto *label = Gtk::make_managed<Gtk::Label>("Human");
  shared_ptr<Scaled_Gtk_Image> image = make_shared<Scaled_Gtk_Image>(string(assets_directory_path) + "textures/flags/neutral-flag.png", 96, 48);
  auto* name = Gtk::make_managed<Gtk::ComboBoxText>();
  auto* box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
  Human_Players.push_back("Random Civilization");
  Human_Players_Boxes.push_back(box);
  name->set_active(true);
  name->append("Random Civilization");
  for(auto &civ : Civilizations)
    name->append(civ.Get_Raw_Name().data());

  name->set_active_text("Random Civilization");
  box->pack_start((image->Get_Gtk_Image()), Gtk::PACK_SHRINK);
  box->pack_start(*name, Gtk::PACK_SHRINK);
  box->pack_start(*label, Gtk::PACK_SHRINK);
  Civs_Box.pack_start(*box);
  name->show();
  box->show();
  image->Get_Gtk_Image().show();
  label->show();
  name->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Creation_Window::Update_Other_Player_Civ), Human_Players.size() - 1, false, image, name));
}

void Game_Creation_Window::Add_AI_Player()
{
  auto *label = Gtk::make_managed<Gtk::Label>("AI");
  shared_ptr<Scaled_Gtk_Image> image = make_shared<Scaled_Gtk_Image>(string(assets_directory_path) + "textures/flags/neutral-flag.png", 96, 48);
  auto* name = Gtk::make_managed<Gtk::ComboBoxText>();
  auto* box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
  AI_Players.push_back("Random Civilization");
  AI_Players_Boxes.push_back(box);
  name->set_active(true);
  name->append("Random Civilization");
  for(auto &civ : Civilizations)
    name->append(civ.Get_Raw_Name().data());

  name->set_active_text("Random Civilization");
  box->pack_start((image->Get_Gtk_Image()), Gtk::PACK_SHRINK);
  box->pack_start(*name, Gtk::PACK_SHRINK);
  box->pack_start(*label, Gtk::PACK_SHRINK);
  Civs_Box.pack_start(*box);
  name->show();
  box->show();
  image->Get_Gtk_Image().set_margin_bottom(2);
  image->Get_Gtk_Image().show();
  label->show();
  name->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &Game_Creation_Window::Update_Other_Player_Civ), AI_Players.size() - 1, true, image, name));
}

void Game_Creation_Window::Remove_AI_Player()
{
  AI_Players.pop_back();
  AI_Players_Boxes[AI_Players_Boxes.size() - 1]->hide();
  AI_Players_Boxes.pop_back();
}

void Game_Creation_Window::Remove_Human_Player()
{
  Human_Players.pop_back();
  Human_Players_Boxes[Human_Players_Boxes.size() - 1]->hide();
  Human_Players_Boxes.pop_back();
}

void Game_Creation_Window::Change_AI_Players_Number()
{
  auto children = Civs_Box.get_children();
  int count_ai = AI_Players_Boxes.size();
  int ai_value = (int) Players_Switch.get_value();
  if(count_ai > ai_value)
  {
    while(count_ai != ai_value)
    {
      Remove_AI_Player();
      count_ai--;
    }
  }

  if(count_ai < ai_value)
  {
    while(count_ai != ai_value)
    {
      Add_AI_Player();
      count_ai++;
    }
  }
}

void Game_Creation_Window::Change_Human_Players_Number()
{
  auto children = Civs_Box.get_children();
  int count_human = Human_Players_Boxes.size() + 1;
  int human_value = (int) Human_Players_Switch.get_value();
  if(count_human > human_value)
  {
    while(count_human != human_value)
    {
      Remove_Human_Player();
      count_human--;
    }
  }

  if(count_human < human_value)
  {
    while(count_human != human_value)
    {
      Add_Human_Player();
      count_human++;
    }
  }
}

void Game_Creation_Window::Add_Players_From_Vector(vector<string> Players_To_Add, bool Is_AI)
{
  for(auto &p : Players_To_Add)
  {
    if(p == "Random Civilization")
    {
      Logger::Log_Info("Adding random civilization!");
      if(Civilizations.size() > 0)
      {
        int random = rand() % Civilizations.size();
        string n = Civilizations[random].Get_Raw_Name().data();
        Players.push_back({n, Is_AI});
        if(! Allow_Duplicate_Civs_Button.get_active())
          Civilizations.erase(Civilizations.begin() + random);
      }
      else
      {
        Logger::Log_Warning("Too many players added - not enough civilizations!");
        continue;
      }
    }
    else
    {
      Players.push_back({p, Is_AI});
    }
  }
}

void Game_Creation_Window::Play_Button_Clicked()
{
  Human_Players.push_back(Civs_Chooser_Combo.get_active_text());
  if(! Allow_Duplicate_Civs_Button.get_active())
  {
    int index = 0;
    for(auto& civ : Civilizations)
    {
      if(civ.Get_Raw_Name().data() == Civs_Chooser_Combo.get_active_text())
      {
        Civilizations.erase(Civilizations.begin() + index);
        break;
      }
      index++;
    }
  }
  reverse(Human_Players.begin(), Human_Players.end());
  Add_Players_From_Vector(Human_Players, false);
  Add_Players_From_Vector(AI_Players, true);
  if(Map_Data.map_path == " ")
  {
    Main_Manager->Show_Game_Window(Main_Settings_Manager, Map_Data, Players, Spectator_Mode_Button.get_active());
    return;
  }
  else
  {
    if(Randomize_Starting_Locations_Button.get_active())
    {
      Main_Manager->Show_Game_Window(Main_Settings_Manager, Map_Data, Players, Spectator_Mode_Button.get_active());
    }
    else
    {
      Main_Manager->Show_Game_Window_Load_Starting_Positions(Main_Settings_Manager, Map_Data, Players, Spectator_Mode_Button.get_active());
    }
  }
}

void Game_Creation_Window::Change_Main_Player_Civ()
{
  string selected_civ_name = Civs_Chooser_Combo.get_active_text();
  Civ* selected_civ = nullptr;
  for(auto& civ : Civilizations)
  {
    if(selected_civ_name == civ.Get_Raw_Name())
    {
        selected_civ = &civ;
    }
  }
  if(selected_civ == nullptr)
  {
    Logger::Log_Error("Error civ not found!");
    return;
  }
  Civs_Description_Label.set_text(selected_civ->Info().data());
  Civs_Color_Image.Change_Path(selected_civ->Get_Texture_Path());
  auto children = Civs_Trait_Box.get_children();
  bool skip_first = true;
  for( auto &var : children)
  {
    if(skip_first)
    {
      skip_first = false;
      continue;
    }
    var->hide();
    Civs_Trait_Box.remove(*var);
  }
  for(auto& trait_name : selected_civ->Get_Trait_Names())
  {
    auto* Trait_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 2);
    auto* Trait_Label = Gtk::make_managed<Gtk::Label>(string(Trait_Manager.Get_Trait_Full_Name(trait_name)) + "-" + string(Trait_Manager.Get_Trait_Full_Explanation(trait_name)));
    auto Trait_Icon = Trait_Manager.Get_Trait_Icon(trait_name);
    Main_Provider.Add_CSS_With_Class(Trait_Label, "medium_label");
    Civs_Trait_Box.pack_start(*Trait_Box);
    Trait_Box->pack_start((Trait_Icon->Get_Gtk_Image()), Gtk::PACK_SHRINK);
    Trait_Box->pack_start(*Trait_Label);
    show_all_children();
  }
}
