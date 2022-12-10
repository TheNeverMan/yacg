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
    Sound_Manager Main_Sound_Manager;
  public:
    Tips_Manager();
    void Show_Random_Tip();
    void Show_Tip_In_Order();
    string Get_Random_Tip();
};
