#include "prefix.hpp"
#include "TwitchStreamPlaylistFactory.hpp"

void TwitchStreamPlaylistFactory::clear()
{
    _media_info.clear();
    _stream_info.clear();
    _uri.clear();
}

TwitchStreamPlaylist::Quality TwitchStreamPlaylistFactory::quality(std::string q)
{
    if (q.compare("Mobile") == 0)
    {
        return TwitchStreamPlaylist::Quality::Mobile;
    }
    else if (q.compare("Low") == 0)
    {
        return TwitchStreamPlaylist::Quality::Low;
    }
    else if (q.compare("Medium") == 0)
    {
        return TwitchStreamPlaylist::Quality::Medium;
    }
    else if (q.compare("High") == 0)
    {
        return TwitchStreamPlaylist::Quality::High;
    }
    else if (q.compare("Source") == 0)
    {
        return TwitchStreamPlaylist::Quality::Source;
    }
    else
    {
        return TwitchStreamPlaylist::Quality::Mobile;
    }
}

TwitchStreamPlaylist TwitchStreamPlaylistFactory::build()
{
    //TODO: Create exceptions and throw them here.
    if(_media_info.substr(0, _media_info.find(':')).compare("#EXT-X-MEDIA") != 0)
    {
        //throw exception; string doesn't start the way we expect it to.
    }

    if(_stream_info.substr(0, _stream_info.find(':')).compare("#EXT-X-STREAM-INF") != 0)
    {
        //throw exception; string doesn't start the way we expect it to.
    }

    if(_uri.substr(0, 4).compare("http") != 0)
    {
        //throw exception; string is not an http uri
    }

    TwitchStreamPlaylist playlist(_stream);

    std::string media_values = _media_info.substr(_media_info.find(':')+1);
    std::stringstream media_values_stream(media_values);
    std::string media_value;
    //loop through comma delimited key/value pairs
    while(std::getline(media_values_stream, media_value, ','))
    {
        std::string key = media_value.substr(0, media_value.find('='));
        //this is actually the only value we care about
        if(key.compare("NAME") == 0)
        {
            //go 1 past for '=' and 1 past for first quotation mark
            std::string value = media_value.substr(media_value.find('=')+2);
            //pop off last quotation mark
            value.pop_back();

            playlist._quality = quality(value);
        }
    }

    std::string stream_values = _stream_info.substr(_stream_info.find(':')+1);
    std::stringstream stream_values_stream(stream_values);
    std::string stream_value;
    //loop through comma delimited key/value pairs
    while(std::getline(stream_values_stream, stream_value, ','))
    {
        auto pos = stream_value.find('=');
        std::string key = stream_value.substr(0, pos);
        int i = 0;
        if(key.compare("BANDWIDTH") == 0)
        {
            //convert string value to integer
            playlist._bandwidth = std::atoi(stream_value.substr(pos + 1).c_str());
        }
        else if(key.compare("RESOLUTION") == 0)
        {
            std::string value = stream_value.substr(pos+1);
            //value is {width}x{height}
            int x_pos = value.find('x');
            auto _sWidth = value.substr(0, x_pos);
            auto _hWidth = value.substr(x_pos+1);
            int width = std::atoi(value.substr(0, x_pos).c_str());
            int height = std::atoi(value.substr(x_pos+1).c_str());

            playlist._resolution._width = width;
            playlist._resolution._height = height;
        }
    }

    playlist._uri = _uri;

    return playlist;
}