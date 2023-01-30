#pragma once
#include<string>
#include<string_view>

#include "texture_owner.h"

using std::string;
using std::string_view;
using std::istringstream;

class Newspaper_Event : public Texture_Owner
{
  private:
    string event;
    string full_date;
    int number_date;
    int player_id;
    bool is_decorative;
  public:
    string_view Get_Event() const;
    string_view Get_Full_Date() const;
    int Get_Number_Date() const;
    bool Is_Decorative() const;
    int Get_Player_Id() const;
    Newspaper_Event(string e, string f_d, string t_p, int p_i, bool i_d);
};
