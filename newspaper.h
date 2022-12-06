#pragma once
#include<vector>
#include<string>
#include<tuple>
#include<array>

using std::string;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::array;
using std::get;

class Newspaper
{
  private:
    enum class Event_Type
    {
      City_Build, City_Conquer, Nuclear_Attack, Revolt,
    };
    vector<tuple<Event_Type,string>> News;
    void Add_News(string date, string info, Event_Type Type);
  public:
    void Add_City_Build(string date, string info);
    void Add_City_Conquer(string date, string info);
    void Add_Nuclear_Attack(string date, string info);
    void Add_Revolt(string date, string info);
    //vector<string> Get_News();
    vector<array<string, 2>> Get_Events_With_Icon_Paths();
};
