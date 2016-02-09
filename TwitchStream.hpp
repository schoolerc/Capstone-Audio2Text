#include <string>
class TwitchStreamPlaylist;
class TwitchStream
{
public:
	TwitchStream(std::string streamName);
	~TwitchStream();

	TwitchStreamPlaylist getPlaylist(std::string quality);

private:
	std::string _streamName;
};