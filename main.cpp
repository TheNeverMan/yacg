#include<iostream>
#include<stdlib.h>
#include<fstream>

#include "rapidxml-1.13/rapidxml.hpp"
#include "window_manager.h"
#include "globals.h"

using namespace rapidxml;
using std::fstream;
using std::cout;
using std::endl;

int main(int argc, char  *argv[])
{
	cout << Display_Version() << endl;
	fstream Font_File("font-installed", ios::in);
	if(!Font_File.is_open())
	{
		Font_File.close();
		cout << "Font is not installed... Installing!";
		system("font-install.bat");
		fstream Font_File2("font-installed", ios::out);
		Font_File2 << "true";
		Font_File2.close();
	}
	Font_File.close();
	Window_Manager Main_Manager(argc, argv);
	return Main_Manager.Run();
}
