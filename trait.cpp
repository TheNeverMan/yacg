#include "trait.h"

Trait::Trait(string raw_trait)
{
  if(raw_trait.find('_') == string::npos)
  {
    trait_name = raw_trait;
  }
  else
  {
    string delimiter = "_";
    size_t pos = 0;
    std::string token;
    bool first = true;
    while ((pos = raw_trait.find(delimiter)) != std::string::npos)
    {
      token = raw_trait.substr(0, pos);
      raw_trait.erase(0, pos + delimiter.length());
      if(first)
      {
        first = false;
        trait_name = token;
        continue;
      }
      Arguments.push_back(token);
    }
    Arguments.push_back(raw_trait);
  }
  ////cout << "Trait: " << trait_name << " Arguments: ";
//  for_each(Arguments.begin(), Arguments.end(), [](string &s){//cout << s << ", "; });
  ////cout << endl;
}

Trait::Trait(xml_node<>* Root_Node)
{
  trait_name = Get_Value_From_Attribute(Root_Node, "name");
  for(xml_node<> *Argument_Node = Root_Node->first_node("argument"); Argument_Node; Argument_Node = Argument_Node->next_sibling("argument"))
  {
    Arguments.push_back(Argument_Node->value());
  }
}

xml_node<>* Trait::Serialize_Trait(memory_pool<>* doc)
{
  xml_node<> *Trait_Node = doc->allocate_node(node_element, "trait");
  xml_attribute<> *Name_Node = doc->allocate_attribute("name", doc->allocate_string(trait_name.c_str()));
  Trait_Node->append_attribute(Name_Node);
  for_each(Arguments.begin(), Arguments.end(), [&](string trait)
  {
    xml_node<>* t_node = doc->allocate_node(node_element, "argument", doc->allocate_string(trait.c_str()));
    Trait_Node->append_node(t_node);
  } );

  return Trait_Node;
}

string_view Trait::Get_Trait_Name() const
{
  return trait_name;
}

string_view Trait::Get_First_Argument() const
{
  if(Arguments.size())
    return Arguments[0];
  Logger::Log_Error("Trait Argument Not Found! Trait: " + trait_name);
  return " ";
}

vector<string> Trait::Get_All_Arguments() const
{
  return Arguments;
}
