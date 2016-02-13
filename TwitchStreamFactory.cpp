#include "TwitchStreamFactory.hpp"

void TwitchStreamFactory::clear()
{
    _playlists.clear();
    _name.clear();
}

void TwitchStreamFactory::addPlaylist(TwitchStreamPlaylist playlist)
{
    _playlists.push_back(playlist);
}

void TwitchStreamFactory::setName(std::string name)
{
    _name = name;
}

TwitchStream TwitchStreamFactory::build()
{
    TwitchStream result;
    result._playlists = _playlists;
    result._streamName = _name;

    return result;
}