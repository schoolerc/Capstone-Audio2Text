#pragma once
#include "TwitchStreamChunk.hpp"
class TwitchStream;

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

  void stream(std::unique_ptr<sql::Connection> &);

  ~TwitchStreamPlaylist();
 private:
  int _bandwidth;
  Resolution _resolution;
  Quality _quality;
  std::string _uri;
  std::vector<TwitchStreamChunk> _prevChunks;
  TwitchStream& _stream;

  TwitchStreamPlaylist(TwitchStream& _stream);

  std::vector<TwitchStreamChunk> getCurrentChunks();
  void downloadChunks(std::vector<TwitchStreamChunk> &chunks, std::unique_ptr<sql::Connection>& connection);
  void storeChunks(std::vector<TwitchStreamChunk>&chunks, std::unique_ptr<sql::Connection>& connection);

  friend class TwitchStreamPlaylistFactory;
};