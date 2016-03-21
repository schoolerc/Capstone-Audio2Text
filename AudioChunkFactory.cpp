#include "prefix.hpp"
#include "AudioChunkFactory.h"
#include "Utilities.hpp"
#include "AudioChunk.h"
#include <tuple>

AudioChunkFactory::~AudioChunkFactory()
{
  if (_formatContext != nullptr)
  {
    avformat_close_input(&_formatContext);
    _formatContext = nullptr;
  }

  if (_ioContext != nullptr)
  {
    avio_close(_ioContext);
    _ioContext = nullptr;
  }

  if (_internalIOBuffer != nullptr)
  {
    av_free(_internalIOBuffer);
    _internalIOBuffer = nullptr;
  }
}

std::tuple<std::string, double> AudioChunkFactory::build()
{
  //loadMediaFile();
  //openAudioStream();
  //processAudioData();
  int pos = _uri.find_last_of('/');
  std::string fileName = _uri.substr(pos + 1);
  std::ofstream file(fileName);
  file << _rawVideoData.rdbuf();
  file.flush();
  file.close();

  std::stringstream cmd;
  std::string audioFile = boost::filesystem::unique_path().native();
  audioFile.append(".wav");
  cmd << "ffmpeg -i " << fileName << " -vn -ac 1 " << audioFile << " > /dev/null 2>&1";

  system(cmd.str().c_str());

  FILE *aFile = fopen(audioFile.c_str(), "r");
  ps_decode_raw(getDecoder(), aFile, -1);
  fclose(aFile);

  auto logarithm = ps_get_logmath(getDecoder());

  int confidence = 1;
  const char * result = ps_get_hyp(getDecoder(), &confidence);
  double tmp = logmath_exp(logarithm, confidence);

  std::remove(fileName.c_str());
  std::remove(audioFile.c_str());
  return std::make_tuple(result == nullptr ? "" : std::string(result), tmp);
}
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
*/

int64_t internalAVFormatSeek(void *ptr, int64_t pos, int which)
{
  AudioChunkFactory *chunk = reinterpret_cast<AudioChunkFactory *>(ptr);
  switch (which)
  {
    default:
      return 0;
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

int internalAVFormatRead(void *ptr, uint8_t *buf, int buf_size)
{
  AudioChunkFactory *chunk = reinterpret_cast<AudioChunkFactory *>(ptr);

  chunk->_rawVideoData.read((char *) buf, buf_size);
  auto bytesRead = chunk->_rawVideoData.gcount();

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

void AudioChunkFactory::loadMediaFile()
{
  const int internalBufSize = 32 * 1024;
  _internalIOBuffer = av_malloc(internalBufSize);

  _ioContext = avio_alloc_context((unsigned char *) _internalIOBuffer, internalBufSize,
                                  0,
                                  this,
                                  internalAVFormatRead,
                                  0,
                                  internalAVFormatSeek);

  _formatContext = avformat_alloc_context();

  _formatContext->pb = _ioContext;

  unsigned char *probeBuffer = (unsigned char *) av_malloc(internalBufSize * 10);
  _rawVideoData.read((char *) probeBuffer, internalBufSize);

  _rawVideoData.seekg(0);

  AVProbeData probeData;
  probeData.buf = probeBuffer;
  probeData.buf_size = _rawVideoData.gcount();
  probeData.filename = _uri.c_str();

  _formatContext->iformat = av_probe_input_format(&probeData, 1);
  _formatContext->flags = AVFMT_FLAG_CUSTOM_IO;

  av_free(probeBuffer);

  int result = avformat_open_input(&_formatContext, _uri.c_str(), 0, 0);
  if (result != 0)
  {
    throw std::runtime_error("F");
  }

  avformat_find_stream_info(_formatContext, nullptr);
}

void AudioChunkFactory::openAudioStream()
{
  for (int stream = 0; stream < _formatContext->nb_streams; stream++)
  {
    if (_formatContext->streams[stream]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
    {
      _audioStream = stream;
      break;
    }
  }

  AVCodecContext *tmp = _formatContext->streams[_audioStream]->codec;

  _audioCodec = avcodec_find_decoder(tmp->codec_id);

  _audioCodecContext = avcodec_alloc_context3(_audioCodec);

  if (avcodec_copy_context(_audioCodecContext, tmp) != 0)
  {
    //throw something
    int i = 0;
  }

  int i = avcodec_open2(_audioCodecContext, _audioCodec, nullptr);
  int j = 0;
}

void AudioChunkFactory::processAudioData()
{
  AVAudioResampleContext *avr = avresample_alloc_context();
  av_opt_set_int(avr, "in_channel_layout", _audioCodecContext->channel_layout, 0);
  av_opt_set_int(avr, "out_channel_layout", AV_CH_LAYOUT_MONO, 0);
  av_opt_set_int(avr, "in_sample_rate", _audioCodecContext->sample_rate, 0);
  av_opt_set_int(avr, "out_sample_rate", _audioCodecContext->sample_rate, 0);
  av_opt_set_int(avr, "in_sample_fmt", _audioCodecContext->sample_fmt, 0);
  av_opt_set_int(avr, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

  avresample_open(avr);

  AVPacket packet;
  AVFrame *originalFrame = av_frame_alloc();
  AVFrame *convertedFrame = av_frame_alloc();
  int i = av_read_frame(_formatContext, &packet);
  while (av_read_frame(_formatContext, &packet) >= 0)
  {
    if (packet.stream_index == _audioStream)
    {
      int got_frame = 0;
      int num_bytes = avcodec_decode_audio4(_audioCodecContext, originalFrame, &got_frame, &packet);

      if (num_bytes < 0)
      {
        //We error'd, try again on the next frame
        continue;
      }

      if (got_frame)
      {
        avresample_convert_frame(avr, convertedFrame, originalFrame);


        int i = 0;
      }
    }
    av_packet_unref(&packet);
  }

  av_frame_free(&originalFrame);
  av_frame_free(&convertedFrame);
}