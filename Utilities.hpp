#pragma once
#include <istream>
#include <string>
#include <sstream>
#include <iomanip>
#include <boost/network/protocol.hpp>
#include <pocketsphinx.h>

typedef boost::network::http::basic_client<boost::network::http::tags::http_async_8bit_tcp_resolve, 1, 1>
    _HttpClientType;


_HttpClientType &getHttpClient();

ps_decoder_t *getDecoder();

std::string url_encode(const std::string &value);

std::string escape_json(const std::string &s);