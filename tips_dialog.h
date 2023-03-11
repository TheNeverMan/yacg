#pragma once
#include<string>

#include "themed_dialog.h"

using std::string_view;
using std::string;

class Tips_Dialog : public Themed_Dialog
{
  public:
    Tips_Dialog(string_view tip);
};
