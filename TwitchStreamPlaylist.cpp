#include <iostream>
#include <thread>
#include "TwitchStreamPlaylist.hpp"
#include "TwitchStreamChunkFactory.hpp"
#include "Utilities.hpp"

namespace network = boost::network;
namespace http = boost::network::http;

TwitchStreamPlaylist::TwitchStreamPlaylist()
{}

TwitchStreamPlaylist::~TwitchStreamPlaylist()
{}

std::vector<TwitchStreamChunk> TwitchStreamPlaylist::getCurrentChunks()
{
    TwitchStreamChunkFactory chunkFactory;

    _HttpClientType::request request(_uri);
    auto response = getHttpClient().get(request);

    if(response.status() != 200)
    {
        //TODO: Throw exception
    }

    chunkFactory.setSource(this);
    chunkFactory.setM3U8(response.body());

    auto chunks = chunkFactory.build();
    return chunks;
}

void TwitchStreamPlaylist::stream()
{
  ps_start_stream(getDecoder());
  while(true)
  {
    auto currentChunks = getCurrentChunks();
    std::vector<TwitchStreamChunk> chunksToDownload(currentChunks.begin(), currentChunks.end());

    std::remove_if(chunksToDownload.begin(), chunksToDownload.end(), [this](TwitchStreamChunk &chunk)
    {
      auto itr = std::find_if(_prevChunks.begin(), _prevChunks.end(), [&chunk](TwitchStreamChunk &c)
      {
        return chunk.getUri().compare(c.getUri()) == 0;
      });

      return itr != _prevChunks.end();
    });

    downloadChunks(chunksToDownload);


    _prevChunks = currentChunks;
  }
}

void TwitchStreamPlaylist::downloadChunks(std::vector<TwitchStreamChunk> &chunks)
{
    for(auto c : chunks)
    {
        c.download();
    }
}