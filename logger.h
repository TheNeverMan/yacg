#pragma once
#include<iostream>
#include<fstream>
#include<string>

#include "globals.h"

using std::string;
using std::fstream;
using std::endl;
using std::ios;
using std::cout;

class Logger
{
  private:
    static inline bool log_to_console;
    static inline string log_file_path;
    static void Log_Entry(string message);
  public:
    static bool Prepare_Logger(string l_f_p, bool l_t_c);
    static void Log_Error(string message);
    static void Log_Info(string message);
    static void Log_Warning(string message);

};
