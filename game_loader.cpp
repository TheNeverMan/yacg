#include "game_loader.h"

Game_Loader::Game_Loader()
{

}

tuple<bool, Game*> Game_Loader::Load_Game(string path)
{
  Logger::Log_Info( "Path is " + path );
  ifstream File;
  bool success = true;
  Game* Placeholder;
  File.open(path, ios::in);
  if(File.is_open())
  {
    Logger::Log_Info( "Loading File Sucessful!" );
    Logger::Log_Info("Parsing XML...");
    try
    {
      xml_document<> doc;
      vector<char> buffer((istreambuf_iterator<char>(File)), istreambuf_iterator<char>());
      buffer.push_back('\0');
      doc.parse<0>(&buffer[0]);
      xml_node<> *Root_Node = doc.first_node()->first_node("game");
      Placeholder = new Game(Root_Node);
      success = true;
    }
    catch(...)
    {
      Logger::Log_Error("Parsing XML Failed!");
      success = false;
      File.close();
    }
  }
  else
  {
    Logger::Log_Error( "Opening file failed!" );
    success = false;
    File.close();
  }
  Logger::Log_Info( "Closing file..." );
  File.close();
  return make_tuple(success, Placeholder);
}
