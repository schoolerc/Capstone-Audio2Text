#include "TwitchStreamChunk.hpp"
#include "FFMPEG.hpp"
#include "Utilities.hpp"
#include <exception>
#include <stdexcept>

int internalAVFormatRead(void *ptr, uint8_t *buf, int buf_size) {
  TwitchStreamChunk *chunk = reinterpret_cast<TwitchStreamChunk *>(ptr);
  chunk->_rawVideoData->read((char *) buf, buf_size);
  int bytesRead = chunk->_rawVideoData->gcount();

  if (chunk->_rawVideoData->eof()) {
	return AVERROR_EOF;
  }

  if (chunk->_rawVideoData->bad()) {
	return -1;
  }

  return bytesRead;
}

int64_t internalAVFormatSeek(void *ptr, int64_t pos, int which) {
  TwitchStreamChunk *chunk = reinterpret_cast<TwitchStreamChunk *>(ptr);
  switch (which) {
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

void TwitchStreamChunk::download() {
  _HttpClientType::request request(getUri());
  auto response = getHttpClient().get(request);

  if (response.status() != 200) {
	throw std::runtime_error("Failed to download video file");
  }

  _rawVideoData = std::make_shared<std::stringstream>(response.body());

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
  _rawVideoData->read((char *) tmpBuffer, internalBufSize);
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

  if (avformat_open_input(&formatContext, this->_uri.c_str(), 0, 0) != 0) {
	throw std::runtime_error("Failed to open input stream for video data");
  }

  avformat_find_stream_info(formatContext, nullptr);
  //TODO: test if we actually have the video file open
  av_dump_format(formatContext, 0, getUri().c_str(), 0);

  int audioStream = -1;
  for (int i = 0; i < formatContext->nb_streams; i++) {
	if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audioStream < 0) {
	  audioStream = i;
	}
  }

  if (audioStream == -1) {
	throw std::runtime_error("Unable to get audio stream.");
  }

  AVCodecContext *audioCodecContextOrig;
  AVCodecContext *audioCodecContext;

  audioCodecContextOrig = formatContext->streams[audioStream]->codec;

  AVCodec *audioCodec = avcodec_find_decoder(audioCodecContextOrig->codec_id);
  if (!audioCodec) {
	throw std::runtime_error("Unsupported codec");
  }

  audioCodecContext = avcodec_alloc_context3(audioCodec);
  if (avcodec_copy_context(audioCodecContext, audioCodecContextOrig) != 0) {
	throw std::runtime_error("Unable to copy codec context");
  }

  avcodec_open2(audioCodecContext, audioCodec, nullptr);

  AVPacket packet;
  AVFrame frame;
  std::vector<AVFrame> audioFrames;
  size_t bufferSize = 0;
  while (av_read_frame(formatContext, &packet) >= 0) {
	if (packet.stream_index == audioStream) {
	  int got_frame = 0;
	  int len = avcodec_decode_audio4(audioCodecContext,
									  &frame,
									  &got_frame,
									  &packet);
	  if(len < 0)
	  {
		continue;
	  }
	  if(got_frame) {
		audioFrames.push_back(frame);
		bufferSize += av_samples_get_buffer_size(NULL,
												 audioCodecContext->channels,
												 frame.nb_samples,
												 audioCodecContext->sample_fmt,
												 1);
	  }

	}
	else {
	  av_free_packet(&packet);
	}
  }


  avformat_close_input(&formatContext);
  av_free(IOContext);
  delete[] internalBuffer;
}
