#include "culture.h"

Culture::Culture() : Help_Object(" ", " ")
{

}

Culture::Culture(string n) : Help_Object(n, " ")
{

}

Culture::~Culture()
{

}

Culture::Culture(xml_node<>* Root_Node) : Help_Object(Root_Node)
{
  Deserialize(Root_Node);
}

xml_node<>* Culture::Serialize(memory_pool<>* doc)
{
  xml_node<>* Root_Node = doc->allocate_node(node_element, "culture");
  xml_node<>* Upgrade_Texture_Replacements_Node = doc->allocate_node(node_element, "replacements");
  for(auto const& [upgrade_name, texture_path] : Upgrade_Texture_Replacements)
  {
    xml_node<>* Upgrade_Texture_Replacement_Node = doc->allocate_node(node_element, "replacement");
    xml_attribute<>* Upgrade_Name_Attribute = doc->allocate_attribute("name", upgrade_name.c_str());
    xml_attribute<>* Texture_Path_Attribute = doc->allocate_attribute("path", texture_path.c_str());
    Upgrade_Texture_Replacement_Node->append_attribute(Upgrade_Name_Attribute);
    Upgrade_Texture_Replacement_Node->append_attribute(Texture_Path_Attribute);
    Upgrade_Texture_Replacements_Node->append_node(Upgrade_Texture_Replacement_Node);
  }
  Root_Node->append_node(Serialize_Help(doc));
  Root_Node->append_node(Upgrade_Texture_Replacements_Node);
  return Root_Node;
}

void Culture::Deserialize(xml_node<>* Root_Node)
{
  xml_node<> *Upgrade_Texture_Replacements_Node = Root_Node->first_node("replacements");
  for(xml_node<> *Upgrade_Texture_Replacement_Node = Upgrade_Texture_Replacements_Node->first_node("replacement"); Upgrade_Texture_Replacement_Node; Upgrade_Texture_Replacement_Node = Upgrade_Texture_Replacement_Node->next_sibling("replacement"))
  {
    string name = Upgrade_Texture_Replacement_Node->first_attribute("name")->value();
    string path = Upgrade_Texture_Replacement_Node->first_node("path")->value();
    Upgrade_Texture_Replacements[name] = path;
  }
}

string Culture::Get_Texture_For_Upgrade(string upgrade_name)
{
  std::transform(upgrade_name.begin(), upgrade_name.end(), upgrade_name.begin(), ::tolower);
  string out = "assets/textures/upgrades/" + upgrade_name + "/" + Get_Name() + "-" + upgrade_name + "-upgrade-texture.svg";
  return out;
}
