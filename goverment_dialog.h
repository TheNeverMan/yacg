#pragma once
#include<string>
#include<memory>
#include<vector>

#include "gov.h"
#include "civ.h"
#include "themed_dialog.h"
#include "scaled_gtk_image.h"

using std::string;
using std::vector;
using std::make_shared;
using std::shared_ptr;

class Goverment_Dialog : public Themed_Dialog
{
  private:
    vector<shared_ptr<Scaled_Gtk_Image>> Gov_Images;
    Civ Player;
    string Selected_Goverment;
  protected:
    void Goverment_Button_Clicked(Gov s_g);
  public:
    Goverment_Dialog(Civ p);
    string Get_Selected_Goverment();
};
