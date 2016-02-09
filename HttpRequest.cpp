#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{}

HttpRequest::~HttpRequest()
{}

void HttpRequest::setUri(std::string uri)
{
     _uri = uri;
}

void HttpRequest::setMethod(HttpMethod method)
{
     _method = method;
}

void HttpRequest::addHeader(std::string key, std::string value)
{
    _headers[key] = value;
}

void HttpRequest::removeHeader(std::string key)
{
    auto itr = _headers.find(key);
    if(itr != _headers.end())
    {
		_headers.erase(key);
    }
}

std::unordered_map<std::string, std::string> HttpRequest::getHeaders()const
{
    return _headers;
}
