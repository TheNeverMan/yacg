#pragma once
#include<string>
#include<vector>

#include "newspaper.h"
#include "themed_dialog.h"

class Newspaper_Dialog : public Themed_Dialog
{
  public:
    Newspaper_Dialog(vector<string> events);
};
