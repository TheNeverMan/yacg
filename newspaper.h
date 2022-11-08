#pragma once
#include<vector>
#include<string>

using std::string;
using std::vector;

class Newspaper
{
  private:
    vector<string> News;
  public:
    void Add_News(string date, string info);
    vector<string> Get_News();
};
