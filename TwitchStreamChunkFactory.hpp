#pragma once
#include <string>
#include "TwitchStreamChunk.hpp"
#include <vector>

class TwitchStreamPlaylist;
class TwitchStreamChunkFactory
{
 public:
  TwitchStreamChunkFactory()
  { }
  ~TwitchStreamChunkFactory()
  { }

  void clear();

  void setM3U8(std::string);
  void setSource(TwitchStreamPlaylist *);

  std::vector<TwitchStreamChunk> build();

 private:
  std::string _m3u8;
  TwitchStreamPlaylist *_src;

  std::string getBaseUri();
};