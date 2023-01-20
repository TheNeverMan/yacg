#include "tips_manager.h"

Tips_Manager::Tips_Manager()
{
  XML_Data_Loader Loader(" ");
  Tips = Loader.Load_Tips();
  tips_index = 0;
}

void Tips_Manager::Show_Random_Tip() const
{
  Sound_Manager::Play_Sound("assets/sounds/tip-audio.mp3");
  Tips_Dialog Dialog(Tips[rand() % Tips.size()]);
  Dialog.Show();
}

void Tips_Manager::Show_Tip_In_Order()
{
  Sound_Manager::Play_Sound("assets/sounds/tip-audio.mp3");
  Tips_Dialog Dialog(Tips[tips_index]);
  tips_index++;
  if(tips_index == static_cast<int>(Tips.size()))
    tips_index = 0;
  Dialog.Show();
}

string_view Tips_Manager::Get_Random_Tip() const
{
  return Tips[rand() % Tips.size()];
}
