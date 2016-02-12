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
	std::string _accessToken;
	std::string _signature;


	/*
		Item 1: the name of the twitch channel
		Item 2: the token retrieved from the access token api call
		Item 3: the signature retrieved from the access token api call
		Item 4: a random number
	*/
	const std::string ACCESS_TOKEN_FORMAT_STR = "http://api.twitch.tv/api/channels/%1%/access_token";
	const std::string USHER_FORMAT_STR = "http://usher.twitch.tv/api/channel/hls/%1%.m3u8?player=twitchweb&token=%2%&sig=%3%&$allow_audio_only=true&allow_source=true&type=any&p=%4%";
};