#pragma once
#include<string>
#include<vector>
#include<memory>

#include "scaled_gtk_image.h"
#include "themed_dialog.h"
#include "gov.h"
#include "city.h"

using std::string;
using std::vector;
using std::make_shared;
using std::shared_ptr;
using std::to_string;

class Internal_Dialog : public Themed_Dialog
{
  private:
    Gov Active_Goverment;
    vector<City>* Cities;
    Gtk::Box Root_Box;
    Gtk::Box Explanation_Box;
    Gtk::Label Explanation_Label;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Label Goverment_Label;
    Gtk::Frame Cities_Frame;
    Gtk::ScrolledWindow Cities_Window;
    Gtk::Box Cities_Box;
    vector<shared_ptr<Scaled_Gtk_Image>> Cities_Icons;
  public:
    Internal_Dialog(Gov a_g, vector<City>& c);
};
