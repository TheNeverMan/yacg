#include "help_object.h"

Help_Object::Help_Object(string n, string h_t)
{
  name = n;
  help_text = h_t;

}

Help_Object::Help_Object(xml_node<>* Root_Node)
{
  xml_node<>* Help_Node = Root_Node->first_node("help");
  name = Help_Node->first_attribute("name")->value();
  help_text = Help_Node->first_attribute("help_text")->value();
}

xml_node<>* Help_Object::Serialize_Help(memory_pool<>* doc)
{
  xml_node<>* Help_Node = doc->allocate_node(node_element, "help");
  xml_attribute<> *Name = doc->allocate_attribute("name", doc->allocate_string(string(name).c_str()));
  Help_Node->append_attribute(Name);
  xml_attribute<> *Help_Text = doc->allocate_attribute("help_text", doc->allocate_string(string(help_text).c_str()));
  Help_Node->append_attribute(Help_Text);
  return Help_Node;
}

string_view Help_Object::Get_Name() const
{
  return name;
}

string_view Help_Object::Info() const
{
  return help_text;
}
