#include "settings_manager.h"

Settings_Manager::Settings_Manager(string p_t_f)
{
  path_to_file = p_t_f;
  Logger::Log_Info("Loading XML Settings Data..." );
  ifstream file (path_to_file);
  if(file.is_open())
  {
    Load_Data_From_XML();
  }
  else
  {
    Logger::Log_Error("Opening Settings File Failed!");
    Logger::Log_Warning("Falling back to default values!");
    autosave = false;
    tile_size = 32;
    show_random_tip_on_startup = true;
    mute = false;
    autoresize = true;
  }
  file.close();
}

Settings_Manager::Settings_Manager()
{
  autosave = false;
  tile_size = 32;
}

bool Settings_Manager::Is_Game_Muted()
{
  return mute;
}

void Settings_Manager::Set_Mute_Value(bool m)
{
  mute = m;
}

void Settings_Manager::Launch_Game_First_Time()
{
  Logger::Log_Info("Creating Tutorial flag");
  ofstream File("do_not_show_tutorial");
  File << "true";
  File.close();
}

bool Settings_Manager::Check_If_Game_Is_Launched_First_Time()
{
  bool out = true;
  fstream File("do_not_show_tutorial");
  if(File.is_open())
    out = false;
  File.close();
  return out;
}

void Settings_Manager::Load_Data_From_XML()
{
  ifstream file (path_to_file);
  vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  xml_document<> doc;
  buffer.push_back('\0');
  file.close();
  doc.parse<0>(&buffer[0]);
  xml_node<> *Settings_Node = doc.first_node()->first_node("settings");
  autosave = static_cast<bool>(stoi(Settings_Node->first_node("autosave")->value()));
  autoresize = static_cast<bool>(stoi(Settings_Node->first_node("autoresize")->value()));
  show_random_tip_on_startup = static_cast<bool>(stoi(Settings_Node->first_node("startup_tip")->value()));
  tile_size = stoi(Settings_Node->first_node("tile_size")->value());
  mute = static_cast<bool>(stoi(Settings_Node->first_node("mute")->value()));
  Logger::Log_Info("XML Settings Data Loaded!" );
}

void Settings_Manager::Write_To_File()
{
  xml_document<> doc;
  ofstream File(path_to_file, ios::trunc | ios::out);
  xml_node<>* Root_Node = doc.allocate_node(node_element, "miniyacg");
  xml_node<>* Settings_Node = doc.allocate_node(node_element, "settings");
  Root_Node->append_node(Settings_Node);
  xml_node<>* Autosave_Node = doc.allocate_node(node_element, "autosave", doc.allocate_string(to_string(autosave).c_str()));
  Settings_Node->append_node(Autosave_Node);
  xml_node<>* Autoresize_Node = doc.allocate_node(node_element, "autoresize", doc.allocate_string(to_string(autoresize).c_str()));
  Settings_Node->append_node(Autoresize_Node);
  xml_node<>* Startup_Tip_Node = doc.allocate_node(node_element, "startup_tip", doc.allocate_string(to_string(show_random_tip_on_startup).c_str()));
  Settings_Node->append_node(Startup_Tip_Node);
  xml_node<>* Tile_Size_Node = doc.allocate_node(node_element, "tile_size", doc.allocate_string(to_string(tile_size).c_str()));
  Settings_Node->append_node(Tile_Size_Node);
  xml_node<>* Mute_Node = doc.allocate_node(node_element, "mute", doc.allocate_string(to_string(mute).c_str()));
  Settings_Node->append_node(Mute_Node);
  doc.append_node(Root_Node);
  std::string s;
  rapidxml::print(std::back_inserter(s), doc, 0);
  File << s;
}

bool Settings_Manager::Get_Autosave_Value()
{
  return autosave;
}

int Settings_Manager::Get_Tile_Size_Value()
{
  return tile_size;
}

void Settings_Manager::Set_Autosave_Value(bool a)
{
  autosave = a;
}

bool Settings_Manager::Get_Random_Tip_On_Startup_Value()
{
  return show_random_tip_on_startup;
}

void Settings_Manager::Set_Random_Tip_On_Startup_Value(bool a)
{
  show_random_tip_on_startup = a;
}


void Settings_Manager::Set_Tile_Size_Value(int t_s)
{
  tile_size = t_s;
}

void Settings_Manager::Set_Autoresize_Tiles_Value(bool a)
{
  autoresize = a;
}

bool Settings_Manager::Get_Autoresize_Tiles_Value()
{
  return autoresize;
}
