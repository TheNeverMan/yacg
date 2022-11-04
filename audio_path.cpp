#include "audio_path.h"

Audio_Path::Audio_Path(string a_p) : File_Path()
{
  Set_Fallback_File_Path("assets/sounds/broken-audio.wav");
  Set_File_Path(a_p);
}

Audio_Path::Audio_Path() : File_Path()
{
  Set_Fallback_File_Path("assets/sounds/broken-audio.wav");
  Set_File_Path("assets/sounds/broken-audio.wav");
}
