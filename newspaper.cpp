#include "newspaper.h"

void Newspaper::Add_News(string date, string info, Event_Type Type, int player_id, bool is_deco)
{
  //News.push_back(make_tuple<Event_Type,string,int>(static_cast<Event_Type>(Type),date + ": " + info, static_cast<int>(player_id)));
  string path = "assets/textures/icons/";
  switch(Type)
  {
    case Event_Type::City_Build:
      path = path + "build-icon.svg.png";
      break;
    case Event_Type::City_Conquer:
      path = path + "conquer-icon.svg.png";
      break;
    case Event_Type::Nuclear_Attack:
      path = path + "nuclear-icon.svg.png";
      break;
    case Event_Type::Revolt:
      path = path + "revolt-icon.svg.png";
      break;
    case Event_Type::Rebellion:
      path = path + "rebellion-icon.svg";
      break;
    case Event_Type::Catastrophe:
      path = path + "catastrophe-icon.svg";
      break;
    case Event_Type::Assassination:
      path = path + "assassination-icon.svg";
      break;
    case Event_Type::Deco:
      path = path + "about-icon.svg";
      break;
    case Event_Type::Horde:
      path = path + "riots-icon.svg";
      break;
    default:
      break;
  }
  Newspaper_Event Tmp(info, date, path, player_id, is_deco);
  News.push_back(Tmp);
}
/*
vector<string> Newspaper::Get_News()
{
  return News;
}*/
void Newspaper::Add_City_Build(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::City_Build, player_id, false);
}

void Newspaper::Add_City_Conquer(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::City_Conquer, player_id, false);
}

void Newspaper::Add_Nuclear_Attack(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Nuclear_Attack, player_id, false);
}

void Newspaper::Add_Revolt(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Revolt, player_id, false);
}

void Newspaper::Add_Rebellion(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Rebellion, player_id, false);
}

void Newspaper::Add_Catastrophe(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Catastrophe, player_id, false);
}

void Newspaper::Add_Assassination(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Assassination, player_id, false);
}

void Newspaper::Add_Deco_Event(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Deco, player_id, true);
}

const vector<Newspaper_Event>& Newspaper::Get_Events_With_Icon_Paths() const
{
  return News;
}

void Newspaper::Add_Horde(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Horde, player_id, false);
}
