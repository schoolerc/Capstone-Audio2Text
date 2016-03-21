#pragma once
#include "TwitchStreamPlaylist.hpp"
class TwitchStreamPlaylist;
class TwitchStream
{
public:
    ~TwitchStream();

	TwitchStreamPlaylist getPlaylist(TwitchStreamPlaylist::Quality);
    std::string streamName() {return _streamName;};

private:
    TwitchStream();

	std::string _streamName;
	std::vector<TwitchStreamPlaylist> _playlists;

    friend class TwitchStreamFactory;
};