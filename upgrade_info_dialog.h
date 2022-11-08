#pragma once
#include<string>
#include<algorithm>

#include "themed_dialog.h"
#include "upgrade.h"
#include "scaled_gtk_image.h"

using std::string;

class Upgrade_Info_Dialog : public Themed_Dialog
{
  private:
    Upgrade Described_Upgrade;
    string Get_Trait_Info();
    Scaled_Gtk_Image Described_Upgrade_Image;
  public:
    Upgrade_Info_Dialog(Upgrade u);
};
