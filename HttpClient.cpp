#include "HttpClient.hpp"
#include "Exception.hpp"
#include <sstream>
#include <string>

using boost::asio::ip::tcp;

HttpClient::HttpClient():_service(), _socket(_service), _resolver(_service), _host("")
{
}

HttpClient::~HttpClient()
{
}

void HttpClient::connect(std::string host)
{
	tcp::resolver::query connectQuery(host, "http");
	auto endpointItr = _resolver.resolve(connectQuery);

	try
	{
		boost::asio::connect(_socket, endpointItr);
	}
	catch (const boost::exception& ex)
	{
		ex << erraction_info("Initiating a tcp connection") << errhost_info(host);
		throw;
	}
	
}

void HttpClient::disconnect()
{
	try {
		_socket.shutdown(tcp::socket::shutdown_both);
		_socket.close();
		_host = "";
	}
	catch (const boost::exception& ex)
	{
		ex << erraction_info("Disconnecting a socket") << errhost_info(_host);
		throw;
	}
}

std::string HttpClient::translateMethod(HttpRequest::HttpMethod method)
{
    switch(method)
    {
    default:
	case HttpRequest::HttpMethod::GET:
        return "GET";
	case HttpRequest::HttpMethod::POST:
        return "POST";
	case HttpRequest::HttpMethod::PUT:
        return "PUT";
	case HttpRequest::HttpMethod::DELETE:
        return "DELETE";
    }
}

std::string HttpClient::serializeRequest(const HttpRequest& r)
{
    std::stringstream request;
    
	request << translateMethod(r.getMethod()) << " " << r.getUri() << " HTTP/1.1" << "\r\n";
    
    for(auto header : r.getHeaders())
    {
        request << header.first << ": " << header.second << "\r\n";
    }

	request << "\r\n";
    
    if(r.getData().size() != 0)
    {
        request << r.getData();
    }

    return request.str();
}

void HttpClient::sendRequest(const HttpRequest& r)
{
	std::string raw = serializeRequest(r);
	try 
	{
		boost::asio::write(_socket, boost::asio::buffer(raw));
	}
	catch (boost::exception& ex)
	{
		ex << erraction_info("Writing http request.") << errhost_info(_host) << errrequest_info(raw);
		throw;
	}
}

void HttpClient::parseHeaders(HttpResponse& response, std::istream& input)
{
	std::stringstream stream;
	stream << input.rdbuf();

	std::string tmp;
	stream >> tmp;

	int responseCode;
	stream >> responseCode;

	response._responseCode = responseCode;

	std::string header;
	while (std::getline(stream, header))
	{
		std::string key;
		std::getline(stream, key, ':');

		std::string value;
		std::getline(stream, value);
		response._headers[key] = value;
	}
}

HttpResponse HttpClient::receiveResponse()
{
	boost::asio::streambuf headerBuf;
	boost::asio::streambuf dataBuf;
	try
	{
		//Read all http response headers
		boost::asio::read_until(_socket, headerBuf, "\r\n\r\n");
		std::istream responseHeaders(&headerBuf);
		HttpResponse response;

		parseHeaders(response, responseHeaders);

		size_t contentLength = std::stoul(response.getHeader("Content-Length"));

		boost::asio::streambuf::mutable_buffers_type tmp = dataBuf.prepare(contentLength);
		boost::asio::read(_socket, tmp);
		dataBuf.commit(contentLength);
		std::istream responseData(&dataBuf);

		response._body = std::string((std::istreambuf_iterator<char>(&dataBuf)), std::istreambuf_iterator<char>());
		return response;
	}
	catch (boost::exception& ex)
	{
		std::string tmp1((std::istreambuf_iterator<char>(&headerBuf)), std::istreambuf_iterator<char>());
		std::string tmp2((std::istreambuf_iterator<char>(&dataBuf)), std::istreambuf_iterator<char>());
		tmp1.append("\r\n\r\n");
		tmp1.append(tmp2);
		ex << errhost_info(_host) << erraction_info("Receiving http response.") << errresponse_info(tmp1);
		throw;
	}
}

HttpResponse HttpClient::executeRequest(const HttpRequest& request)
{
	try
	{
		sendRequest(request);
		return receiveResponse();
	}
	catch (boost::exception& ex)
	{
		throw;
	}
}