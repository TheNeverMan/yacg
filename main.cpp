#include<iostream>

#include "rapidxml-1.13/rapidxml.hpp"
#include "window_manager.h"
#include "globals.h"

using namespace rapidxml;
using namespace std;


int main(int argc, char  *argv[])
{
	//cout << Display_Version() << endl;
	Window_Manager Main_Manager(argc, argv);
	return Main_Manager.Run();
}
