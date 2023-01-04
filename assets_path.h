#pragma once
#include<string>
#include<filesystem>

using std::string;
using std::to_string;

#define STD_THREAD_IMPLEMENTATION_INCLUDE_MUTEX "mingw-std-threads/mingw.mutex.h"
#define STD_THREAD_IMPLEMENTATION_INCLUDE_THREAD "mingw-std-threads/mingw.thread.h"

  const string path_delimeter(1,std::filesystem::path::preferred_separator);
  const string assets_directory_path("assets" + path_delimeter);
