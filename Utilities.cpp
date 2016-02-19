#include "Utilities.hpp"
#ifndef SPHINX_MODELDIR
#define SPHINX_MODELDIR "/usr/local/share/pocketsphinx/model"
#endif

_HttpClientType &getHttpClient()
{
  static _HttpClientType client;
  return client;
}

ps_decoder_t *getDecoder()
{
  static ps_decoder_t *ps = nullptr;
  static cmd_ln_t *config = nullptr;

  if (ps == nullptr || config == nullptr)
  {
    config = cmd_ln_init(nullptr, ps_args(), true,
                         "-hmm", SPHINX_MODELDIR "/en-us/en-us",
                         "-lm", SPHINX_MODELDIR "/en-us/en-us.lm.bin",
                         "-dict", SPHINX_MODELDIR "/en-us/cmudict-en-us.dict",
                         nullptr);
    ps = ps_init(config);
  }

  return ps;
}
//*/
std::string escape_json(const std::string &s)
{
  std::ostringstream o;
  for (auto c = s.cbegin(); c != s.cend(); c++)
  {
    switch (*c)
    {
      case '"':
        o << "\\\"";
        break;
      case '\\':
        o << "\\\\";
        break;
      case '\b':
        o << "\\b";
        break;
      case '\f':
        o << "\\f";
        break;
      case '\n':
        o << "\\n";
        break;
      case '\r':
        o << "\\r";
        break;
      case '\t':
        o << "\\t";
        break;
      default:
        if ('\x00' <= *c && *c <= '\x1f')
        {
          o << "\\u"
              << std::hex << std::setw(4) << std::setfill('0') << (int) *c;
        }
        else
        {
          o << *c;
        }
    }
  }
  return o.str();
}


std::string url_encode(const std::string &value)
{
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;

  for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
  {
    std::string::value_type c = (*i);

    // Keep alphanumeric and other accepted characters intact
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
    {
      escaped << c;
      continue;
    }

    // Any other characters are percent-encoded
    escaped << std::uppercase;
    escaped << '%' << std::setw(2) << int((unsigned char) c);
    escaped << std::nouppercase;
  }

  return escaped.str();
}