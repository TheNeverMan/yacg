#include "assets_path.h"

#ifdef WIN32
  #include STD_THREAD_IMPLEMENTATION_INCLUDE_MUTEX
  #include STD_THREAD_IMPLEMENTATION_INCLUDE_THREAD
#else
  #include<mutex>
  #include<thread>
#endif
