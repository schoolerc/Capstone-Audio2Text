#include "TwitchStream.hpp"
#include "TwitchStreamPlaylist.hpp"

TwitchStream::TwitchStream(std::string streamName):_streamName(streamName)
{}

TwitchStream::~TwitchStream()
{}

TwitchStreamPlaylist TwitchStream::getPlaylist(std::string quality)
{
	return TwitchStreamPlaylist();
}
