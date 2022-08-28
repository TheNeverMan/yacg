#pragma once
#include<string>

#include "xml_serializable.h"

using namespace std;

class Texture_Owner
{
  private:
    string texture_path;
  public:
    Texture_Owner(string t_p);
    Texture_Owner(xml_node<>* Root_Node);
    string Get_Texture_Path();
    xml_node<>* Serialize_Textures(memory_pool<>* doc);
};
