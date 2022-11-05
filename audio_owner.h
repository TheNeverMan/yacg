#pragma once
#include<string>
#include<filesystem>

#include "xml_serializable.h"
#include "logger.h"
#include "audio_path.h"

using namespace std;

class Audio_Owner
{
  private:
    Audio_Path Sound_Path;
  public:
    Audio_Owner(string s_p);
    Audio_Owner();
    Audio_Owner(xml_node<>* Root_Node);
    string Get_Audio_Path();
    xml_node<>* Serialize_Audio(memory_pool<>* doc);
};
