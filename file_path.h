#pragma once
#include<string>
#include<algorithm>
#include<fstream>

#include "assets_path.h"
#include "logger.h"

using namespace std;

class File_Path
{
  protected:
    void Set_Fallback_File_Path(string f_f_p);
    string fallback_file_path;
    void Test_File_Existence();
  private:
    string file_path;
    void Validate_Path();
  public:
    File_Path(string f_p);
    File_Path();
    string Get_File_Path();
    void Set_File_Path(string f_p);
};
