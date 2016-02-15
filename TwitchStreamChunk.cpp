#include "TwitchStreamChunk.hpp"
#include "FFMPEG.hpp"
#include "Utilities.hpp"
#include <exception>
#include <stdexcept>

int internalAVFormatRead(void* ptr, uint8_t* buf, int buf_size)
{
    TwitchStreamChunk* chunk = reinterpret_cast<TwitchStreamChunk*>(ptr);
    chunk->_rawVideoData->read((char*)buf, buf_size);
    int bytesRead = chunk->_rawVideoData->gcount();

    if(chunk->_rawVideoData->eof())
    {
        return AVERROR_EOF;
    }

    if(chunk->_rawVideoData->bad())
    {
        return -1;
    }

    return bytesRead;
}

int64_t internalAVFormatSeek(void* ptr, int64_t pos, int which)
{
    TwitchStreamChunk* chunk = reinterpret_cast<TwitchStreamChunk*>(ptr);
    switch(which)
    {
        case AVSEEK_SIZE:
            return chunk->_rawVideoData->str().size();

        case SEEK_SET:
            chunk->_rawVideoData->seekg(pos, std::ios_base::beg);
            return chunk->_rawVideoData->tellg();

        case SEEK_CUR:
            chunk->_rawVideoData->seekg(pos, std::ios_base::cur);
            return chunk->_rawVideoData->tellg();

        case SEEK_END:
            chunk->_rawVideoData->seekg(pos, std::ios_base::end);
            return chunk->_rawVideoData->tellg();
    }
}

void TwitchStreamChunk::download()
{
    _HttpClientType::request request(getUri());
    auto response = getHttpClient().get(request);

    if(response.status() != 200)
    {
        throw std::runtime_error("Failed to download video file");
    }

    _rawVideoData = std::make_shared<std::stringstream>(response.body());

    const int internalBufSize = 1024*1024*8;
    unsigned char* internalBuffer = new unsigned char[internalBufSize];
    AVIOContext* IOContext = avio_alloc_context(internalBuffer, internalBufSize, 0, this, internalAVFormatRead, 0, internalAVFormatSeek);
    AVFormatContext* formatContext = avformat_alloc_context();

    formatContext->pb = IOContext;
    formatContext->flags = AVFMT_FLAG_CUSTOM_IO;

    if(avformat_open_input(&formatContext, this->_uri.c_str(), 0, 0) != 0)
    {
        throw std::runtime_error("Failed to open input stream for video data");
    }

    //TODO: test if we actually have the video file open
}
