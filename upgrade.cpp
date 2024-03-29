#include "upgrade.h"
#include "assets_path.h"

Upgrade::Upgrade(int c, int p, int m, string h_t, string n, string r_t, vector<string> a_t, string t_p, bool avoid, std::vector<string> t) : Traits_Owner(t), Help_Object(n, h_t), Texture_Owner(t_p), Technology_Requirements_Owner(r_t)
{
  cost = c;
  production = p;
  maitenance = m;
  Allowed_Tiles = a_t;
  does_avoid_same_type_upgrades = avoid;
}

Upgrade::Upgrade(int c, int p, int m, string h_t, string n, string r_t) : Traits_Owner({"unbuildable"}), Help_Object(n, h_t), Texture_Owner("assets/textures/upgrades/none-upgrade-texture.png"), Technology_Requirements_Owner(r_t)
{
  cost = c;
  production = p;
  maitenance = m;
}

int Upgrade::Get_Production() const
{
  return production;
}

int Upgrade::Get_Maitenance() const
{
  return maitenance;
}

int Upgrade::Get_Cost() const
{
  return cost;
}

bool Upgrade::Is_Tile_Allowed_By_Name(string_view tile_name) const
{
  for(auto &name : Allowed_Tiles)
    if(name == tile_name)
      return true;
  return false;
}

bool Upgrade::Does_Avoid_Same_Type_Upgrades() const
{
  return Has_Trait("must_avoid");
}

vector<string> Upgrade::Get_Allowed_Tiles() const
{
  return Allowed_Tiles;
}

void Upgrade::Decrease_Cost_By_One()
{
  cost--;
}

Upgrade::Upgrade(xml_node<>* Root_Node) : Traits_Owner(Root_Node), Help_Object(Root_Node), Texture_Owner(Root_Node), Technology_Requirements_Owner(Root_Node)
{
  Deserialize(Root_Node);
}

void Upgrade::Deserialize(xml_node<>* Root_Node)
{
  cost = Traits_Owner::Get_Int_Value_From_Attribute(Root_Node, "cost");
  production = Traits_Owner::Get_Int_Value_From_Attribute(Root_Node, "production");
  maitenance = Traits_Owner::Get_Int_Value_From_Attribute(Root_Node, "maitenance");
  xml_node<>* Allowed_Tiles_Node = Traits_Owner::Get_Subnode(Root_Node, "tiles");
  for(xml_node<> *Allowed_Tile_Node = Allowed_Tiles_Node->first_node("tile"); Allowed_Tile_Node; Allowed_Tile_Node = Allowed_Tile_Node->next_sibling("tile"))
  {
    Allowed_Tiles.push_back(Allowed_Tile_Node->value());
  }
}

xml_node<>* Upgrade::Serialize(memory_pool<>* doc)
{
  xml_node<> *Root_Node = doc->allocate_node(node_element, "upgrade");
  xml_attribute<> *Cost = doc->allocate_attribute("cost", doc->allocate_string(to_string(cost).c_str()));
  Root_Node->append_attribute(Cost);
  xml_attribute<> *Production = doc->allocate_attribute("production", doc->allocate_string(to_string(production).c_str()));
  Root_Node->append_attribute(Production);
  xml_attribute<> *Maitenance = doc->allocate_attribute("maitenance", doc->allocate_string(to_string(maitenance).c_str()));
  Root_Node->append_attribute(Maitenance);
  xml_node<> *Tiles_Node = doc->allocate_node(node_element, "tiles");

  for_each(Allowed_Tiles.begin(), Allowed_Tiles.end(), [&](string tile)
  {
    xml_node<>* ti_node = doc->allocate_node(node_element, "tile", doc->allocate_string(tile.c_str()));
    Tiles_Node->append_node(ti_node);
  } );


  Root_Node->append_node(Tiles_Node);
  Root_Node->append_node(Serialize_Traits(doc));
  Root_Node->append_node(Serialize_Help(doc));
  Root_Node->append_node(Serialize_Textures(doc));
  Root_Node->append_node(Serialize_Technologies(doc));
  return Root_Node;
}


void Upgrade::Reduce_Cost(int how_much)
{
  cost = cost - how_much;
}

void Upgrade::Reduce_Maitenance(int how_much)
{
  maitenance = maitenance - how_much;
}

void Upgrade::Increase_Production_By_One()
{
  production++;
}
