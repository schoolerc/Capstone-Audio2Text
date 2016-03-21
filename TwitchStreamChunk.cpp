#include "prefix.hpp"
#include "TwitchStreamChunk.hpp"
#include "Utilities.hpp"
#include "AudioChunkFactory.h"



void TwitchStreamChunk::download() {
  _HttpClientType::request request(getUri());
  auto response = getHttpClient().get(request);

  if (response.status() != 200) {
	throw std::runtime_error("Failed to download video file");
  }

  AudioChunkFactory factory;
  factory.setRawData(response.body());
  factory.setUri(getUri());

  std::tie(_hypothesis, _confidence) = factory.build();

  //std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("INSERT INTO ChannelTranscription.Test (test_hyp, test_confidence) VALUES (?, ?)"));

  //statement->setString(1, hyp);
  //statement->setDouble(2, confidence);

  //auto result = statement->execute();

  int i = 0;

}
