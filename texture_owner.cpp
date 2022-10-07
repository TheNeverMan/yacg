#include "texture_owner.h"
#include "globals.h"

string Texture_Owner::Get_Texture_Path()
{
  return Texture_Path.Get_File_Path();
}

Texture_Owner::Texture_Owner(string t_p) : Texture_Path(t_p)
{
  namespace fs = std::filesystem;
  fs::path f{ Texture_Path.Get_File_Path() };
  if (!fs::exists(f))
    {
      Logger::Log_Warning("Texture " + t_p + " not found! Setting to broken-texture.png");
      Texture_Path.Set_File_Path(assets_directory_path + "textures" + path_delimeter + "broken-texture.png");
    }
}

Texture_Owner::Texture_Owner(xml_node<>* Root_Node)
{
  xml_node<>* Texture_Node = Root_Node->first_node("texture_owner");
  Texture_Path.Set_File_Path(Texture_Node->value());
}

xml_node<>* Texture_Owner::Serialize_Textures(memory_pool<>* doc)
{
  xml_node<>* Texture_Node = doc->allocate_node(node_element, "texture_owner", doc->allocate_string(Texture_Path.Get_File_Path().c_str()));
  return Texture_Node;
}
