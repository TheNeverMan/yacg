#include "sound_manager.h"

void Data_Callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
  if (pDecoder == NULL) {
      return;
  }

  ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

  (void)pInput;
}

void Sound_Manager::Play_Sound(string s_p)
{
  Audio_Path Sound_Path(s_p);
  ma_result result;
  ma_decoder decoder;
  ma_device_config deviceConfig;
  ma_device device;

  result = ma_decoder_init_file(Sound_Path.Get_File_Path().c_str(), NULL, &decoder);
  if (result != MA_SUCCESS)
  {
      Logger::Log_Error("Initializing sound decoder failed!");
      return;
  }

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format   = decoder.outputFormat;
  deviceConfig.playback.channels = decoder.outputChannels;
  deviceConfig.sampleRate        = decoder.outputSampleRate;
  deviceConfig.dataCallback      = static_cast<void (*)(ma_device *, void *, const void *, unsigned int)>(&Data_Callback);
  deviceConfig.pUserData         = &decoder;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
  {
      Logger::Log_Error("Failed to open playback device.");
      ma_decoder_uninit(&decoder);
      return;
  }

  if (ma_device_start(&device) != MA_SUCCESS)
  {
      Logger::Log_Error("Failed to start playback device.");
      ma_device_uninit(&device);
      ma_decoder_uninit(&decoder);
      return;
  }
  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);
}
