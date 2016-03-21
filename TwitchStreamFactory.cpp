#include "prefix.hpp"
#include "TwitchStreamFactory.hpp"
#include "TwitchStreamPlaylistFactory.hpp"

void TwitchStreamFactory::clear()
{
    _playlists.clear();
    _name.clear();
}

void TwitchStreamFactory::setName(std::string name)
{
    _name = name;
}

void TwitchStreamFactory::setM3U8(std::string m3u8)
{
    _m3u8 = m3u8;
}

TwitchStream TwitchStreamFactory::build()
{
    TwitchStream result;
    result._streamName = _name;

    std::stringstream stream(_m3u8);

    std::string m3uLine;

    //ditch first two lines, there's no really important information
    std::getline(stream, m3uLine);
    std::getline(stream, m3uLine);

    TwitchStreamPlaylistFactory playlistFactory(result);
    while(stream.good())
    {
        playlistFactory.clear();
        std::string mediaInfo, streamInfo, streamUri;

        if(!(std::getline(stream, mediaInfo) && std::getline(stream, streamInfo) && std::getline(stream, streamUri)))
        {
            break;
        }
        playlistFactory.setIndexFileUri(streamUri);
        playlistFactory.setMediaInfoStr(mediaInfo);
        playlistFactory.setStreamInfoStr(streamInfo);

        result._playlists.push_back(playlistFactory.build());
    }

    return result;
}