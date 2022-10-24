#include "upgrade_info_dialog.h"

string Upgrade_Info_Dialog::Get_Trait_Info()
{
  string out;
  vector<string> traits = Described_Upgrade.Get_Trait_Names();

  if(find(traits.begin(), traits.end(), "movementbonus") != traits.end())
  {
    out = out + "\n Movement Bonus - This upgrade will boost units movement by " + to_string(Described_Upgrade.How_Many_Times_Has_Trait("movementbonus"));
    auto new_end = std::remove(traits.begin(), traits.end(), "movementbonus");
    traits.erase(new_end, traits.end());
  }
  if(find(traits.begin(), traits.end(), "economybonus") != traits.end())
  {
    out = out + "\n Economy Bonus - This upgrade will boost nearby tiles production by " + to_string(Described_Upgrade.How_Many_Times_Has_Trait("economybonus"));
    auto new_end = std::remove(traits.begin(), traits.end(), "economybonus");
    traits.erase(new_end, traits.end());
  }
  if(find(traits.begin(), traits.end(), "minordefbonus") != traits.end())
  {
    out = out + "\n Minor Defense Bonus - This upgrade will increase unit defense by " + to_string(Described_Upgrade.How_Many_Times_Has_Trait("minordefbonus")*10) + " 5";
    auto new_end = std::remove(traits.begin(), traits.end(), "minordefbonus");
    traits.erase(new_end, traits.end());
  }
  if(find(traits.begin(), traits.end(), "majordefbonus") != traits.end())
  {
    out = out + "\n Major Defense Bonus - This upgrade will increase unit defense by " + to_string(Described_Upgrade.How_Many_Times_Has_Trait("majordefbonus")*20) + " %";
    auto new_end = std::remove(traits.begin(), traits.end(), "majordefbonus");
    traits.erase(new_end, traits.end());
  }
  if(find(traits.begin(), traits.end(), "accumulateproduction") != traits.end())
  {
    out = out + "\n Accumulate Production - This upgrade will produce gold equal to gold output of all nearby tiles x" + to_string(Described_Upgrade.How_Many_Times_Has_Trait("accumulateproduction"));
    auto new_end = std::remove(traits.begin(), traits.end(), "accumulateproduction");
    traits.erase(new_end, traits.end());
  }
  if(find(traits.begin(), traits.end(), "recruit") != traits.end())
  {
    out = out + "\n Recruit - Allows recruiting: ";
    for(auto &s : Described_Upgrade.Get_All_Arguments_For_Trait("recruit"))
      out = out + s + ", ";
    out = out + " units.";
    auto new_end = std::remove(traits.begin(), traits.end(), "recruit");
    traits.erase(new_end, traits.end());
  }
  if(find(traits.begin(), traits.end(), "avoid") != traits.end())
  {
    out = out + "\n Avoid - This upgrade can not border: ";
    for(auto &s : Described_Upgrade.Get_All_Arguments_For_Trait("avoid"))
      out = out + s + ", ";
    out = out + ".";
    auto new_end = std::remove(traits.begin(), traits.end(), "avoid");
    traits.erase(new_end, traits.end());
  }
  if(find(traits.begin(), traits.end(), "mustborder") != traits.end())
  {
    out = out + "\n Must Border - This upgrade must border at least one of these upgrades: ";
    for(auto &s : Described_Upgrade.Get_All_Arguments_For_Trait("mustborder"))
      out = out + s + ", ";
    out = out + ".";
    auto new_end = std::remove(traits.begin(), traits.end(), "mustborder");
    traits.erase(new_end, traits.end());
  }
  for(string &trait : traits)
  {
    if(trait == "removable")
      out = out + "\n Removable - This upgrade can be removed.";
    if(trait == "borderexpand")
      out = out + "\n Increase Borders - Will increase your borders.";
    if(trait == "increasehealrate")
      out = out + "\n Increase Heal Rate - This upgrade will increase heal rate of units healing on this tile.";
    if(trait == "giveaction")
      out = out + "\n Give Action - This upgrade will give you one more action";
    if(trait == "cityconnection")
      out = out + "\n City Connection - This upgrade can be used to connect cities with each other";
    if(trait == "allowflyingunitsheal")
      out = out + "\n Repair Flying Units - Flying units can heal on this upgrade.";
    if(trait == "renewflyingmovement")
      out = out + "\n Renew Flying Movement - Flying units will renew whole movment on this upgrade.";
    if(trait == "fightdebuff")
      out = out + "\n Fight Debuff - Units fighting on this tile will be weaker.";
    if(trait == "movementdebuff")
      out = out + "\n Movement Debuff - Moving on this tile will cost one more movement point";
    if(trait == "cannotbeplundered")
      out = out + "\n Can't Be Plundered";
  }
  return out;
}

Upgrade_Info_Dialog::Upgrade_Info_Dialog(Upgrade u) : Described_Upgrade(u), Themed_Dialog("Upgrade"), Described_Upgrade_Image(u.Get_Texture_Path(), 96, 96)
{
  Gtk::Box *Dialog_Box = get_content_area();
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Upgrade Informations");
  auto* Dialog_Root_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL,2);
  string allowed_tiles;
  vector<string> Allowed_Tiles = u.Get_Allowed_Tiles();
  for_each(Allowed_Tiles.begin(), Allowed_Tiles.end(), [&](string &tile){allowed_tiles = allowed_tiles + tile + ", ";});
  string message = "Build " + Described_Upgrade.Get_Name();
  message = message + "\n Allowed Tiles: " + allowed_tiles;
  message = message + "\n Cost \t" + to_string(Described_Upgrade.Get_Cost());
  message = message + "\n Maintenace \t" + to_string(Described_Upgrade.Get_Maitenance());
  message = message + "\n Production \t" + to_string(Described_Upgrade.Get_Production());
  message = message + Get_Trait_Info();
  auto* Described_Upgrade_Label = Gtk::make_managed<Gtk::Label>(message);
  Dialog_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Dialog_Root_Box);
  Dialog_Root_Box->pack_start(*(Described_Upgrade_Image.Get_Gtk_Image()));
  Dialog_Root_Box->pack_start(*Described_Upgrade_Label);
}
