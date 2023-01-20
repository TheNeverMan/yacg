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
      City_Build, City_Conquer, Nuclear_Attack, Revolt, Catastrophe, Rebellion, Assassination
    };
    vector<tuple<Event_Type,string,int>> News;
    void Add_News(string date, string info, Event_Type Type, int player_id);
  public:
    void Add_City_Build(string date, string info, int player_id);
    void Add_City_Conquer(string date, string info, int player_id);
    void Add_Nuclear_Attack(string date, string info, int player_id);
    void Add_Revolt(string date, string info, int player_id);
    void Add_Catastrophe(string date, string info, int player_id);
    void Add_Rebellion(string date, string info, int player_id);
    void Add_Assassination(string date, string info, int player_id);
    //vector<string> Get_News();
    vector<tuple<array<string, 2>, int>> Get_Events_With_Icon_Paths() const;
};
