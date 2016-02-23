#pragma once
#include <string>
#include <sstream>
#include <memory>
#include "AudioChunk.h"

class TwitchStreamChunk
{
    friend int internalAVFormatRead(void* ptr, uint8_t* buf, int buf_size);
    friend int64_t internalAVFormatSeek(void* ptr, int64_t pos, int which);
    std::string _uri;
    double _duration;
    std::shared_ptr<AudioChunk> _chunk;
    friend class TwitchStreamChunkFactory;

public:
    TwitchStreamChunk(){}
    ~TwitchStreamChunk(){}

    std::string getUri(){return _uri;}
    double getDuration(){return _duration;}
    std::shared_ptr<AudioChunk> getAudioChunk() {return _chunk;}
    void download();
};