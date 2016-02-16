#include "TwitchStreamChunk.hpp"
#include "FFMPEG.hpp"
#include "Utilities.hpp"
#include "AudioChunkFactory.h"
#include <exception>
#include <stdexcept>


void TwitchStreamChunk::download() {
  _HttpClientType::request request(getUri());
  auto response = getHttpClient().get(request);

  if (response.status() != 200) {
	throw std::runtime_error("Failed to download video file");
  }

  AudioChunkFactory factory;
  factory.setRawData(response.body());
  factory.setUri(getUri());

  AudioChunk c = factory.build();

}
