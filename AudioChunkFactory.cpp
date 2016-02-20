#include "AudioChunkFactory.h"
<<<<<<< HEAD
#include <sstream>
=======
#include "Utilities.hpp"
#include <fstream>
#include <thread>
#include <boost/filesystem.hpp>
>>>>>>> alpha
#include "FFMPEG.hpp"
#include <iostream>
#include <exception>
#include <stdexcept>

void printAudioFrameInfo(const AVCodecContext* codecContext, const AVFrame* frame)
{
    // See the following to know what data type (unsigned char, short, float, etc) to use to access the audio data:
    // http://ffmpeg.org/doxygen/trunk/samplefmt_8h.html#af9a51ca15301871723577c730b5865c5
    std::cout << "Audio frame info:\n"
        << "  Sample count: " << frame->nb_samples << '\n'
        << "  Channel count: " << codecContext->channels << '\n'
        << "  Format: " << av_get_sample_fmt_name(codecContext->sample_fmt) << '\n'
        << "  Bytes per sample: " << av_get_bytes_per_sample(codecContext->sample_fmt) << '\n'
        << "  Is planar? " << av_sample_fmt_is_planar(codecContext->sample_fmt) << '\n';


    std::cout << "frame->linesize[0] tells you the size (in bytes) of each plane\n";

    if (codecContext->channels > AV_NUM_DATA_POINTERS && av_sample_fmt_is_planar(codecContext->sample_fmt))
    {
        std::cout << "The audio stream (and its frames) have too many channels to fit in\n"
            << "frame->data. Therefore, to access the audio data, you need to use\n"
            << "frame->extended_data to access the audio data. It's planar, so\n"
            << "each channel is in a different element. That is:\n"
            << "  frame->extended_data[0] has the data for channel 1\n"
            << "  frame->extended_data[1] has the data for channel 2\n"
            << "  etc.\n";
    }
    else
    {
        std::cout << "Either the audio data is not planar, or there is enough room in\n"
            << "frame->data to store all the channels, so you can either use\n"
            << "frame->data or frame->extended_data to access the audio data (they\n"
            << "should just point to the same data).\n";
    }

    std::cout << "If the frame is planar, each channel is in a different element.\n"
        << "That is:\n"
        << "  frame->data[0]/frame->extended_data[0] has the data for channel 1\n"
        << "  frame->data[1]/frame->extended_data[1] has the data for channel 2\n"
        << "  etc.\n";

    std::cout << "If the frame is packed (not planar), then all the data is in\n"
        << "frame->data[0]/frame->extended_data[0] (kind of like how some\n"
        << "image formats have RGB pixels packed together, rather than storing\n"
        << " the red, green, and blue channels separately in different arrays.\n";
}

<<<<<<< HEAD
int internalAVFormatRead(void *ptr, uint8_t *buf, int buf_size) {
    AudioChunkFactory *chunk = reinterpret_cast<AudioChunkFactory *>(ptr);
    chunk->_rawVideoData.read((char *) buf, buf_size);
    int bytesRead = chunk->_rawVideoData.gcount();

    if (chunk->_rawVideoData.eof()) {
        return AVERROR_EOF;
    }

    if (chunk->_rawVideoData.bad()) {
        return -1;
=======
AudioChunk AudioChunkFactory::build()
{
  int pos = _uri.find_last_of('/');
  std::string fileName = _uri.substr(pos + 1);
  std::ofstream file(fileName);
  file << _rawVideoData.rdbuf();
  file.flush();
  file.close();

  std::stringstream cmd;
  std::string audioFile = boost::filesystem::unique_path().native();
  audioFile.append(".wav");
  cmd << "ffmpeg -i " << fileName << " -vn -ar 16000 -ac 1 " << audioFile << " > /dev/null 2>&1";

  system(cmd.str().c_str());

  FILE* f = fopen(audioFile.c_str(), "rb");
  ps_decode_raw(getDecoder(), f, -1);
  int confidence;
  std::cout << ps_get_hyp(getDecoder(), &confidence) << std::endl;
  fclose(f);


  std::remove(fileName.c_str());
  std::remove(audioFile.c_str());
  /*
  AVFormatContext *formatContext = avformat_alloc_context();

  if (avformat_open_input(&formatContext, fileName.c_str(), 0, 0) != 0)
  {
    throw std::runtime_error("Failed to open input stream for video data");
  }

  avformat_find_stream_info(formatContext, nullptr);
  //TODO: test if we actually have the video file open

  int audioStream = -1;
  for (int i = 0; i < formatContext->nb_streams; i++)
  {
    if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audioStream < 0)
    {
      audioStream = i;
>>>>>>> alpha
    }

    return bytesRead;
}

int64_t internalAVFormatSeek(void *ptr, int64_t pos, int which) {
    AudioChunkFactory *chunk = reinterpret_cast<AudioChunkFactory *>(ptr);
    switch (which) {
        case AVSEEK_SIZE:
            return chunk->_rawVideoData.str().size();

        case SEEK_SET:
            chunk->_rawVideoData.seekg(pos, std::ios_base::beg);
            return chunk->_rawVideoData.tellg();

        case SEEK_CUR:
            chunk->_rawVideoData.seekg(pos, std::ios_base::cur);
            return chunk->_rawVideoData.tellg();

        case SEEK_END:
            chunk->_rawVideoData.seekg(pos, std::ios_base::end);
            return chunk->_rawVideoData.tellg();
    }
}

void AudioChunkFactory::clear()
{
    _rawVideoData.clear();
    _uri.clear();
}

AudioChunk AudioChunkFactory::build()
{
//Create ffmpeg structures
    const int internalBufSize = 1024 * 32;
    unsigned char *internalBuffer = new unsigned char[internalBufSize];
    AVIOContext *IOContext =
            avio_alloc_context(internalBuffer, internalBufSize, 0, this, internalAVFormatRead, 0, internalAVFormatSeek);
    AVFormatContext *formatContext = avformat_alloc_context();

    //Tell ffmpeg to use our IO stuffs
    formatContext->pb = IOContext;
    formatContext->flags = AVFMT_FLAG_CUSTOM_IO;

    //Find out what input format to use.
    unsigned char *tmpBuffer = new unsigned char[internalBufSize];
    _rawVideoData.read((char *) tmpBuffer, internalBufSize);
    int bytesRead = _rawVideoData.gcount();

    //reset the stream for ffmpeg to use
    _rawVideoData.seekg(0, std::ios_base::beg);

    //Create the probeData structure for av_prove_input_format
    AVProbeData probeData;
    probeData.buf = tmpBuffer;
    probeData.buf_size = bytesRead;
    probeData.filename = _uri.c_str();

    formatContext->iformat = av_probe_input_format(&probeData, 1);

    //cleanup probe
    delete[] tmpBuffer;

    if (avformat_open_input(&formatContext, _uri.c_str(), 0, 0) != 0) {
        throw std::runtime_error("Failed to open input stream for video data");
    }

    avformat_find_stream_info(formatContext, nullptr);
    //TODO: test if we actually have the video file open
    av_dump_format(formatContext, 0, _uri.c_str(), 0);

    int audioStream = -1;
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audioStream < 0) {
            audioStream = i;
        }
    }

    if (audioStream == -1) {
        throw std::runtime_error("Unable to get audio stream.");
    }

    AVCodecContext *audioCodecContext;

    audioCodecContext = formatContext->streams[audioStream]->codec;

    AVCodec *audioCodec = avcodec_find_decoder(audioCodecContext->codec_id);
    if (!audioCodec) {
        throw std::runtime_error("Unsupported codec");
    }

    avcodec_open2(audioCodecContext, audioCodec, nullptr);

    AVPacket packet;
    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    AVFrame *frame = av_frame_alloc();
    std::vector<AVFrame> audioFrames;
    size_t bufferSize = 0;
    while (av_read_frame(formatContext, &packet) >= 0) {
        if(packet.stream_index == audioStream)
        {
            AVPacket decodingPacket = packet;
            while(decodingPacket.size > 0)
            {
                int gotFrame = 0;
                int result = avcodec_decode_audio4(audioCodecContext, frame, &gotFrame, &decodingPacket);

                if(result >= 0 && gotFrame)
                {
                    decodingPacket.size -= result;
                    decodingPacket.data += result;

                    printAudioFrameInfo(audioCodecContext, frame);
                }
                else
                {
                    decodingPacket.size = 0;
                    decodingPacket.data = nullptr;
                }
            }
        }
        av_packet_unref(&packet);
    }


    avformat_close_input(&formatContext);
    av_free(IOContext);
    delete[] internalBuffer;
    return AudioChunk();
}