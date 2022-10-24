#pragma once
#include<string>
#include<iostream>
#include<thread>

#include "themed_dialog.h"
#include "scaled_gtk_image.h"

using namespace std;

class Map_Generation_Dialog : public Themed_Dialog
{
  private:
    Gtk::Box Dialog_Box;
    Gtk::Box Explanation_Box;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Label Explanation_Label;
    Gtk::ProgressBar Main_Progress_Bar;
    Glib::Dispatcher Event_Dispatcher;
  protected:
    void Pulse_Fraction();
  public:
    Map_Generation_Dialog();
    void Notify();
};
