#include "AudioChunkFactory.h"
#include "Utilities.hpp"
#include <fstream>
#include <thread>
#include <boost/filesystem.hpp>
#include "FFMPEG.hpp"


int internalAVFormatRead(void *ptr, uint8_t *buf, int buf_size)
{
  AudioChunkFactory *chunk = reinterpret_cast<AudioChunkFactory *>(ptr);
  chunk->_rawVideoData.read((char *) buf, buf_size);
  int bytesRead = chunk->_rawVideoData.gcount();

  if (chunk->_rawVideoData.eof())
  {
    return AVERROR_EOF;
  }

  if (chunk->_rawVideoData.bad())
  {
    return -1;
  }

  return bytesRead;
}

int64_t internalAVFormatSeek(void *ptr, int64_t pos, int which)
{
  AudioChunkFactory *chunk = reinterpret_cast<AudioChunkFactory *>(ptr);
  switch (which)
  {
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
    }
  }

  if (audioStream == -1)
  {
    throw std::runtime_error("Unable to get audio stream.");
  }

  AVCodecContext *audioCodecContext;

  audioCodecContext = formatContext->streams[audioStream]->codec;

  AVCodec *audioCodec = avcodec_find_decoder(audioCodecContext->codec_id);
  if (!audioCodec)
  {
    throw std::runtime_error("Unsupported codec");
  }


  avcodec_open2(audioCodecContext, audioCodec, nullptr);


  AVPacket packet;
  AVFrame *frame = av_frame_alloc();

  SwrContext *resampleContext = swr_alloc();

  av_opt_set_int(resampleContext, "in_channel_layout", audioCodecContext->channel_layout, 0);
  av_opt_set_int(resampleContext, "in_sample_rate", audioCodecContext->sample_rate, 0);
  av_opt_set_sample_fmt(resampleContext, "in_sample_fmt", audioCodecContext->sample_fmt, 0);

  av_opt_set_int(resampleContext, "out_channel_layout", AV_CH_LAYOUT_MONO, 0);
  av_opt_set_int(resampleContext, "out_sample_rate", 16000, 0);
  av_opt_set_sample_fmt(resampleContext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

  int result = swr_init(resampleContext);

  /*int64_t dist_nb_samples = av_rescale_rnd(swr_get_delay(resampleContext, 48000) + ,
                                           16000,
                                           audioCodecContext->sample_rate,
                                           AV_ROUND_UP);*/

  /*unsigned char **dst_data = nullptr;
  int dst_linesize = 0;
  int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_MONO);
  /*int ret = av_samples_alloc_array_and_samples(&dst_data,
                                               &dst_linesize,
                                               av_get_channel_layout_nb_channels(AV_CH_LAYOUT_MONO),
                                               dist_nb_samples,
                                               AV_SAMPLE_FMT_S16,
                                               0);*/

  /*while (av_read_frame(formatContext, &packet) >= 0)
  {
    if (packet.stream_index == audioStream)
    {
      AVPacket decodingPacket = packet;
      while (decodingPacket.size > 0)
      {
        int gotFrame = 0;
        int result = avcodec_decode_audio4(audioCodecContext, frame, &gotFrame, &decodingPacket);
        if (result >= 0 && gotFrame)
        {
          decodingPacket.size -= result;
          decodingPacket.data += result;
          int64_t dist_nb_samples = av_rescale_rnd(swr_get_delay(resampleContext, 48000) + frame->nb_samples,
                                                   16000,
                                                   audioCodecContext->sample_rate,
                                                   AV_ROUND_UP);
          unsigned char **dst_data = nullptr;
          int dst_linesize = 0;
          int ret = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize,
                                                       av_get_channel_layout_nb_channels(AV_CH_LAYOUT_MONO),
                                                       dist_nb_samples,
                                                       AV_SAMPLE_FMT_S16,
                                                       0);
          int result = swr_convert(resampleContext,
                      dst_data,
                      dist_nb_samples,
                      (const unsigned char **) frame->data,
                      frame->nb_samples);
          int i = ps_process_raw(getDecoder(), (const int16 *) dst_data[0], result, 0, 1);
          av_freep(&dst_data);
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

  swr_free(&resampleContext);
  avformat_close_input(&formatContext);*/
  return AudioChunk();
}