#include "unit_info_dialog.h"

string Unit_Info_Dialog::Get_Trait_Info()
{
  string out;
  for(string &trait : Described_Unit.Get_Trait_Names())
  {
    if(trait == "flying")
      out = out + "\n Flying - This unit can only refresh movement in airports and can't take cities";
    if(trait == "missle")
      out = out + "\n Missle - This unit can be used once and can't take cities";
    if(trait == "plunder")
      out = out + "\n Plunder - This unit can plunder upgrades to destroy enemy economy";
    if(trait == "steal")
      out = out + "\n Steal - This unit will produce one gold every time it kills a unit";
    if(trait == "atomic")
      out = out + "\n Atomic - This unit will plunder and kill units on all nearby tiles when attacking";
    if(trait == "fightflying")
      out = out + "\n Fight Flying - This unit can fight back flying units";
    if(trait == "doesnotrecievedefensivebonus")
      out = out + "\n Does Not Recieve Defensive Bonuses";
    if(trait == "cannottakecities")
      out = out + "\n Can Not Conquer Cities";
  }
  return out;
}

Unit_Info_Dialog::Unit_Info_Dialog(Unit u) : Described_Unit(u), Themed_Dialog("Unit")
{
  Gtk::Box *Dialog_Box = get_content_area();
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Unit Informations");
  auto* Dialog_Root_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL,2);
  string message = "Recruit " + Described_Unit.Get_Name();
  message = message + "\n Cost \t" + to_string(Described_Unit.Get_Cost());
  message = message + "\n Maintenace \t" + to_string(Described_Unit.Get_Maitenance());
  message = message + "\n Attack Power \t" + to_string(Described_Unit.Get_Attack_Power());
  message = message + "\n Defense Power \t" + to_string(Described_Unit.Get_Defense_Power());
  message = message + "\n Movement Points \t" + to_string(Described_Unit.Get_Current_Actions());
  message + message + "\n Obsolete with \t" + Described_Unit.Get_Obsolete_Unit_Name();
  message = message + Get_Trait_Info();
  Glib::RefPtr<Gdk::Pixbuf> unit_pix;
  Glib::RefPtr<Gdk::Pixbuf> finished_pix;
  finished_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 64, 64);
  unit_pix = Gdk::Pixbuf::create_from_file(Described_Unit.Get_Texture_Path());
  unit_pix->scale(finished_pix, 0, 0, 64, 64, 0, 0, 2, 2, Gdk::INTERP_BILINEAR);
  auto* Described_Unit_Image = Gtk::make_managed<Gtk::Image>(finished_pix);
  auto* Described_Unit_Label = Gtk::make_managed<Gtk::Label>(message);
  Dialog_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Dialog_Root_Box);
  Dialog_Root_Box->pack_start(*Described_Unit_Image);
  Dialog_Root_Box->pack_start(*Described_Unit_Label);
}
