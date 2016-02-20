#include "Twitch.hpp"
#include <boost/format.hpp>
#include <random>
#include <thread>
#include <rapidjson/document.h>
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "Utilities.hpp"
#include "TwitchStreamPlaylistFactory.hpp"
#include "TwitchStreamFactory.hpp"

namespace json = rapidjson;
namespace network = boost::network;
namespace http = boost::network::http;

Twitch::Twitch()
{
}

Twitch::~Twitch()
{
}

TwitchStream Twitch::openStream(std::string channel_name)
{
	auto token_and_sig = getAccessTokenAndSig(channel_name);
    return getStreamSource(channel_name, token_and_sig.first, token_and_sig.second);
}

std::pair<std::string, std::string> Twitch::getAccessTokenAndSig(std::string channel_name)
{
	//format the request
	std::string accessUrl = (boost::format(ACCESS_TOKEN_URL_FORMAT_STR) % channel_name).str();
	network::uri::uri accessUri(accessUrl);

	_HttpClientType::request request(accessUri);
	request << network::header("Connection", "close");
	request << network::header("Accept", "text/json");

	
	//perform a GET
	auto response = getHttpClient().get(request);


	//convert the body of the response and get the relevent values;
	json::Document doc;
	doc.Parse(response.body().c_str());

	auto signature = doc["sig"].GetString();
	auto accessToken = doc["token"].GetString();

    return std::make_pair(accessToken, signature);
}

TwitchStream Twitch::getStreamSource(std::string channel_name, std::string access_token, std::string signature)
{
	//Generate a random number for the usher request
	std::default_random_engine engine;
	std::uniform_int_distribution<int> dist(0, 1E7);
	int r = dist(engine);

	std::string strUri = (boost::format(USHER_URL_FORMAT_STR) % channel_name).str();
	network::uri::uri uri(strUri);
	
	//generated query values
	uri << network::uri::query(USHER_GET_TOKEN_KEY, url_encode(access_token));
	uri << network::uri::query(USHER_GET_SIGNATURE_KEY, signature);
	uri << network::uri::query(USHER_GET_RANDOM_KEY, std::to_string(r));

	//constant query values
	uri << network::uri::query("player", "twitchweb");
	uri << network::uri::query("$allow_audio_only", "true");
	uri << network::uri::query("allow_source", "true");
	uri << network::uri::query("type", "any");
	//*/

    _HttpClientType::request request(uri.string());
	request << network::header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	request << network::header("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:44.0) Gecko/20100101 Firefox/44.0");
	request << network::header("Host", "usher.twitch.tv");
	request << network::header("Accept-Encoding", "gzip, deflate");
	request << network::header("Accept-Language", "en-US,en;q=0.5");
	request << network::header("Connection", "close");

	auto response = getHttpClient().get(request);

    auto contentType = response.headers().find("Content-Type");
    if(contentType == response.headers().end() || contentType->second.compare("application/vnd.apple.mpegurl") != 0)
    {
        //throw exception
    }

	TwitchStreamFactory twitchStreamFactory;

	twitchStreamFactory.setName(channel_name);
	twitchStreamFactory.setM3U8(response.body());

	return twitchStreamFactory.build();
}