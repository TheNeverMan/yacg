#pragma once
#include<string>
#include<string_view>

using std::string;
using std::to_string;
using std::string_view;

//version of the game
constexpr int version_major = 1;
constexpr int version_minor = 5;
constexpr int version_build = 0;
constexpr string_view codename="SUPPORT";

string Display_Version();
