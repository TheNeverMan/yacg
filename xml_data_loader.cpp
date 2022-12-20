#include "xml_data_loader.h"

XML_Data_Loader::XML_Data_Loader(string p_t_x)
{
  path_to_xml = p_t_x;
  Logger::Log_Info("Initializing XML Loader with path " + path_to_xml);
}

std::filesystem::directory_iterator XML_Data_Loader::Get_Files_In_Directory(string path)
{
  return std::filesystem::directory_iterator(assets_directory_path + path);
}

vector<char> XML_Data_Loader::Load_File(string path)
{
  Logger::Log_Info("Loading file " + path + "...");
  vector<char> out;
  try
  {
    ifstream file (path);
    if(! file.is_open())
    {
      Logger::Log_Error("Loading file failed");
    }
    vector<char> out((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    out.push_back('\0');
    file.close();
    return out;
  }
  catch(...)
  {
    Logger::Log_Error("Loading file failed!");
    throw;
  }

}

vector<Tile> XML_Data_Loader::Load_Tiles_From_File(string path)
{
  Logger::Log_Info("Loading XML Tile Data From " + path);
  vector<Tile> out;
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Tile Data failed!");
    return out;
  }
  xml_node<>* tiles_node = Root_Node->first_node("tiles");
  for(xml_node<> * tile_node = tiles_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling())
  {
    int c = stoi(tile_node->first_attribute("cost")->value());
    string n = tile_node->first_attribute("name")->value();
    string t_p = tile_node->first_attribute("texture")->value();
    vector<string> traits;
    for(xml_node<> *trait_node = tile_node->first_node("trait"); trait_node; trait_node = trait_node->next_sibling("trait"))
    {
      traits.push_back(trait_node->first_attribute("name")->value());
    }
    Tile tmp(c, n, t_p, traits);
    out.push_back(tmp);
  }
  return out;
}

vector<string> XML_Data_Loader::Load_Tips_From_File(string path)
{
  Logger::Log_Info("Loading XML Tips Data From " + path);
  vector<string> out;
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Tips Data failed!");
    return out;
  }
  xml_node<>* tips_node = Root_Node->first_node("tips");
  for(xml_node<> * tip_node = tips_node->first_node("tip"); tip_node; tip_node = tip_node->next_sibling("tip"))
  {
    string tip = tip_node->value();
    out.push_back(tip);
  }
  return out;
}

vector<string> XML_Data_Loader::Load_Tips()
{
  Logger::Log_Info("Loading XML Tips Data..." );
  vector<string> out;
  for(auto& file : Get_Files_In_Directory("tips"))
  {
    vector<string> tmp = Load_Tips_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Tips Data Loaded!" );
  return out;
}

vector<Culture> XML_Data_Loader::Load_Cultures_From_File(string path)
{
  Logger::Log_Info("Loading XML Cultures Data From " + path);
  vector<Culture> out;
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Cultures Data failed!");
    return out;
  }
  xml_node<>* cultures_node = Root_Node->first_node("cultures");
  for(xml_node<> * culture_node = cultures_node->first_node("culture"); culture_node; culture_node = culture_node->next_sibling("culture"))
  {
    string name = culture_node->first_attribute("name")->value();
    vector<string> cities;
    vector<string> leaders;
    for(xml_node<> *city_node = culture_node->first_node("city"); city_node; city_node = city_node->next_sibling("city"))
    {
      cities.push_back(city_node->first_attribute("name")->value());
    }
    for(xml_node<> *leader_node = culture_node->first_node("leader"); leader_node; leader_node = leader_node->next_sibling("leader"))
    {
      leaders.push_back(leader_node->first_attribute("name")->value());
    }
    Culture tmp(name, cities, leaders);
    out.push_back(tmp);
  }
  return out;
}

vector<Culture> XML_Data_Loader::Load_Cultures()
{
  Logger::Log_Info("Loading XML Cultures Data..." );
  vector<Culture> out;
  for(auto& file : Get_Files_In_Directory("cultures"))
  {
    vector<Culture> tmp = Load_Cultures_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Cultures Data Loaded!" );
  return out;
}

vector<string> XML_Data_Loader::Load_Traits_From_Root_Node(xml_node<>* Root_Node)
{
  vector<string> out;
  for(xml_node<> *trait_node = Root_Node->first_node("trait"); trait_node; trait_node = trait_node->next_sibling("trait"))
  {
    out.push_back(trait_node->first_attribute("name")->value());
  }
  return out;
}

vector<Tile> XML_Data_Loader::Load_Tiles()
{
  Logger::Log_Info("Loading XML Tile Data..." );
  vector<Tile> out;
  for(auto& file : Get_Files_In_Directory("tiles"))
  {
    vector<Tile> tmp = Load_Tiles_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Tile Data Loaded!" );
  return out;
}

vector<Tech> XML_Data_Loader::Load_Techs_From_File(string path)
{
  Logger::Log_Info("Loading XML Techs Data From " + path );
  vector<Tech> out;
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Tech Data failed!");
    return out;
  }
  xml_node<>* technologies_node = Root_Node->first_node("technologies");
  for(xml_node<> * tech_node = technologies_node->first_node("technology"); tech_node; tech_node = tech_node->next_sibling())
  {
    int c = stoi(tech_node->first_attribute("cost")->value());
    string n = tech_node->first_attribute("name")->value();
    string h_t = tech_node->first_attribute("info")->value();
    string t_p = tech_node->first_attribute("texture")->value();
    vector<string> req;
    vector<string> traits = Load_Traits_From_Root_Node(tech_node);
    for(xml_node<> *req_node = tech_node->first_node("requirement"); req_node; req_node = req_node->next_sibling("requirement"))
    {
      req.push_back(req_node->first_attribute("name")->value());
    }

    Tech tmp(n, c, req, h_t, t_p, traits);
    out.push_back(tmp);
  }
  return out;
}

vector<Tech> XML_Data_Loader::Load_Techs()
{
  Logger::Log_Info("Loading XML Tech Data..." );
  vector<Tech> out;
  for(auto& file : Get_Files_In_Directory("technologies"))
  {
    vector<Tech> tmp = Load_Techs_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Tech Data Loaded!" );
  return out;
}

vector<Unit> XML_Data_Loader::Load_Units_From_File(string path)
{
  Logger::Log_Info("Loading XML Units Data From " + path);
  vector<Unit> out;
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Units Data failed!");
    return out;
  }
  xml_node<>* units_node = Root_Node->first_node("units");
  for(xml_node<> * unit_node = units_node->first_node("unit"); unit_node; unit_node = unit_node->next_sibling())
  {
    string n = unit_node->first_attribute("name")->value();
    int c = stoi(unit_node->first_attribute("cost")->value());
    int a = stoi(unit_node->first_attribute("atk")->value());
    int d = stoi(unit_node->first_attribute("def")->value());
    int m = stoi(unit_node->first_attribute("move")->value());
    int ma = stoi(unit_node->first_attribute("mait")->value());
    string r = unit_node->first_attribute("requirement")->value();
    string h_t = unit_node->first_attribute("info")->value();
    string t_p = unit_node->first_attribute("texture")->value();
    string s_p = unit_node->first_attribute("audio")->value();
    string obsolete = unit_node->first_attribute("obsolete")->value();
    vector<string> a_t;
    vector<string> traits = Load_Traits_From_Root_Node(unit_node);
    for(xml_node<> *tile_node = unit_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling("tile"))
    {
      a_t.push_back(tile_node->first_attribute("name")->value());
    }
    Unit tmp(n, c, a, d, m, ma, h_t, r, t_p, a_t, traits, obsolete, s_p);
    out.push_back(tmp);
  }
  return out;
}

vector<Unit> XML_Data_Loader::Load_Units()
{
  Logger::Log_Info("Loading XML Units Data..." );
  vector<Unit> out;
  for(auto& file : Get_Files_In_Directory("units"))
  {
    vector<Unit> tmp = Load_Units_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Tech Units Loaded!" );
  return out;
}

vector<Upgrade> XML_Data_Loader::Load_Upgrades_From_File(string path)
{
  Logger::Log_Info("Loading XML Upgrades Data From " + path);
  vector<Upgrade> out;
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Upgrades Data failed!");
    return out;
  }
  xml_node<>* upgrades_node = Root_Node->first_node("upgrades");
  for(xml_node<> * upgrade_node = upgrades_node->first_node("upgrade"); upgrade_node; upgrade_node = upgrade_node->next_sibling())
  {
    string n = upgrade_node->first_attribute("name")->value();
    int c = stoi(upgrade_node->first_attribute("cost")->value());
    int m = stoi(upgrade_node->first_attribute("mait")->value());
    int p = stoi(upgrade_node->first_attribute("prod")->value());
    string h_t = upgrade_node->first_attribute("info")->value();
    string r = upgrade_node->first_attribute("requirement")->value();
    string t_p = upgrade_node->first_attribute("texture")->value();
    bool avoid = (bool) stoi(upgrade_node->first_attribute("avoid")->value());
    vector<string> correct_tiles;
    vector<string> traits = Load_Traits_From_Root_Node(upgrade_node);
    for(xml_node<> *tile_node = upgrade_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling("tile"))
    {
      correct_tiles.push_back(tile_node->first_attribute("name")->value());
    }
    Upgrade tmp(c, p, m, h_t, n, r, correct_tiles, t_p, avoid, traits);
    out.push_back(tmp);
  }
  return out;
}

vector<Upgrade> XML_Data_Loader::Load_Upgrades()
{
  Logger::Log_Info("Loading XML Upgrades Data..." );
  vector<Upgrade> out;
  for(auto& file : Get_Files_In_Directory("upgrades"))
  {
    vector<Upgrade> tmp = Load_Upgrades_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Tech Upgrades Loaded!" );
  return out;
}

vector<Gov> XML_Data_Loader::Load_Govs_From_File(string path)
{
  Logger::Log_Info("Loading XML Govs Data From " + path );
  vector<Gov> out;
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  if(Root_Node == nullptr)
  {
    Logger::Log_Error("Loading Govs Data failed!");
    return out;
  }
  xml_node<>* govs_node = Root_Node->first_node("goverments");
  for(xml_node<> * gov_node = govs_node->first_node("goverment"); gov_node; gov_node = gov_node->next_sibling("goverment"))
  {
    string n = gov_node->first_attribute("name")->value();
    string i = gov_node->first_attribute("info")->value();
    string t_r = gov_node->first_attribute("requirement")->value();
    string t_p = gov_node->first_attribute("texture")->value();
    string l_t = gov_node->first_attribute("title")->value();
    string s_n = gov_node->first_attribute("state_name")->value();
    int m_s = stoi(gov_node->first_attribute("max_stability")->value());
    std::setlocale(LC_NUMERIC,"C"); //decimal dot place
    double pa = stod(gov_node->first_attribute("passive_stability")->value());
    double a = stod(gov_node->first_attribute("army_stability")->value());
    vector<string> traits = Load_Traits_From_Root_Node(gov_node);
    Gov tmp_gov(n, l_t, s_n, t_r, i, t_p, traits, m_s, pa, a);
    out.push_back(tmp_gov);
  }
  return out;
}

vector<Gov> XML_Data_Loader::Load_Govs()
{
  Logger::Log_Info("Loading XML Goverments Data..." );
  vector<Gov> out;
  for(auto& file : Get_Files_In_Directory("govs"))
  {
    vector<Gov> tmp = Load_Govs_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Tech Goverments Loaded!" );
  return out;
}

vector<Civ> XML_Data_Loader::Load_Civs_From_File(string path)
{
  Logger::Log_Info("Loading XML Civs Data From " + path );
  vector<Civ> out;
  vector<Tech> Technologies = Load_Techs();
  vector<Unit> Units = Load_Units();
  vector<Gov> Goverments = Load_Govs();
  vector<Upgrade> Upgrades = Load_Upgrades();
  xml_document<> doc;
  vector<char> buffer = Load_File(path);
  doc.parse<0>(&buffer[0]);
  xml_node<>* Root_Node = doc.first_node();
  xml_node<>* civs_node = Root_Node->first_node("civs");
  for(xml_node<> * civ_node = civs_node->first_node("civ"); civ_node; civ_node = civ_node->next_sibling())
  {
    string n = civ_node->first_attribute("name")->value();
    string h_t = civ_node->first_node("info")->value();
    int r = stoi(civ_node->first_attribute("r")->value());
    int g = stoi(civ_node->first_attribute("g")->value());
    int b = stoi(civ_node->first_attribute("b")->value());
    string p = civ_node->first_attribute("personality")->value();
    string lower_n = n;
    std::transform(lower_n.begin(), lower_n.end(), lower_n.begin(), ::tolower);
    lower_n.erase(std::remove_if(lower_n.begin(), lower_n.end(), isspace), lower_n.end());
    string t_p = "assets/textures/flags/" + lower_n + "-flag.svg";
    string s_p = civ_node->first_attribute("audio")->value();
    string c = civ_node->first_attribute("culture")->value();
    vector<string> cities;
    vector<string> traits = Load_Traits_From_Root_Node(civ_node);
    vector<string> leaders;
    map<string, vector<string>> g_n_r;
    vector<string> rebellions;
    for(xml_node<> *city_node = civ_node->first_node("city"); city_node; city_node = city_node->next_sibling("city"))
    {
      cities.push_back(city_node->first_attribute("name")->value());
    }
    for(xml_node<> *leader_node = civ_node->first_node("leader"); leader_node; leader_node = leader_node->next_sibling("leader"))
    {
      leaders.push_back(leader_node->first_attribute("name")->value());
    }
    for(xml_node<> *rebel_node = civ_node->first_node("rebel"); rebel_node; rebel_node = rebel_node->next_sibling("rebel"))
    {
      rebellions.push_back(rebel_node->first_attribute("name")->value());
    }
    for(xml_node<> *rep_node = civ_node->first_node("replacement"); rep_node; rep_node = rep_node->next_sibling("replacement"))
    {
      g_n_r[rep_node->first_attribute("name")->value()].push_back(rep_node->first_attribute("leader")->value());
      g_n_r[rep_node->first_attribute("name")->value()].push_back(rep_node->first_attribute("state_name")->value());
    }
    Civ tmp(n, leaders, h_t, cities, Technologies, Units, r, g, b, traits, Goverments, g_n_r, p, Upgrades, t_p, s_p, c, rebellions);
    out.push_back(tmp);
  }
  return out;
}

vector<Civ> XML_Data_Loader::Load_Civs()
{
  Logger::Log_Info("Loading XML Civs Data..." );
  vector<Civ> out;
  for(auto& file : Get_Files_In_Directory("civs"))
  {
    vector<Civ> tmp = Load_Civs_From_File(file.path().string());
    out.insert( out.end(), tmp.begin(), tmp.end() );
  }
  Logger::Log_Info("XML Tech Civs Loaded!" );
  return out;
}
