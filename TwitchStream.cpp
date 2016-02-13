#include "TwitchStream.hpp"

TwitchStream::TwitchStream()
{}

TwitchStream::~TwitchStream()
{}

TwitchStreamPlaylist TwitchStream::getPlaylist(TwitchStreamPlaylist::Quality q)
{
	return _playlists.front();
}
