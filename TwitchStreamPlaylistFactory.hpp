#pragma once
#include "TwitchStreamPlaylist.hpp"

class TwitchStreamPlaylistFactory
{
public:
    TwitchStreamPlaylistFactory(){};
    ~TwitchStreamPlaylistFactory(){};

    void setMediaInfoStr(std::string media_info){_media_info = media_info;}
    void setStreamInfoStr(std::string stream_info){_stream_info = stream_info;}
    void setIndexFileUri(std::string uri){_uri = uri;}

    void clear();

    TwitchStreamPlaylist build();

private:
    std::string _media_info;
    std::string _stream_info;
    std::string _uri;

    TwitchStreamPlaylist::Quality quality(std::string);
};