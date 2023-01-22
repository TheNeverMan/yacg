#include "tutorial_dialog.h"

Tutorial_Dialog::Tutorial_Dialog(string i_p, string t_p) : Themed_Dialog("Tutorial"), Explanation_Text_Path(t_p), Explanation_Image_Path(i_p)
{
  Gtk::Box* Root_Box = get_content_area();
  Close_Button.set_label("Got it!");
  Explanation_Image = Gtk::Image(Explanation_Image_Path.Get_File_Path().data());
  Explanation_Text = Gtk::Label(Explanation_Text_Path.Get_File_Content());
  Text_Frame = Gtk::Frame("Tutorial");
  Dialog_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Root_Box->pack_start(Dialog_Box);
  Dialog_Box.pack_start(Explanation_Image);
  Dialog_Box.pack_start(Text_Frame);
  Text_Frame.add(Explanation_Text);
}
