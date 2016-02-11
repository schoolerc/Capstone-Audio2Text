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
	request.setUri("/");
	request.setMethod(HttpRequest::HttpMethod::GET);
	request.setData("");

	request.addHeader("Host", "www.twitch.tv");
	request.addHeader("Accept", "text/html");
	request.addHeader("Connection", "keep-alive");

	HttpClient client;

	try
	{
		client.connect("www.twitch.tv");
		HttpResponse r = client.executeRequest(request);
		std::cout << r.getResponseCode();
		std::cout << r.getContent();
	}
	catch (boost::exception& ex)
	{
		boost::diagnostic_information(ex);
	}
}