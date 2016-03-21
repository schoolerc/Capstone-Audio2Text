#include "prefix.hpp"
#include "TwitchStream.hpp"

TwitchStream::TwitchStream()
{}

TwitchStream::~TwitchStream()
{}

TwitchStreamPlaylist TwitchStream::getPlaylist(TwitchStreamPlaylist::Quality q)
{
	auto itr = std::find_if(_playlists.begin(), _playlists.end(), [q](TwitchStreamPlaylist& playlist)
	{
		return playlist.getQuality() == q;
	});

	if(itr == _playlists.end())
	{
		//TODO: throw exception here
	}

    return *itr;
}

