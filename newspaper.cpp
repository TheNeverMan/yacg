#include "newspaper.h"

void Newspaper::Add_News(string date, string info)
{
  News.push_back(date + ": " + info);
}

vector<string> Newspaper::Get_News()
{
  return News;
}
