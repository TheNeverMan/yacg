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

void Sound_Manager::Set_Background_Song(string song)
{
  Background_Song.Set_File_Path(song);
}

void Sound_Manager::Play_Background_Song()
{

}


Sound_Manager::Sound_Manager()
{
  Settings_Manager Main_Settings_Manager("miniyacg-config-settings.xml");
  mute = Main_Settings_Manager.Is_Game_Muted();
  if(mute)
    Logger::Log_Warning("Game is muted! Not going to play any sounds!");
  Logger::Log_Info("Opening New Sound Device...");
  ma_device_config deviceConfig;
  Audio_Path Sound_Path("assets/sounds/broken-audio.mp3"); //init with default file to copy parameters
  ma_decoder_init_file(Sound_Path.Get_File_Path().c_str(), NULL, &decoder);
  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format   = decoder.outputFormat;
  deviceConfig.playback.channels = decoder.outputChannels;
  deviceConfig.sampleRate        = decoder.outputSampleRate;
  deviceConfig.dataCallback      = static_cast<void (*)(ma_device *, void *, const void *, unsigned int)>(&Data_Callback);
  deviceConfig.pUserData         = NULL;
  if ((ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) && (ma_device_init(NULL, &deviceConfig, &Background_Device) != MA_SUCCESS))
  {
      Logger::Log_Error("Failed to open playback device.");
      return;
  }
  ma_decoder_uninit(&decoder);
}

Sound_Manager::~Sound_Manager()
{
  try
  {
    ma_device_uninit(&device);
  }
  catch(...)
  {
    Logger::Log_Warning("Uninitializing Sound Device Failed!");
  }
}

void Sound_Manager::Play_Sound(string s_p)
{
  if(mute)
    return;
  try
  {
    if(decoder.data.vfs.file != NULL)
    {
      Logger::Log_Info("Dec Uninit");
      ma_decoder_uninit(&decoder);

    }
  }
  catch(...)
  {
    Logger::Log_Warning("Uninitializing Decoder Failed!");
  }
  Logger::Log_Info("Playing " + s_p);
  Audio_Path Sound_Path(s_p);
  ma_result result;
  result = ma_decoder_init_file(Sound_Path.Get_File_Path().c_str(), NULL, &decoder);
  if (result != MA_SUCCESS)
  {
      Logger::Log_Error("Initializing sound decoder failed!");
      return;
  }

  device.pUserData = &decoder;

  if (ma_device_start(&device) != MA_SUCCESS)
  {
      Logger::Log_Error("Failed to start playback device.");
      ma_device_uninit(&device);
      ma_decoder_uninit(&decoder);
      return;
  }
}
