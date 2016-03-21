#pragma once
#include <vector>

class AudioChunk {
private:
    std::vector<unsigned char> _buffer;
    int _duration;

    friend class AudioChunkFactory;

public:
    AudioChunk();

    ~AudioChunk();

    const unsigned char* buffer() const {return _buffer.data();}
    unsigned char* buffer() {return _buffer.data();}
    unsigned int size() const {return _buffer.size();}
    int duration() const { return _duration;}
};
