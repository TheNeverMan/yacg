#include "magic_map_generation_thread_communicator.h"

Magic_Map_Generation_Thread_Communicator::Magic_Map_Generation_Thread_Communicator(Map_Generation_Dialog *g, shared_ptr<Gtk_Game_Map> m_g)
{
  Generation_Dialog = g;
  Game_Map = m_g;
}

void Magic_Map_Generation_Thread_Communicator::Do_Task(Game* m_g)
{
  Game_Map->Generate_Map(m_g, this);
}

void Magic_Map_Generation_Thread_Communicator::Notify_Dialog()
{
  Generation_Dialog->Notify();
}
