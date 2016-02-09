#include "HttpClient.hpp"
#include <sstream>

using boost::asio::ip::tcp;

HttpClient::HttpClient():_service(), _socket(_service), _resolver(_service), _host("")
{
}

HttpClient::~HttpClient()
{
}

bool HttpClient::connect(std::string host)
{
    tcp::resolver::query connectQuery(host, "http");
    auto endpointItr = _resolver.resolve(connectQuery);
    auto result = boost::asio::connect(_socket, endpointItr);
    bool ret = result != tcp::resolver::iterator();
    if(ret)
    {
        _host = host;
    }
    return ret;
}

bool HttpClient::disconnect()
{
    if(_socket.is_open())
    {
        _socket.shutdown(tcp::socket::shutdown_both);
        _socket.close();
        _host = "";
        return true;
    }
    return false;
}

std::string HttpClient::translateMethod(HttpRequest::HttpMethod method)
{
    switch(method)
    {
    default:
    case HttpRequest::GET:
        return "GET";
    case HttpRequest::POST:
        return "POST";
    case HttpRequest::PUT:
        return "PUT";
    case HttpRequest::DELETE:
        return "DELETE";
    }
}

std::string HttpClient::serializeRequest(const HttpRequest& r)
{
    std::stringstring request;
    
    request << translateMethod(r.getMethod()) << " " << r.getUri() << " HTTP/1.1" << "\r\n"
    
    for(auto header : r.getHeaders)
    {
        request << header.first << ": " << header.second << "\r\n";
    }

    request << "\r\n"
    
    if(r.getData().size() != 0)
    {
        request << r.getData();
    }

    return request.str();
}

bool HttpClient::executeRequest(const HttpRequest& request, std::function<void(const HttpResponse&)>)
{
    if(!_socket.is_open())
    {
        return false;
    }
    std::string serializedRequest = serializeRequest(request):
    
    //write returns number of bytes transferred, confirm with request size
    auto result = boost::asio::write(_socket, boost::asio::buffer(serializedRequest.c_str(), serializedRequest.size()));
    if(result != serializedRequest.size())
    {
        return false;
    }

    
}
