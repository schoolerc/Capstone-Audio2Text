#pragma once
#include <tuple>
#include <string>
class AudioChunk;
class AudioChunkFactory
{
 public:
  AudioChunkFactory()
  { }
  ~AudioChunkFactory();

  void clear();
  void setRawData(std::string buffer)
  { _rawVideoData.str(buffer); }
  void setUri(std::string uri)
  { _uri = uri; }

  std::tuple<std::string, double> build();
 private:
  std::stringstream _rawVideoData;
  std::string _uri;
  AVIOContext *_ioContext = nullptr;
  AVFormatContext *_formatContext = nullptr;
  void *_internalIOBuffer = nullptr;
  int _audioStream = -1;
  AVCodec* _audioCodec = nullptr;
  AVCodecContext* _audioCodecContext = nullptr;
  std::vector<unsigned char> _audioBuffer;


  void loadMediaFile();
  void openAudioStream();
  void processAudioData();

  friend int internalAVFormatRead(void *ptr, uint8_t *buf, int buf_size);
  friend int64_t internalAVFormatSeek(void *ptr, int64_t pos, int which);
};