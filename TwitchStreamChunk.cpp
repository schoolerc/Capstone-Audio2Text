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

    //Create ffmpeg structures
    const int internalBufSize = 1024*32;
    unsigned char* internalBuffer = new unsigned char[internalBufSize];
    AVIOContext* IOContext = avio_alloc_context(internalBuffer, internalBufSize, 0, this, internalAVFormatRead, 0, internalAVFormatSeek);
    AVFormatContext* formatContext = avformat_alloc_context();

    //Tell ffmpeg to use our IO stuffs
    formatContext->pb = IOContext;
    formatContext->flags = AVFMT_FLAG_CUSTOM_IO;

    //Find out what input format to use.
    unsigned char* tmpBuffer = new unsigned char[internalBufSize];
    _rawVideoData->read((char*)tmpBuffer, internalBufSize);
    int bytesRead = _rawVideoData->gcount();

    //reset the stream for ffmpeg to use
    _rawVideoData->seekg(0, std::ios_base::beg);

    //Create the probeData structure for av_prove_input_format
    AVProbeData probeData;
    probeData.buf = tmpBuffer;
    probeData.buf_size = bytesRead;
    probeData.filename = getUri().c_str();

    formatContext->iformat = av_probe_input_format(&probeData, 1);

    //cleanup probe
    delete[] tmpBuffer;

    if(avformat_open_input(&formatContext, this->_uri.c_str(), 0, 0) != 0)
    {
        throw std::runtime_error("Failed to open input stream for video data");
    }

    //TODO: test if we actually have the video file open
    av_dump_format(formatContext, 0, getUri().c_str(), 0);



    avformat_close_input(&formatContext);
    av_free(IOContext);
    delete[] internalBuffer;
}
