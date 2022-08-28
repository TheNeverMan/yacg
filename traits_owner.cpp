#include "traits_owner.h"

Traits_Owner::Traits_Owner(std::vector<string> t)
{
  Traits = t;
}

Traits_Owner::Traits_Owner(xml_node<>* Root_Node)
{
  xml_node<>* Traits_Node = Root_Node->first_node("traits");
  for(xml_node<> *Trait_Node = Traits_Node->first_node("trait"); Trait_Node; Trait_Node = Trait_Node->next_sibling("trait"))
  {
    Traits.push_back(Trait_Node->value());
  }
}

xml_node<>* Traits_Owner::Serialize_Traits(memory_pool<>* doc)
{
  xml_node<> *Traits_Node = doc->allocate_node(node_element, "traits");

  for_each(Traits.begin(), Traits.end(), [&](string trait)
  {
    xml_node<>* t_node = doc->allocate_node(node_element, "trait", doc->allocate_string(trait.c_str()));
    Traits_Node->append_node(t_node);
  } );

  return Traits_Node;
}

vector<string> Traits_Owner::Get_Traits()
{
  return Traits;
}

bool Traits_Owner::Has_Trait(string trait_name)
{
  if(find_if(Traits.begin(), Traits.end(), [&](string trait){ if(trait == trait_name){return true;} return false; }) != Traits.end())
  {
    return true;
  }
  return false;
}

int Traits_Owner::How_Many_Times_Has_Trait(string trait_name)
{
  return count(Traits.begin(), Traits.end(), trait_name);
}
