#pragma once

#include <sstream>
#include "AudioChunk.h"

class AudioChunkFactory
{
 public:
  AudioChunkFactory()
  { };
  ~AudioChunkFactory()
  { };

  void clear();
  void setRawData(std::string buffer)
  { _rawVideoData.str(buffer); }
  void setUri(std::string uri)
  { _uri = uri; }

  AudioChunk build();
 private:
  std::stringstream _rawVideoData;
  std::string _uri;

  friend int internalAVFormatRead(void *ptr, uint8_t *buf, int buf_size);
  friend int64_t internalAVFormatSeek(void *ptr, int64_t pos, int which);
};