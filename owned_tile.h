#pragma once

#include "xml_serializable.h"
#include "tile.h"

class Tile;

struct Owned_Tile
{
  int owner = 0; //neutrals
  Tile Place;
  Owned_Tile(xml_node<>* Root_Node);
  Owned_Tile();
};
