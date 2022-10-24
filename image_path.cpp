#include "image_path.h"

Image_Path::Image_Path(string i_p) : File_Path()
{
  Set_Fallback_File_Path("assets/textures/broken-texture.png");
  Set_File_Path(i_p);
}

Image_Path::Image_Path() : File_Path()
{
  Set_Fallback_File_Path("assets/textures/broken-texture.png");
  Set_File_Path("assets/textures/broken-texture.png");
}
