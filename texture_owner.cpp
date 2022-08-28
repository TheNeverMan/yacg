#include "texture_owner.h"

string Texture_Owner::Get_Texture_Path()
{
  return texture_path;
}

Texture_Owner::Texture_Owner(string t_p)
{
  texture_path = t_p;
}

Texture_Owner::Texture_Owner(xml_node<>* Root_Node)
{
  xml_node<>* Texture_Node = Root_Node->first_node("texture_owner");
  texture_path = Texture_Node->value();
}

xml_node<>* Texture_Owner::Serialize_Textures(memory_pool<>* doc)
{
  xml_node<>* Texture_Node = doc->allocate_node(node_element, "texture_owner", doc->allocate_string(texture_path.c_str()));
  return Texture_Node;
}
