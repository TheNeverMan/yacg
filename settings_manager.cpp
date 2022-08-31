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
  }
  file.close();
}

Settings_Manager::Settings_Manager()
{
  autosave = false;
  tile_size = 32;
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
  tile_size = stoi(Settings_Node->first_node("tile_size")->value());
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
  xml_node<>* Tile_Size_Node = doc.allocate_node(node_element, "tile_size", doc.allocate_string(to_string(tile_size).c_str()));
  Settings_Node->append_node(Tile_Size_Node);
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

void Settings_Manager::Set_Tile_Size_Value(int t_s)
{
  tile_size = t_s;
}
