#include "Twitch.hpp"
#include <boost/network.hpp>
#include <boost/network/protocol.hpp>
#include <boost/format.hpp>
#include <random>
#include <thread>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "Utilities.hpp"

namespace network = boost::network;
namespace http = boost::network::http;
namespace json = rapidjson;


void Twitch::connectToChannel(std::string channel_name)
{
	_channelName = channel_name;
	getAccessTokenAndSig();
	getUsherLinks();
}

void Twitch::getAccessTokenAndSig()
{
	//format the request
	std::string accessUrl = (boost::format(ACCESS_TOKEN_URL_FORMAT_STR) % _channelName).str();
	network::uri::uri accessUri(accessUrl);
	accessUri << network::uri::query("as3", "t");

	http::client::request request(accessUri);
	request << network::header("Connection", "close");
	request << network::header("Accept", "text/json");

	
	//perform a GET
	auto response = _cachedClient.get(request);

	for (auto header : response.headers())
	{
		std::cout << header.first << ": " << header.second << std::endl;
	}
	std::cout << std::endl;

	//convert the body of the response and get the relevent values;
	json::Document doc;
	doc.Parse(response.body().c_str());

	_signature = doc["sig"].GetString();
	_accessToken = doc["token"].GetString();
}

void Twitch::getUsherLinks()
{
	//Generate a random number for the usher request
	std::default_random_engine engine;
	std::uniform_int_distribution<int> dist(0, 1E7);
	int r = dist(engine);

	std::string strUri = (boost::format(USHER_URL_FORMAT_STR) % _channelName).str();
	network::uri::uri uri(strUri);
	
	//generated query values
	uri << network::uri::query(USHER_GET_TOKEN_KEY, escape_json(_accessToken));
	uri << network::uri::query(USHER_GET_SIGNATURE_KEY, _signature);
	uri << network::uri::query(USHER_GET_RANDOM_KEY, std::to_string(r));

	//constant query values
	uri << network::uri::query("player", "twitchweb");
	uri << network::uri::query("$allow_audio_only", "true");
	uri << network::uri::query("allow_source", "true");
	uri << network::uri::query("type", "any");
	//*/

	http::client::request request(uri.string());
	request << network::header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	request << network::header("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:44.0) Gecko/20100101 Firefox/44.0");
	request << network::header("Host", "usher.twitch.tv");
	request << network::header("Accept-Encoding", "gzip, deflate");
	request << network::header("Accept-Language", "en-US,en;q=0.5");
	request << network::header("Connection", "close");


	auto response = _cachedClient.get(request);
	std::cout << response.status() << std::endl;
	for (auto header : response.headers())
	{
		std::cout << header.first << ": " << header.second << std::endl;
	}
	std::cout << response.body() << std::endl;


	std::this_thread::sleep_for(std::chrono::seconds(1));
	
}

void Twitch::getIndexForQuality(Twitch::StreamQuality q)
{

}

void Twitch::setQuality(Twitch::StreamQuality q)
{
	_quality = q;
}