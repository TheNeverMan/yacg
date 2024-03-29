#pragma once
#include<string>
#include<filesystem>

using std::string;
using std::to_string;
using std::string_view;

#define STD_THREAD_IMPLEMENTATION_INCLUDE_MUTEX "mingw-std-threads/mingw.mutex.h"
#define STD_THREAD_IMPLEMENTATION_INCLUDE_THREAD "mingw-std-threads/mingw.thread.h"

  const string_view path_delimeter((char*)(&std::filesystem::path::preferred_separator), 1);
  const string assets_directory_path("assets" + string(path_delimeter));
