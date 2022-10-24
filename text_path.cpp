#include "text_path.h"

Text_Path::Text_Path(string t_p) : File_Path()
{
  Set_Fallback_File_Path("assets/broken-text.txt");
  Set_File_Path(t_p);
}

Text_Path::Text_Path() : File_Path()
{
  Set_Fallback_File_Path("assets/broken-text.txt");
  Set_File_Path("assets/broken-text.txt");
}

string Text_Path::Get_File_Content()
{
  string out = "";
  fstream File(Get_File_Path(), ios::in);
  while(!File.eof())
  {
    string tmp;
    File >> tmp;
    out = out + tmp + " ";
  }
  File.close();
  return out;
}
