#include "gtk_game_map.h"

Gtk_Game_Map::Gtk_Game_Map(int x_s, int y_s, int t_s)
{
  tile_size = t_s;
  is_in_thread = false;
  x_size = x_s;
  y_size = y_s;
  add_events(Gdk::BUTTON_PRESS_MASK);
}

Gtk_Game_Map::Gtk_Game_Map()
{
  is_in_thread = false;
}

void Gtk_Game_Map::Set_Tile_Size(int t_s)
{
  tile_size = t_s;
}

int Gtk_Game_Map::Get_Tile_Size()
{
  return tile_size;
}

void Gtk_Game_Map::Force_Redraw()
{
  auto win = get_window();
  if (win)
  {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
    win->invalidate_rect(r, false);
  }
}

void Gtk_Game_Map::Add_Combat_Overlay(array<int, 2> Coords)
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Get_Gtk_Tile(Coords[0], Coords[1])->Add_Combat_Overlay();
  Force_Redraw();
}

void Gtk_Game_Map::Add_Selection_Overlay(array<int, 2> Coords)
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Get_Gtk_Tile(Coords[0], Coords[1])->Add_Selection_Overlay();
  Force_Redraw();
}

void Gtk_Game_Map::Set_Map_Size(int x_s, int y_s)
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  x_size = x_s;
  y_size = y_s;
}

void Gtk_Game_Map::Update_Tile(vector<string_view> Textures, guint32 border_color, int x, int y)
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  Game_Map[y + (x * y_size)]->Update_Texture(Textures, border_color);
  Force_Redraw();
}

void Gtk_Game_Map::Zoom_In()
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  tile_size = tile_size + 4;
  for_each(Game_Map.begin(), Game_Map.end(), [&](shared_ptr<Gtk_Tile> Tile){Tile->Increase_Tile_Size();});
  Force_Redraw();
}

void Gtk_Game_Map::Zoom_Out()
{
  if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
  tile_size = tile_size - 4;
  for_each(Game_Map.begin(), Game_Map.end(), [&](shared_ptr<Gtk_Tile> Tile){Tile->Decrease_Tile_Size();});
  Force_Redraw();
}

void Gtk_Game_Map::Set_City_Overlay(array<int, 2> Coords, string city_name)
{
  Get_Gtk_Tile(Coords[0], Coords[1])->Set_City_Name(city_name);
}

void Gtk_Game_Map::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, array<int,2> Coords, string text)
{
  Pango::FontDescription font;

  font.set_family("Connection II");
  font.set_weight(Pango::WEIGHT_BOLD);

// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
auto layout = create_pango_layout(text);

layout->set_font_description(font);

int text_width;
int text_height;

//get the text dimensions (it updates the variables -- by reference)
layout->get_pixel_size(text_width, text_height);

// Position the text in the middle
cr->move_to(Coords[0], Coords[1]);

layout->show_in_cairo_context(cr);
}

bool Gtk_Game_Map::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  set_size_request(tile_size * x_size, tile_size * y_size);
  int start = 0;
  int start_y = 0;
  long long int index = 0;
  while(start < x_size)
  {
    while(start_y < y_size)
    {
      Glib::RefPtr<Gdk::Pixbuf> Tile_Pixbuf = Game_Map[index]->Get_Pixbuf();
      Gdk::Cairo::set_source_pixbuf(cr, Tile_Pixbuf, start * tile_size, start_y * tile_size);
      cr->rectangle(start * tile_size, start_y * tile_size, Tile_Pixbuf->get_width(), Tile_Pixbuf->get_height());
      cr->fill();
      start_y++;
      index++;
    }
    start_y = 0;
    start++;
  }
  start = 0;
  start_y = 0;
  index = 0;
  while(start < x_size)
  {
    while(start_y < y_size)
    {
      if(Game_Map[index]->Has_City_Set())
      {
        cr->set_source_rgb(1.0, 1.0, 1.0);
        draw_text(cr, {tile_size * start - tile_size / 2, static_cast<int>(tile_size * start_y + (tile_size * 0.75))}, Game_Map[index]->Get_City_Name());
      }
      start_y++;
      index++;
    }
    start_y = 0;
    start++;
  }
  return true;
}

void Gtk_Game_Map::Generate_Map(Game* Main_Game, Magic_Map_Generation_Thread_Communicator* Thread_Portal)
{

    is_in_thread = true;
    long long int tiles;
    long long int start;
    long long int tiles_processed;
    long long int start_y;
    int percent;
    {
      if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
      tiles = x_size * y_size;
      tiles_processed = 0;
      start = 0;
      start_y = 0;
      percent = tiles / 100;
    }
    while(start < x_size)
    {
      while(start_y < y_size)
      {
        {
          if(is_in_thread){lock_guard<mutex> Lock(Main_Mutex);}
          string tile_texture = Main_Game->Get_Map()->Get_Tile(start,start_y).Get_Texture_Path(); //this is incredibly slow pls fix
          string unit_texture = assets_directory_path + "textures" + path_delimeter + "upgrades" + path_delimeter + "none-upgrade-texture.png";
          if(Main_Game->Get_Map()->Get_Tile(start,start_y).Has_Unit())
            unit_texture = Main_Game->Get_Player_By_Id(Main_Game->Get_Map()->Get_Tile(start,start_y).Get_Unit_Owner_Id())->Get_Unit_On_Tile(start,start_y).Get_Texture_Path();
          string upgrade_texture = Main_Game->Get_Upgrade_By_Name(Main_Game->Get_Map()->Get_Upgrade(start,start_y)).Get_Texture_Path();
          guint32 border_color = Main_Game->Get_Border_Color_By_Player_Id(Main_Game->Get_Map()->Get_Owner(start,start_y));

          vector<string> Textures {tile_texture, upgrade_texture, unit_texture};
          auto tmp = make_shared<Gtk_Tile>(Textures, border_color, tile_size);
          Game_Map.push_back(tmp);
          //cout << start_y << " " << start << endl;
          start_y++;
          tiles_processed++;
        }
        if(tiles_processed % percent == 0)
        {
          //cout << start << " " << start_y << " " << percent << endl;
          Thread_Portal->Notify_Dialog();

        }
      }
      start_y = 0;
      start++;
    }
    is_in_thread = false;
}

shared_ptr<Gtk_Tile> Gtk_Game_Map::Get_Gtk_Tile(int x, int y) const
{
  return Game_Map[y + (x * y_size)];
}
