#include "logger.h"

bool Logger::Prepare_Logger(string l_f_p, bool l_t_c)
{
  log_file_path = l_f_p;
  log_to_console = l_t_c;
  fstream Log_File(log_file_path, ios::trunc | ios::out);
  if(Log_File.is_open())
  {
    Log_File << Display_Version() << endl;
    Log_File << "MINIYACG Logger v1.0" << endl;
  }
  Log_File.close();
  return true;
}

void Logger::Log_Entry(string message)
{
  if(log_to_console)
    cout << message << endl;
  fstream Log_File(log_file_path, ios::ate | ios::out | ios::app);
  Log_File << message << endl;
  Log_File.close();
}

void Logger::Log_Error(string message)
{
  Log_Entry("[ERROR] " + message);
}

void Logger::Log_Info(string message)
{
  Log_Entry("[INFO] " + message);
}

void Logger::Log_Warning(string message)
{
  Log_Entry("[WARNING] " + message);
}
