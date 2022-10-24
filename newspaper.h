#pragma once
#include<vector>
#include<string>

using namespace std;

class Newspaper
{
  private:
    vector<string> News;
  public:
    void Add_News(string date, string info);
    vector<string> Get_News();
};
