#pragma once
#include<string>
#include<iostream>
#include<thread>

#include "themed_dialog.h"

using namespace std;

class Map_Generation_Dialog : public Themed_Dialog
{
  private:
    Gtk::ProgressBar Main_Progress_Bar;
    Glib::Dispatcher Event_Dispatcher;
  protected:
    void Pulse_Fraction();
  public:
    Map_Generation_Dialog();
    void Notify();
};
