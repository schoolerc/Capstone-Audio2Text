#pragma once
#include <string>
#include <deque>
#include <boost/network/protocol.hpp>
#include "TwitchStreamChunk.hpp"

/*
 * \brief Class that represents a specific quality of a twitch stream.
 */
class TwitchStreamPlaylist
{
 public:
  enum class Quality
  {
    Mobile,
    Low,
    Medium,
    High,
    Source
  };

  struct Resolution
  {
    int _width;
    int _height;
  };

  int getBandwith()
  { return _bandwidth; }
  Resolution getResolution()
  { return _resolution; }
  Quality getQuality()
  { return _quality; }
  std::string getUri()
  { return _uri; }

  void stream();

  ~TwitchStreamPlaylist();
 private:
  int _bandwidth;
  Resolution _resolution;
  Quality _quality;
  std::string _uri;
  std::vector<TwitchStreamChunk> _prevChunks;

  TwitchStreamPlaylist();

  std::vector<TwitchStreamChunk> getCurrentChunks();
  void downloadChunks(std::vector<TwitchStreamChunk> &chunks);

  friend class TwitchStreamPlaylistFactory;
};