#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{}

HttpResponse::~HttpResponse()
{}

std::unordered_map<std::string, std::string> HttpResponse::getHeaders()
{
    return _headers;
}

std::string HttpResponse::getUri()
{
    return _uri;
}

std::string HttpResponse::getContent()
{
    return _body;
}

std::string HttpResponse::getHost()
{
    return _host;
}

std::string HttpResponse::getHeader(std::string key)
{
    auto itr = _headers.find(key);

    if(itr != _headers.end())
    {
        return itr->second;
    }
    return "";
}
