#pragma once
#include<string>
#include<tuple>
#include<vector>

#define DR_WAV_IMPLEMENTATION
#define DRWAV_API static
#define DRWAV_PRIVATE static
#include "dr_wav.h"
#define MA_NO_FLAC
#define MA_NO_MP3
#define MA_API static
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "logger.h"
#include "audio_path.h"
#include "settings_manager.h"

using std::string;
using std::tuple;
using std::make_tuple;
using std::get;
using std::vector;

class Sound_Manager
{
  private:
    ma_decoder decoder;
    ma_device device;
    bool mute;
    ma_decoder Background_Loop;
    ma_device Background_Device;
    Audio_Path Background_Song;
  public:
    //void Data_Callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    void Play_Sound(string s_p);
    Sound_Manager();
    ~Sound_Manager();
    void Set_Background_Song(string song);
    void Play_Background_Song();
};
