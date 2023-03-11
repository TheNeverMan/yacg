#include "tips_dialog.h"
#include "assets_path.h"

Tips_Dialog::Tips_Dialog(string_view tip) : Themed_Dialog("Tip")
{
  Gtk::Box *Dialog_Box = get_content_area();
  Close_Button.set_label("Got it!");
  auto* Dialog_Root_Frame = Gtk::make_managed<Gtk::Frame>("Tip");
  auto* Dialog_Root_Box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL,2);
  string message = string(tip);
  Glib::RefPtr<Gdk::Pixbuf> unit_pix;
  Glib::RefPtr<Gdk::Pixbuf> finished_pix;
  finished_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 64, 64);
  unit_pix = Gdk::Pixbuf::create_from_file(string(assets_directory_path) + "textures" + string(path_delimeter) + "tip.png");
  unit_pix->scale(finished_pix, 0, 0, 64, 64, 0, 0, 2, 2, Gdk::INTERP_BILINEAR);
  auto* Described_Unit_Image = Gtk::make_managed<Gtk::Image>(finished_pix);
  auto* Described_Unit_Label = Gtk::make_managed<Gtk::Label>(message);
  Dialog_Box->pack_start(*Dialog_Root_Frame);
  Dialog_Root_Frame->add(*Dialog_Root_Box);
  Dialog_Root_Box->pack_start(*Described_Unit_Image);
  Dialog_Root_Box->pack_start(*Described_Unit_Label);
}
