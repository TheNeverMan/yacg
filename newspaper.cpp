#include "newspaper.h"

void Newspaper::Add_News(string date, string info, Event_Type Type, int player_id)
{
  News.push_back(make_tuple<Event_Type,string,int>(static_cast<Event_Type>(Type),date + ": " + info, static_cast<int>(player_id)));
}
/*
vector<string> Newspaper::Get_News()
{
  return News;
}*/
void Newspaper::Add_City_Build(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::City_Build, player_id);
}

void Newspaper::Add_City_Conquer(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::City_Conquer, player_id);
}

void Newspaper::Add_Nuclear_Attack(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Nuclear_Attack, player_id);
}

void Newspaper::Add_Revolt(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Revolt, player_id);
}

void Newspaper::Add_Rebellion(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Rebellion, player_id);
}

void Newspaper::Add_Catastrophe(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Catastrophe, player_id);
}

void Newspaper::Add_Assassination(string date, string info, int player_id)
{
  Add_News(date, info, Event_Type::Assassination, player_id);
}

vector<tuple<array<string,2>, int>> Newspaper::Get_Events_With_Icon_Paths() const
{
  vector<tuple<array<string,2>, int>> out;
  for(auto& Event : News)
  {
    string info = get<1>(Event);
    string path = "assets/textures/icons/";
    switch(get<0>(Event))
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
      default:
        break;
    }
    out.push_back(make_tuple<array<string,2>, int>({path, info}, static_cast<int>(get<2>(Event))));
  }
  return out;
}
