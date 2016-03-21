#pragma once
#include "TwitchStream.hpp"
#include "TwitchStreamPlaylist.hpp"

class TwitchStreamFactory
{
public:
    TwitchStreamFactory(){}
    ~TwitchStreamFactory(){}

    void setName(std::string);
    void setM3U8(std::string);

    void clear();

    TwitchStream build();

private:
    std::string _m3u8;
    std::vector<TwitchStreamPlaylist> _playlists;
    std::string _name;
};