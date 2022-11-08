#pragma once
#include<gtkmm.h>
#include<string>

#include "assets_path.h"
#include "logger.h"

using std::string;

class File_Opener_Dialog : public Gtk::FileChooserDialog
{
  private:
    string main_directory;
    string filetype_pattern;
    string returned_path;
  public:
    File_Opener_Dialog(string m_d, string f_p, bool to_open);
    void Show();
    string Get_File_Path();
};
