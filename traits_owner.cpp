#include "traits_owner.h"

Traits_Owner::Traits_Owner(std::vector<string> t)
{
  for(auto& tr : t)
  {
    Trait tmp(tr);
    Traits.push_back(tmp);
  }
}

Traits_Owner::Traits_Owner(xml_node<>* Root_Node)
{
  xml_node<>* Traits_Node = Get_Subnode(Root_Node, "traits");
  for(xml_node<> *Trait_Node = Traits_Node->first_node("trait"); Trait_Node; Trait_Node = Trait_Node->next_sibling("trait"))
  {
    Trait tmp(Trait_Node);
    Traits.push_back(tmp);
  }
}

xml_node<>* Traits_Owner::Serialize_Traits(memory_pool<>* doc)
{
  xml_node<> *Traits_Node = doc->allocate_node(node_element, "traits");
  for(auto& trait : Traits)
  {
    Traits_Node->append_node(trait.Serialize_Trait(doc));
  }
  return Traits_Node;
}

void Traits_Owner::Give_Trait(string_view raw_trait)
{
  Trait tmp(raw_trait.data());
  Traits.push_back(tmp);
}

vector<string> Traits_Owner::Get_Trait_Names()
{
  vector<string> out;
  for_each(Traits.begin(), Traits.end(), [&](Trait t){out.push_back(t.Get_Trait_Name().data());});
  return out;
}

bool Traits_Owner::Has_Trait(string_view trait_name) const
{
  if(find_if(Traits.begin(), Traits.end(), [&](Trait trait){ if(trait.Get_Trait_Name() == trait_name){return true;} return false; }) != Traits.end())
  {
    return true;
  }
  return false;
}

int Traits_Owner::How_Many_Times_Has_Trait(string_view trait_name) const
{
  return count_if(Traits.begin(), Traits.end(), [&](Trait trait)
  {
    if(trait_name == trait.Get_Trait_Name())
      return true;
    return false;
  });
}

vector<string> Traits_Owner::Get_All_Arguments_For_Trait(string_view trait_name)
{
  vector<string> out;
  for(Trait t : Traits)
  {
    if(t.Get_Trait_Name() == trait_name)
    {
      vector<string> tmp =t.Get_All_Arguments();
      out.insert( out.end(), tmp.begin(), tmp.end() );
    }
  }
  return out;
}
