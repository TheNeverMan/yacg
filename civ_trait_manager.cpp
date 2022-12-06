#include "civ_trait_manager.h"

string Civ_Trait_Manager::Get_Trait_Full_Name(string trait_name)
{
  string out;
  if(trait_name == "M")
    out = "Militaristic";
  if(trait_name == "E")
    out = "Economic";
  if(trait_name == "S")
    out = "Scientific";
  if(trait_name == "N")
    out = "Nautical";
  if(trait_name == "C")
    out = "Cultural";
  if(trait_name == "R")
    out = "Religious";
  if(trait_name == "X")
    out = "Expansive";
  if(trait_name == "A")
    out = "Agricultural";
  if(trait_name == "O")
    out = "Commercial";
  if(trait_name == "D")
    out = "Nomadic";
  if(trait_name == "V")
    out = "Surviving";
  if(trait_name == "P")
    out = "Patriotic";
  if(trait_name == "I")
    out = "Mistic";
  return out;
}

shared_ptr<Scaled_Gtk_Image> Civ_Trait_Manager::Get_Trait_Icon(string trait_name)
{
  string full_trait_name = " ";
  if(trait_name.size() == 1)
    full_trait_name = Get_Trait_Full_Name(trait_name);
  else
    full_trait_name = trait_name;
  string image_path = "assets/textures/icons/traits";
  std::transform(full_trait_name.cbegin(), full_trait_name.cend(),
                 full_trait_name.begin(), // write to the same location
                 [](unsigned char c) { return std::tolower(c); });
  image_path = image_path + "/" + full_trait_name + "-icon.svg.png";
  shared_ptr<Scaled_Gtk_Image> Tmp = make_shared<Scaled_Gtk_Image>(image_path, 24, 24);
  Trait_Icons.push_back(Tmp);
  return Tmp;
}

string Civ_Trait_Manager::Get_Trait_Full_Explanation(string trait_name)
{
  string out = " ";
  if(trait_name == "M")
    out = "Your units have one more attack strength";
  if(trait_name == "E")
    out = "You can build upgrades one gold cheaper";
  if(trait_name == "S")
    out = "Your research funds are 10% bigger";
  if(trait_name == "N")
    out = "Your naval units have two more movement";
  if(trait_name == "C")
    out = "Changing a goverment costs no actions";
  if(trait_name == "R")
    out = "You have one more actions from the start of the game";
  if(trait_name == "X")
    out = "Cities you settle start with bigger teritorry";
  if(trait_name == "A")
    out = "Farms produce one more gold";
  if(trait_name == "O")
    out = "Production boost from roads is doubled";
  if(trait_name == "D")
    out = "All your units have one more movement";
  if(trait_name == "V")
    out = "All your infantry units can move on Ice and Desert tiles";
  if(trait_name == "P")
    out = "Your units fight better in your territory";
  if(trait_name == "I")
    out = "You start the game with one more random tech unlocked";
  return out;
}
