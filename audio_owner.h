#pragma once
#include<string>
#include<filesystem>

#include "xml_serializable.h"
#include "logger.h"
#include "audio_path.h"

using std::string;
using std::string_view;

class Audio_Owner : public XML_Serializable
{
  private:
    Audio_Path Sound_Path;
  public:
    Audio_Owner(string s_p);
    Audio_Owner();
    Audio_Owner(xml_node<>* Root_Node);
    string_view Get_Audio_Path() const;
    xml_node<>* Serialize_Audio(memory_pool<>* doc);
};
