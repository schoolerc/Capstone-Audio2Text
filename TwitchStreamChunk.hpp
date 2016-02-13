#pragma once
#include <string>

class TwitchStreamChunk
{
    std::string _uri;
    double _duration;

    friend class TwitchStreamChunkFactory;

public:
    TwitchStreamChunk(){}
    ~TwitchStreamChunk(){}

    std::string getUri(){return _uri;}
    double getDuration(){return _duration;}

};