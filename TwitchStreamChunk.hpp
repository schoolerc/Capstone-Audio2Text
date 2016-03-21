#pragma once
#include <string>
#include <sstream>
#include <memory>
#include "AudioChunk.h"

class TwitchStreamChunk
{
  std::string _uri;
  double _duration;
  std::string _hypothesis;
  double _confidence;
  std::string _timestamp = "00:00:00";
  friend class TwitchStreamChunkFactory;

 public:
  TwitchStreamChunk()
  { }
  ~TwitchStreamChunk()
  { }

  std::string getUri()
  { return _uri; }
  double getDuration()
  { return _duration; }
  std::string getHyp()
  { return _hypothesis; }
  double getConfidence()
  { return _confidence; }
  std::string getTimestamp()
  { return _timestamp; }
  void download();
};