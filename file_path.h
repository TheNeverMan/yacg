#pragma once
#include<string>
#include<algorithm>

#include "globals.h"

using namespace std;

class File_Path
{
  private:
    string file_path;
    void Validate_Path();
  public:
    File_Path(string f_p);
    File_Path();
    string Get_File_Path();
    void Set_File_Path(string f_p);
};
