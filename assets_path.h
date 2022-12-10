#pragma once
#include<string>

using std::string;

#define STD_THREAD_IMPLEMENTATION_INCLUDE_MUTEX "mingw-std-threads/mingw.mutex.h"
#define STD_THREAD_IMPLEMENTATION_INCLUDE_THREAD "mingw-std-threads/mingw.thread.h"

#ifdef WIN32
  const string assets_directory_path = "assets\\";
  const string path_delimeter = "\\";
#else
  const string assets_directory_path = "assets/";
  const string path_delimeter = "/";
#endif
