#include "audio_owner.h"
#include "assets_path.h"

string Audio_Owner::Get_Audio_Path()
{
  return Sound_Path.Get_File_Path();
}

Audio_Owner::Audio_Owner(string s_p) : Sound_Path(s_p)
{

}

Audio_Owner::Audio_Owner() : Sound_Path("assets/sounds/broken-audio.wav")
{

}

Audio_Owner::Audio_Owner(xml_node<>* Root_Node)
{
  xml_node<>* Sound_Node = Root_Node->first_node("audio_owner");
  Sound_Path.Set_File_Path(Sound_Node->value());
}

xml_node<>* Audio_Owner::Serialize_Audio(memory_pool<>* doc)
{
  xml_node<>* Sound_Node = doc->allocate_node(node_element, "audio_owner", doc->allocate_string(Sound_Path.Get_File_Path().c_str()));
  return Sound_Node;
}
