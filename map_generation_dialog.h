#pragma once
#include<string>
#include<iostream>
#include "thread.h"

#include "themed_dialog.h"
#include "scaled_gtk_image.h"
#include "tips_manager.h"

using std::string;

class Map_Generation_Dialog : public Themed_Dialog
{
  private:
    Gtk::Box Dialog_Box;
    Gtk::Box Explanation_Box;
    Scaled_Gtk_Image Explanation_Image;
    Gtk::Label Explanation_Label;
    Tips_Manager Random_Tip_Manager;
    Gtk::Label Tip_Label;
    Gtk::Box Text_Tip_Box;
    Gtk::Box Fraction_Bar_Box;
    Gtk::Label Fraction_Label;
    Gtk::ProgressBar Main_Progress_Bar;
    Glib::Dispatcher Event_Dispatcher;
  protected:
    void Pulse_Fraction();
  public:
    Map_Generation_Dialog();
    void Notify();
};
