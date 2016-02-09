#include <string>
#include <unordered_map>

class HttpResponse
{
private:
    std::unorderd_map<std::string, std::string> _headers;
    std::string _uri;
    std::string _host;
    std::string _body;

public:
    std::unordered_map<std::string, std::string> getHeaders();
    std::string getHeader(std::string key);
    std::string getHost();
    std::string getUri();
    std::string getContent();
}
