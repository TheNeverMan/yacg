#pragma once
#include<string>

#include "themed_dialog.h"

using std::string;

class Tips_Dialog : public Themed_Dialog
{
  public:
    Tips_Dialog(string tip);
};
