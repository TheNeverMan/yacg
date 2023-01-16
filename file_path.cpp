#include "file_path.h"

string_view File_Path::Get_File_Path() const
{
  return file_path;
}

void File_Path::Validate_Path()
{
  string linux_path_delimeter = "/";
  string windows_path_delimeter = "\\";
  replace(file_path.begin(), file_path.end(), *linux_path_delimeter.c_str(), *path_delimeter.data());
  replace(file_path.begin(), file_path.end(), *windows_path_delimeter.c_str(), *path_delimeter.data());
}

File_Path::File_Path(string f_p)
{
  file_path = f_p;
  Validate_Path();
  Set_Fallback_File_Path("assets/no-file.txt");
  Test_File_Existence();
}

File_Path::File_Path()
{
  file_path = "assets/no-file.txt";
  Validate_Path();
  Set_Fallback_File_Path("assets/no-file.txt");
  Test_File_Existence();
}

bool File_Path::Does_Exist() const
{
  return file_path != fallback_file_path;
}


void File_Path::Set_File_Path(string_view f_p)
{
  file_path = f_p;
  Validate_Path();
  Test_File_Existence();
}

void File_Path::Test_File_Existence()
{
  if(!std::filesystem::exists(file_path))
  {
    Logger::Log_Warning("File " + string(file_path) + " not found! Setting to " + string(fallback_file_path));
    file_path = fallback_file_path;
  }
}

void File_Path::Set_Fallback_File_Path(string_view f_f_p)
{
  string linux_path_delimeter = "/";
  string windows_path_delimeter = "\\";
  replace(f_f_p.begin(), f_f_p.end(), *linux_path_delimeter.c_str(), *path_delimeter.data());
  replace(f_f_p.begin(), f_f_p.end(), *windows_path_delimeter.c_str(), *path_delimeter.data());
  fallback_file_path = f_f_p;
}
