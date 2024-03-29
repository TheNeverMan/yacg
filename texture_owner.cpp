#include "texture_owner.h"
#include "assets_path.h"

string_view Texture_Owner::Get_Texture_Path() const
{
  return Texture_Path.Get_File_Path();
}

Texture_Owner::Texture_Owner(string t_p) : Texture_Path(t_p)
{

}

Texture_Owner::Texture_Owner() : Texture_Path("assets/textures/broken-texture.png")
{

}

Texture_Owner::Texture_Owner(xml_node<>* Root_Node)
{
  xml_node<>* Texture_Node = Root_Node->first_node("texture_owner");
  Texture_Path.Set_File_Path(Texture_Node->value());
}

xml_node<>* Texture_Owner::Serialize_Textures(memory_pool<>* doc)
{
  xml_node<>* Texture_Node = doc->allocate_node(node_element, "texture_owner", doc->allocate_string(Texture_Path.Get_File_Path().data()));
  return Texture_Node;
}
