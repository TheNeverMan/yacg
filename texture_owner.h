#pragma once
#include<string>
#include<filesystem>

#include "xml_serializable.h"
#include "logger.h"
#include "image_path.h"

using std::string;
using std::string_view;

class Texture_Owner
{
  private:
    Image_Path Texture_Path;
  public:
    Texture_Owner(string t_p);
    Texture_Owner();
    Texture_Owner(xml_node<>* Root_Node);
    string_view Get_Texture_Path() const;
    xml_node<>* Serialize_Textures(memory_pool<>* doc);
};
