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

using namespace std;

class Sound_Manager
{
  private:
    ma_decoder decoder;
    ma_device device;
    bool mute;
  public:
    //void Data_Callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    void Play_Sound(string s_p);
    Sound_Manager();
    ~Sound_Manager();
};
