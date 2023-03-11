#include "civs_dialog.h"

Civs_Dialog::Civs_Dialog(vector<Civ>& p) : Themed_Dialog("Foregin Ministry"), Explanation_Image(string(assets_directory_path) + "textures/dialogs/civs-dialog-texture.svg", 70, 70), Players(p)
{
  Gtk::Box *Dialog_Box = get_content_area();
  Explanation_Box = Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2);
  Explanation_Label = Gtk::Label("Here is information about every civilization currently in the game. \n Each civilization has score in points which measures its overall performance.");
  Dialog_Root_Frame = Gtk::Frame("Civilizations");
  Dialog_Scrolled_Window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
  Dialog_Scrolled_Window.set_min_content_height(300);
  Dialog_Box->pack_start(Explanation_Box);
  Hide_Dead_Button = Gtk::CheckButton("Hide eliminated players");
  Hide_Dead_Button.set_active(true);
  Explanation_Box.pack_start((Explanation_Image.Get_Gtk_Image()));
  Explanation_Box.pack_start(Explanation_Label);
  //Dialog_Box->pack_start(Hide_Dead_Button);
  Dialog_Box->pack_start(Dialog_Root_Frame);
  Dialog_Root_Frame.add(Dialog_Scrolled_Window);
  Dialog_Scrolled_Window.add(Civs_List_Box);
  rows = 0;
  Hide_Dead_Button.signal_toggled().connect(sigc::mem_fun(*this, &Civs_Dialog::Update_Players));
  Update_Players();
}

string Civs_Dialog::Get_Number_In_Thousands(int number)
{
  int thousands = 0;
  int millions = 0;
  while(number > 1000)
  {
    number = number - 1000;
    thousands++;
    if(thousands == 1000)
    {
      thousands = 0;
      millions++;
    }
  }
  if(millions)
    return to_string(millions) + " millions";
  if(thousands)
    return to_string(thousands) + " thousands";
  return to_string(number);
}

void Civs_Dialog::Update_Players()
{
  Logger::Log_Info("Updating Players...");
  /*if(rows != 0)
  {
    int start = 0;
    while(start <= rows)
    {
      Civs_List_Box.remove_row(start);
      start++;
    }
    rows = 0;
  }*/
  vector<Row> All_Rows;
  int index = 1;
  for(auto &player : Players)
  {
    if(Hide_Dead_Button.get_active() && player.Get_Capital_Name() == "exile")
    {
      Logger::Log_Info("Player " + string(player.Get_Name()) + " is dead, skipping...");
      continue;
    }
    Row Row_Tmp;
    Row_Tmp.Flag_Image = Gtk::make_managed<Gtk::Image>();
    Scaled_Pixbuf tmp(player.Get_Texture_Path(), 96, 48);
    Row_Tmp.Color_Image = Gtk::make_managed<Gtk::Image>();
    Glib::RefPtr<Gdk::Pixbuf> color_pix;
    color_pix = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, 48, 48);
    color_pix->fill(player.Get_Civ_Color());
    Row_Tmp.Color_Image->set(color_pix);
    Row_Tmp.Color_Image->set_margin_bottom(3);
    Row_Tmp.Name_Label = Gtk::make_managed<Gtk::Label>(string(player.Get_Full_Name()) + " (" + string(player.Get_Leader_Name()) + ") " + " ID: " + to_string(index));
    Row_Tmp.Points_Label = Gtk::make_managed<Gtk::Label>(" Points: " + Get_Number_In_Thousands(player.Get_Score()));
    Row_Tmp.Army_Label = Gtk::make_managed<Gtk::Label>(" Army Size: " + Get_Number_In_Thousands(player.Get_Army_Manpower()));
    Row_Tmp.Population_Label = Gtk::make_managed<Gtk::Label>(" Population: " + Get_Number_In_Thousands(player.Get_Population()));
    Row_Tmp.Capital_Label = Gtk::make_managed<Gtk::Label>(" Capital: " + string(player.Get_Capital_Name()));
    Row_Tmp.points = player.Get_Score();
    Row_Tmp.Flag_Image->set(tmp.Get_Pixbuf());
    Row_Tmp.Flag_Image->set_margin_end(3);
    All_Rows.push_back(Row_Tmp);
    rows++;
    index++;
  }
  sort(All_Rows.begin(), All_Rows.end(), [](auto& a, auto& b){return a.points > b.points;});
  index = 1;
  for(auto& row : All_Rows)
  {
    Attach_Row(row, index);
    index++;
  }
}

void Civs_Dialog::Attach_Row(Row& Row_To_Attach, int index)
{
  Civs_List_Box.attach(*Row_To_Attach.Flag_Image, 0,index-1);
  Civs_List_Box.attach(*Row_To_Attach.Color_Image, 1,index-1);
  Civs_List_Box.attach(*Row_To_Attach.Name_Label, 2,index-1);
  Civs_List_Box.attach(*Row_To_Attach.Points_Label, 3,index-1);
  Civs_List_Box.attach(*Row_To_Attach.Army_Label, 4,index-1);
  Civs_List_Box.attach(*Row_To_Attach.Population_Label, 5,index-1);
  Civs_List_Box.attach(*Row_To_Attach.Capital_Label, 6,index-1);
  Main_Provider.Add_CSS(Row_To_Attach.Name_Label);
  Main_Provider.Add_CSS(Row_To_Attach.Points_Label);
  Main_Provider.Add_CSS(Row_To_Attach.Army_Label);
  Main_Provider.Add_CSS(Row_To_Attach.Population_Label);
  Main_Provider.Add_CSS(Row_To_Attach.Points_Label);
}
