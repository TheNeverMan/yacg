#include "globals.h"
#include "assets_path.h"

string Display_Version()
{
  string out = to_string(version_major);
  out = out + ".";
  out = out + to_string(version_minor);
  out = out + ".";
  out = out + to_string(version_build);
  out = out + "-";
  out = out + codename;
  out = out + " " + path_delimeter;
  return out;
  //return version_major + "." + version_minor + "." + version_build + "-" + codename;
}
