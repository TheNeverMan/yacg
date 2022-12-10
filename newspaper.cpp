#include "newspaper.h"

void Newspaper::Add_News(string date, string info, Event_Type Type)
{
  News.push_back(make_tuple<Event_Type,string>(static_cast<Event_Type>(Type),date + ": " + info));
}
/*
vector<string> Newspaper::Get_News()
{
  return News;
}*/
void Newspaper::Add_City_Build(string date, string info)
{
  Add_News(date, info, Event_Type::City_Build);
}

void Newspaper::Add_City_Conquer(string date, string info)
{
  Add_News(date, info, Event_Type::City_Conquer);
}

void Newspaper::Add_Nuclear_Attack(string date, string info)
{
  Add_News(date, info, Event_Type::Nuclear_Attack);
}

void Newspaper::Add_Revolt(string date, string info)
{
  Add_News(date, info, Event_Type::Revolt);
}

vector<array<string,2>> Newspaper::Get_Events_With_Icon_Paths()
{
  vector<array<string,2>> out;
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
      default:
        break;
    }
    out.push_back({path, info});
  }
  return out;
}
