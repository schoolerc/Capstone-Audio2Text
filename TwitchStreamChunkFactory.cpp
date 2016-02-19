#include <sstream>
#include "TwitchStreamChunkFactory.hpp"
#include "TwitchStreamPlaylist.hpp"

void TwitchStreamChunkFactory::clear()
{
  _m3u8.clear();
  _src = nullptr;
}

void TwitchStreamChunkFactory::setM3U8(std::string m3u8)
{
  _m3u8 = m3u8;
}

void TwitchStreamChunkFactory::setSource(TwitchStreamPlaylist *src)
{
  _src = src;
}

std::vector<TwitchStreamChunk> TwitchStreamChunkFactory::build()
{
  if (_src == nullptr)
  {
    //TODO: throw exception
  }
  std::vector<TwitchStreamChunk> chunks;
  std::string baseUri = getBaseUri();

  std::stringstream m3u8Stream(_m3u8);
  std::string m3u8Line;
  while (std::getline(m3u8Stream, m3u8Line))
  {
    //Look for meta-data tag describing length of a chunk
    auto colonPos = m3u8Line.find(':');
    if (m3u8Line.substr(0, colonPos).compare("#EXTINF") == 0)
    {
      //found a chunk

      //get the duration
      std::string durationString = m3u8Line.substr(colonPos + 1);
      durationString.pop_back();

      int duration = std::atoi(durationString.c_str());

      //get the last part of the uri
      std::string filename;
      std::getline(m3u8Stream, filename);

      std::string fullUri;
      fullUri.append(baseUri);
      fullUri.append(filename);

      TwitchStreamChunk chunk;
      chunk._duration = duration;
      chunk._uri = fullUri;

      chunks.push_back(chunk);
    }
  }
  return chunks;
}

std::string TwitchStreamChunkFactory::getBaseUri()
{
  std::string uri = _src->getUri();
  auto pos = uri.find_last_of("/");

  return uri.substr(0, pos + 1);
}