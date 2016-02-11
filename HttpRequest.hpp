#pragma once
#undef DELETE
#include <string>
#include <unordered_map>
class HttpRequest
{
public:
    enum class HttpMethod
    {
	GET,
	POST,
	PUT,
	DELETE
    };

    HttpRequest();
    ~HttpRequest();
    void setUri(std::string uri);
    void setMethod(HttpMethod method);
    void addHeader(std::string key, std::string value);
    void removeHeader(std::string key);
    void setData(std::string data);

    std::string getHeader(std::string key)const;
    std::unordered_map<std::string, std::string> getHeaders()const;
	std::string getUri()const { return _uri; }
	HttpMethod getMethod()const { return _method; }
	std::string getData()const { return _data; }
private:
    std::unordered_map<std::string, std::string> _headers;
    std::string _uri;
    std::string _data;
    HttpMethod _method;

};