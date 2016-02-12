#include <string>
#include <memory>
#include <map>
#include <boost/network/protocol.hpp>

class Twitch
{
public:
	enum class StreamQuality
	{
		Mobile,
		Low,
		Medium,
		High,
		Source
	};

	Twitch() {};
	~Twitch() {};

	void connectToChannel(std::string channel_name);
	void setQuality(StreamQuality q);
	void stream();
	void stopStreaming();
private:
	std::string _channelName;
	bool _continueStreaming = false;
	std::string _accessToken;
	std::string _signature;
	StreamQuality _quality = StreamQuality::Mobile;
	std::map<StreamQuality, std::string> _usherLinks;
	boost::network::http::client _cachedClient;

	void getAccessTokenAndSig();
	void getUsherLinks();
	void getIndexForQuality(StreamQuality q);

	//To be used with the boost format api. Element 1 is the name of the channel to query.
	const std::string ACCESS_TOKEN_URL_FORMAT_STR = "http://api.twitch.tv/api/channels/%1%/access_token";
	const std::string USHER_URL_FORMAT_STR = "http://usher.twitch.tv/api/channel/hls/%1%.m3u8";

	const std::string USHER_GET_TOKEN_KEY = "token";
	const std::string USHER_GET_SIGNATURE_KEY = "sig";
	const std::string USHER_GET_RANDOM_KEY = "p";
};