#include "file_opener_dialog.h"

File_Opener_Dialog::File_Opener_Dialog(string m_d, string f_p, bool to_open) : main_directory(m_d), filetype_pattern(f_p), FileChooserDialog("Please choose a file:")
{
  Logger::Log_Info("Showing File Opener Dialog...");
  if(to_open)
    set_action(Gtk::FILE_CHOOSER_ACTION_OPEN);
  else
    set_action(Gtk::FILE_CHOOSER_ACTION_SAVE);
  set_select_multiple(false);
  set_create_folders(false);
  add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  add_button("_Open", Gtk::RESPONSE_OK);
  auto user_filter = Gtk::FileFilter::create();
  user_filter->set_name("Game Files");
  user_filter->add_pattern(filetype_pattern);
  add_filter(user_filter);
  bool out = set_current_folder(main_directory);
  if(!out)
    Logger::Log_Warning("Directory " + main_directory + " not found!");
  returned_path = " ";
}

void File_Opener_Dialog::Show()
{
  int result = run();

 //Handle the response:
 switch(result)
 {
   case(Gtk::RESPONSE_OK):
   {
     returned_path = get_filename();
     break;
   }
   default:
   {
    Logger::Log_Error("Unexpected button clicked." );
     break;
   }
 }
}

string File_Opener_Dialog::Get_File_Path()
{
  return returned_path;
}
