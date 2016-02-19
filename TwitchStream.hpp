#pragma once
#include <string>
#include <vector>
#include "TwitchStreamPlaylist.hpp"
class TwitchStream
{
 public:
  ~TwitchStream();

  TwitchStreamPlaylist getPlaylist(TwitchStreamPlaylist::Quality);
  std::string streamName();

 private:
  TwitchStream();

  std::string _streamName;
  std::vector<TwitchStreamPlaylist> _playlists;

  friend class TwitchStreamFactory;
};