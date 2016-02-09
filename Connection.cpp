#include "Connection.hpp"

Connection::Connection(boost::asio::io_service& service, HttpClient& client) : _socket(service), _client(client)
{

}

Connection::~Connection()
{
    _socket.shutdown(boost::asio::ip::tcp::shutdown_type::shutdown_both);
    _socket.close();
}

bool Connection::open(std::string host, std::string service)
{

}
