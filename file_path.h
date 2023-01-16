#pragma once
#include<string>
#include<algorithm>
#include<fstream>

#include "assets_path.h"
#include "logger.h"

using std::string;
using std::fstream;
using std::string_view;

class File_Path
{
  protected:
    void Set_Fallback_File_Path(string_view f_f_p);
    string fallback_file_path;
    void Test_File_Existence();
  private:
    string file_path;
    void Validate_Path();
  public:
    File_Path(string f_p);
    File_Path();
    string_view Get_File_Path() const;
    void Set_File_Path(string_view f_p);
    bool Does_Exist() const;
};
