#pragma once
#include<fstream>

#include "file_path.h"

using namespace std;

class Text_Path : public File_Path
{
  public:
    Text_Path(string t_p);
    Text_Path();
    string Get_File_Content();
};
