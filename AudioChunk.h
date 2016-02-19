#pragma once
#include <vector>

class AudioChunk
{
 private:
  unsigned char *_buffer = nullptr;
  unsigned int _size;
  int _duration;

  friend class AudioChunkFactory;

 public:
  AudioChunk();
  AudioChunk(const AudioChunk &);

  ~AudioChunk();

  AudioChunk combine(const AudioChunk &);

  unsigned char *buffer()
  { return _buffer; }
  unsigned int size()
  { return _size; }
  int duration()
  { return _duration; }
};
