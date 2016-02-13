#pragma once
#include <string>
#include <memory>
#include <map>
#include <boost/network/protocol.hpp>
#include <utility>
#include "TwitchStream.hpp"
class TwitchStreamFactory;
class Twitch
{
public:

	Twitch();
	~Twitch();

	TwitchStream openStream(std::string channel_name);
private:


	std::pair<std::string, std::string> getAccessTokenAndSig(std::string channel_name);
	TwitchStream getStreamSource(std::string channel_name, std::string access_token, std::string signature);
	void parseM3U8(std::string, TwitchStreamFactory&);

	//To be used with the boost format api. Element 1 is the name of the channel to query.
	const std::string ACCESS_TOKEN_URL_FORMAT_STR = "http://api.twitch.tv/api/channels/%1%/access_token";
	const std::string USHER_URL_FORMAT_STR = "http://usher.twitch.tv/api/channel/hls/%1%.m3u8";

	const std::string USHER_GET_TOKEN_KEY = "token";
	const std::string USHER_GET_SIGNATURE_KEY = "sig";
	const std::string USHER_GET_RANDOM_KEY = "p";
};