#include <boost/asio.hpp>
#include <functional>
#include <thread>
#include <memory>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
class HttpClient
{
private:
    boost::asio::io_service _service;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver _resolver;
    std::string _host;

    std::string translateMethod(HttpRequest::HttpMethod);
    std::string serializeRequest(const HttpRequest&);
    HttpResponse deserializeResponse(const std::string&);
public:
    HttpClient();
    ~HttpClient();
    bool connect(std::string host);
    bool disconnect();
    bool isConnected();
	HttpResponse executeRequest(const HttpRequest&);

};