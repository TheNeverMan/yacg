#include "magic_thread_communicator.h"

Magic_Thread_Communicator::Magic_Thread_Communicator(Game *g, Game_Window *m_g)
{
  lock_guard<mutex> Lock(Main_Mutex);
  Main_Game = g;
  Main_Window = m_g;
  turn_outcome = 0;
  has_finished = false;
}

void Magic_Thread_Communicator::Do_Task()
{
  //i wonder will i die from not locking mutex here
  turn_outcome = Main_Game->End_Player_Turn(this);
}

void Magic_Thread_Communicator::Notify_Window()
{
  Main_Window->Notify_Game_Window_About_Turn();
}

void Magic_Thread_Communicator::Set_Turn_Outcome(int t_o)
{
  lock_guard<mutex> Lock(Main_Mutex);
  turn_outcome = t_o;
}

int Magic_Thread_Communicator::Get_Turn_Outcome()
{
  lock_guard<mutex> Lock(Main_Mutex);
  return turn_outcome;
}

void Magic_Thread_Communicator::Finish()
{
  lock_guard<mutex> Lock(Main_Mutex);
  has_finished = true;
}

bool Magic_Thread_Communicator::Has_Finished()
{
  lock_guard<mutex> Lock(Main_Mutex);
  return has_finished;
}
