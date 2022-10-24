#pragma once
#include<string>
#include<filesystem>

#include "xml_serializable.h"
#include "logger.h"
#include "image_path.h"

using namespace std;

class Texture_Owner
{
  private:
    Image_Path Texture_Path;
  public:
    Texture_Owner(string t_p);
    Texture_Owner();
    Texture_Owner(xml_node<>* Root_Node);
    string Get_Texture_Path();
    xml_node<>* Serialize_Textures(memory_pool<>* doc);
};
