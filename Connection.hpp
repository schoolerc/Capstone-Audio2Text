#include <boost/asio.hpp>
#include "HttpClient.hpp"
class Connection
{
private:
    boost::asio::ip::tcp::socket _socket;
    HttpClient& _client
public:
    Connection(boost::asio::io_service&, HttpClient&);
    ~Connection()

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    bool open(std::string host, std::string service);
    
    bool AsyncRead(boost::asio::mutable_buffer& buffer);
    bool AsyncWrite(boost::asio::const_buffer& buffer);
}
