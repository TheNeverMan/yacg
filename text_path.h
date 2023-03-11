#pragma once
#include<fstream>
#include<sstream>

#include "file_path.h"

using std::string;
using std::string_view;
using std::fstream;
using std::stringstream;

class Text_Path : public File_Path
{
  public:
    Text_Path(string t_p);
    Text_Path();
    string Get_File_Content();
};
