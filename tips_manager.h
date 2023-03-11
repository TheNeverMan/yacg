#pragma once
#include<string>
#include<vector>
#include<random>

#include "tips_dialog.h"
#include "sound_manager.h"
#include "xml_data_loader.h"

class Tips_Manager
{
  private:
    vector<string> Tips;
    int tips_index;
  public:
    Tips_Manager();
    void Show_Random_Tip() const;
    void Show_Tip_In_Order();
    string_view Get_Random_Tip() const;
};
