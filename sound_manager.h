#pragma once
#include<string>

#define DR_FLAC_IMPLEMENTATION
#include "audio-libs/dr_flac.h"  /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "audio-libs/dr_mp3.h"   /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "audio-libs/dr_wav.h"   /* Enables WAV decoding. */
#define MINIAUDIO_IMPLEMENTATION
#include "audio-libs/miniaudio.h"
#include "logger.h"
#include "audio_path.h"

using namespace std;

class Sound_Manager
{
  public:
    //void Data_Callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    void Play_Sound(string s_p);
};
