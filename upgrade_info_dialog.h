#pragma once
#include<string>
#include<algorithm>

#include "themed_dialog.h"
#include "upgrade.h"

using namespace std;

class Upgrade_Info_Dialog : public Themed_Dialog
{
  private:
    Upgrade Described_Upgrade;
    string Get_Trait_Info();
  public:
    Upgrade_Info_Dialog(Upgrade u);
};
