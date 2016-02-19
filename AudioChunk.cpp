#include "AudioChunk.h"
#include <cstdlib>
#include <string.h>

AudioChunk::AudioChunk()
{ }

AudioChunk::~AudioChunk()
{
  if (_buffer != nullptr)
  {
    //free(_buffer);
  }
}

AudioChunk::AudioChunk(const AudioChunk &src)
{
  _duration = src._duration;
  _size = src._size;

  _buffer = src._buffer;
}

AudioChunk AudioChunk::combine(const AudioChunk &chunk)
{
  AudioChunk result;
  result._duration = _duration + chunk._duration;
  result._size = _size + chunk._size;

  result._buffer = (unsigned char *) malloc(result._size);

  memcpy(result._buffer, _buffer, _size);
  memcpy(result._buffer + _size, chunk._buffer, chunk._size);

  return result;
}