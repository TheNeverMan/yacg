#include "game_saver.h"

Game_Saver::Game_Saver(Game *g_t_s)
{
  Game_To_Save = g_t_s;
}

bool Game_Saver::Save_Game(string path)
{
  Logger::Log_Info("Path is " + path );
  ofstream File;
  File.open(path, ios::trunc | ios::out);
  if(File.is_open())
  {
    memory_pool<> pool;
    xml_document<> doc;
    xml_node<>* Root_Node = Game_To_Save->Serialize(&pool);
    doc.append_node(Root_Node);
    File << doc;
  }
  else
  {
    Logger::Log_Error("Opening file failed!" );
    File.close();
    return false;
  }
  Logger::Log_Info("Closing file..." );
  File.close();
  return true;
}
