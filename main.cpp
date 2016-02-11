#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpClient.hpp"
#include "Exception.hpp"

int main(int argc, const char* argv[])
{
	HttpRequest request;
	request.setUri("/api/channels/ClintStevens/access_token");
	request.setMethod(HttpRequest::HttpMethod::GET);
	request.setData("");

	request.addHeader("Host", "api.twitch.tv");
	request.addHeader("Accept-Encoding", "gzip");
	request.addHeader("Accept", "text/html");
	request.addHeader("Connection", "keep-alive");

	HttpClient client;

	try
	{
		client.connect("api.twitch.tv");
		HttpResponse r = client.executeRequest(request);
		std::cout << r.getResponseCode() << std::endl;;
		std::cout << r.getContent();
		std::this_thread::sleep_for(std::chrono::minutes(5));
	}
	catch (boost::exception& ex)
	{
		boost::diagnostic_information(ex);
	}
	return 0;
}