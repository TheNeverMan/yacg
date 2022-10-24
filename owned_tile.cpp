#include "owned_tile.h"

Owned_Tile::Owned_Tile()
{

}

Owned_Tile::Owned_Tile(xml_node<>* Root_Node) : Place(Root_Node->first_node("tile"))
{
  
}
