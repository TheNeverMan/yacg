#pragma once
#include<string>
#include<tuple>
#include<vector>
#include<mutex>

#define DR_WAV_IMPLEMENTATION
#define DRWAV_API static
#define DRWAV_PRIVATE static
#include "dr_wav.h"
#define MA_NO_FLAC
#define DR_MP3_IMPLEMENTATION
#define DRMP3_API static
#define DRMP3_PRIVATE static
#include "dr_mp3.h"
#define MA_API static
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "logger.h"
#include "audio_path.h"
#include "settings_manager.h"
#define MA_DEBUG_OUTPUT

using std::string;
using std::tuple;
using std::make_tuple;
using std::get;
using std::vector;
using std::mutex;
using std::lock_guard;

extern mutex Sound_Mutex;

#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

class Sound_Manager
{
  private:
    inline static ma_decoder Decoders[150];
    inline static bool Ended_Decoders[150] {true};
    inline static ma_device_config Device_Config;
    inline static ma_device Device;
    inline static  ma_event Stop_Event; /* <-- Signaled by the audio thread, waited on by the main thread. */
    inline static ma_decoder_config Decoder_Config;
  public:
    static int Init_Manager()
    {
      Device_Config = ma_device_config_init(ma_device_type_playback);
      Device_Config.playback.format   = SAMPLE_FORMAT;
      Device_Config.playback.channels = CHANNEL_COUNT;
      Device_Config.sampleRate        = SAMPLE_RATE;
      Device_Config.dataCallback      = data_callback;
      Device_Config.pUserData         = NULL;
      Decoder_Config = ma_decoder_config_init(SAMPLE_FORMAT, CHANNEL_COUNT, SAMPLE_RATE);
      if (ma_device_init(NULL, &Device_Config, &Device) != MA_SUCCESS) {
          Logger::Log_Error("Failed to open playback device.\n");
          return -3;
      }
      ma_event_init(&Sound_Manager::Stop_Event);
      if (ma_device_start(&Device) != MA_SUCCESS) {
          ma_device_uninit(&Device);
          Logger::Log_Error("Failed to start playback device.\n");
          return -4;
      }
      return 0;
    }
    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        float* pOutputF32 = (float*)pOutput;

        MA_ASSERT(pDevice->playback.format == SAMPLE_FORMAT);   /* <-- Important for this example. */
        int index = 0;
        while(index < 150)
        {
              if(Ended_Decoders[index])
              {
                index++;
                continue;
              }
              ma_uint32 framesRead = read_and_mix_pcm_frames_f32(&Decoders[index], pOutputF32, frameCount);
              if (framesRead < frameCount) {
                Ended_Decoders[index] = true;
              }
          index++;
        }
        (void)pInput;
    }
    static ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder* pDecoder, float* pOutputF32, ma_uint32 frameCount)
    {
        /*
        The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
        contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
        doing that in this example.
        */
        ma_result result;
        float temp[4096];
        ma_uint32 tempCapInFrames = ma_countof(temp) / CHANNEL_COUNT;
        ma_uint32 totalFramesRead = 0;

        while (totalFramesRead < frameCount) {
            ma_uint64 iSample;
            ma_uint64 framesReadThisIteration;
            ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
            ma_uint32 framesToReadThisIteration = tempCapInFrames;
            if (framesToReadThisIteration > totalFramesRemaining) {
                framesToReadThisIteration = totalFramesRemaining;
            }

          //  Main_Mutex.lock();
            result = ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration, &framesReadThisIteration);
            //Main_Mutex.unlock();
            if (result != MA_SUCCESS || framesReadThisIteration == 0) {
                break;
            }

            /* Mix the frames together. */
            for (iSample = 0; iSample < framesReadThisIteration*CHANNEL_COUNT; ++iSample) {
                pOutputF32[totalFramesRead*CHANNEL_COUNT + iSample] += temp[iSample];
            }

            totalFramesRead += (ma_uint32)framesReadThisIteration;

            if (framesReadThisIteration < (ma_uint32)framesToReadThisIteration) {
                break;  /* Reached EOF. */
            }
        }
        return totalFramesRead;
    }

    static void Play_Sound(string path)
    {
      ma_result result;
      int index = 0;
      while(!Ended_Decoders[index])
        index++;
      lock_guard<mutex> Guard(Sound_Mutex);
      Audio_Path Resource_Path(path);
      Logger::Log_Info("Playing " + Resource_Path.Get_File_Path());
      result = ma_decoder_init_file(Resource_Path.Get_File_Path().c_str(), &Decoder_Config, &Decoders[index]);
      Ended_Decoders[index] = false;
      if (result != MA_SUCCESS) {
         Logger::Log_Error("Failed to load: " + Resource_Path.Get_File_Path());
         ma_decoder_uninit(&Decoders[index]);
      }
    }

    static void Uninit_Manager()
    {
      lock_guard<mutex> Guard(Sound_Mutex);
      int index = 0;
      while(index < 150)
      {
        if(!Ended_Decoders[index])
          ma_decoder_uninit(&Decoders[index]);
        index++;
      }
      ma_device_uninit(&Device);
    }

};
