#include "newspaper_event.h"

Newspaper_Event::Newspaper_Event(string e, string f_d, string t_p, int p_i, bool i_d) : Texture_Owner(t_p)
{
  event = f_d + ": " + e;
  full_date = f_d;
  player_id = p_i;
  is_decorative = i_d;
  istringstream iss(full_date);
  string s;
  getline( iss, s, ' ' );
  number_date = stoi(s);
}
string_view Newspaper_Event::Get_Event() const
{
  return event;
}
string_view Newspaper_Event::Get_Full_Date() const
{
  return full_date;
}
int Newspaper_Event::Get_Number_Date() const
{
  return number_date;
}
bool Newspaper_Event::Is_Decorative() const
{
  return is_decorative;
}
int Newspaper_Event::Get_Player_Id() const
{
  return player_id;
}
