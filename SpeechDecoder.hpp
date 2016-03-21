#pragma once
class AudioChunk;
class SpeechDecoder
{
 private:
  ps_decoder_t *_decoder = nullptr;
  cmd_ln_t *_options = nullptr;
 public:
  SpeechDecoder(const std::string &modelDir);
  ~SpeechDecoder();

  void setLogFile(const std::string &file);
  void setAudioSampleRate(const unsigned int &rate);

  std::tuple<std::string, double> processChunk(const AudioChunk&);

};
