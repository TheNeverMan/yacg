#include "civ_trait_manager.h"

string_view Civ_Trait_Manager::Get_Trait_Full_Name(string trait_name) const
{
  return Traits[trait_name][0];
}

string_view Civ_Trait_Manager::Get_Trait_Letter(string trait_name) const
{
  for(auto &var : Traits)
  {
    if(var.second[0] == trait_name)
      return var.first;
  }
  Logger::Log_Error(trait_name + " trait not found!");
  throw;
}


shared_ptr<Scaled_Gtk_Image> Civ_Trait_Manager::Get_Trait_Icon(string trait_name) const
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

Civ_Trait_Manager::Civ_Trait_Manager()
{
  Traits["M"] = {"Militaristic", "Your units have one more attack strength"};
  Traits["E"] = {"Economic", "You can build upgrades one gold cheaper"};
  Traits["S"] = {"Scientific", "Your research funds are 10% bigger"};
  Traits["N"] = {"Nautical", "Your naval units have two more movement"};
  Traits["C"] = {"Cultural", "Changing a goverment costs no actions"};
  Traits["R"] = {"Religious", "You have one more actions from the start of the game"};
  Traits["X"] = {"Expansive", "Cities you settle start with bigger teritorry"};
  Traits["A"] = {"Agricultural", "Farms produce one more gold"};
  Traits["O"] = {"Commercial", "Production boost from roads is doubled"};
  Traits["D"] = {"Nomadic", "All your units have one more movement"};
  Traits["V"] = {"Surviving", "All your infantry units can move on Ice and Desert tiles"};
  Traits["P"] = {"Patriotic", "Your units fight better in your territory"};
  Traits["I"] = {"Mistic", "You start the game with one more random tech unlocked"};
}

string_view Civ_Trait_Manager::Get_Trait_Full_Explanation(string trait_name) const
{
  return Traits[trait_name][1];
}
