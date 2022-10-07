#include "file_path.h"

string File_Path::Get_File_Path()
{
  return file_path;
}

void File_Path::Validate_Path()
{
  string linux_path_delimeter = "/";
  string windows_path_delimeter = "\\";
  replace(file_path.begin(), file_path.end(), linux_path_delimeter, path_delimeter);
  replace(file_path.begin(), file_path.end(), windows_path_delimeter, path_delimeter);
}

File_Path::File_Path(string f_p)
{
  file_path = f_p;
  Validate_Path();
}

File_Path::File_Path()
{
  file_path = " ";
}


void File_Path::Set_File_Path(string f_p)
{
  file_path = f_p;
  Validate_Path();
}
