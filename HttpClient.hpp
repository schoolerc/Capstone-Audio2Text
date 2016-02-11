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
	void parseHeaders(HttpResponse&, std::istream&);


	void sendRequest(const HttpRequest&);
	HttpResponse receiveResponse();
public:
    HttpClient();
    ~HttpClient();
    void connect(std::string host);
    void disconnect();
    bool isConnected();
	HttpResponse executeRequest(const HttpRequest&);

};