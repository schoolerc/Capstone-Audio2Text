#include "prefix.hpp"
#include "TwitchStream.hpp"
#include "TwitchStreamPlaylist.hpp"
#include "TwitchStreamChunkFactory.hpp"
#include "Utilities.hpp"
#include <cppconn/exception.h>

namespace network = boost::network;
namespace http = boost::network::http;

TwitchStreamPlaylist::TwitchStreamPlaylist(TwitchStream &stream) : _stream(stream)
{ }

TwitchStreamPlaylist::~TwitchStreamPlaylist()
{ }

std::vector<TwitchStreamChunk> TwitchStreamPlaylist::getCurrentChunks()
{
  TwitchStreamChunkFactory chunkFactory;

  _HttpClientType::request request(_uri);
  auto response = getHttpClient().get(request);

  if (response.status() != 200)
  {
    //TODO: Throw exception
  }

  chunkFactory.setSource(this);
  chunkFactory.setM3U8(response.body());

  auto chunks = chunkFactory.build();
  return chunks;
}

void TwitchStreamPlaylist::stream(std::unique_ptr<sql::Connection> &connection)
{
  ps_start_stream(getDecoder());
  //std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement("INSERT INTO ChannelTranscription.Test (test_hyp, test_confidence) VALUES (?, ?)"));
  std::string raw_stmt = "CREATE TABLE IF NOT EXISTS `ChannelTranscription`.`" + _stream.streamName() + "` (\n" +
      "`id` INT NOT NULL AUTO_INCREMENT,\n" +
      "`hyp` VARCHAR(256) NULL,\n" +
      "`confidence` DOUBLE NULL,\n" +
      "`_time` DATETIME(0) NULL,\n" +
      "PRIMARY KEY(`id`));";
  std::unique_ptr<sql::PreparedStatement> statement(connection->prepareStatement(raw_stmt));

  statement->execute();
  while (true)
  {
    auto currentChunks = getCurrentChunks();
    std::vector<TwitchStreamChunk> chunksToDownload(currentChunks.begin(), currentChunks.end());

    std::remove_if(chunksToDownload.begin(), chunksToDownload.end(), [this](TwitchStreamChunk &chunk)
    {
      auto itr = std::find_if(_prevChunks.begin(), _prevChunks.end(), [&chunk](TwitchStreamChunk &c)
      {
        return chunk.getUri().compare(c.getUri()) == 0;
      });

      return itr != _prevChunks.end();
    });

    downloadChunks(chunksToDownload, connection);

    storeChunks(chunksToDownload, connection);


    _prevChunks = currentChunks;
  }
}

void TwitchStreamPlaylist::downloadChunks(std::vector<TwitchStreamChunk> &chunks,
                                          std::unique_ptr<sql::Connection> &connection)
{
  std::string raw_stmt = "INSERT INTO `ChannelTranscription`.`" + _stream.streamName()
      + "` (`hyp`, `confidence`, `_time`) VALUES (?, ?, CURRENT_TIMESTAMP)";
  std::unique_ptr<sql::PreparedStatement> insert_stmt(connection->prepareStatement(raw_stmt));
  for (auto c : chunks)
  {
    c.download();

    insert_stmt->clearParameters();
    insert_stmt->setString(1, c.getHyp());
    insert_stmt->setDouble(2, c.getConfidence());

    int result = insert_stmt->executeUpdate();
  }

  //remove old data to avoid overflowing the database
  raw_stmt =
      "DELETE FROM `ChannelTranscription`.`" + _stream.streamName() + "` WHERE `_time` < NOW()-INTERVAL 20 SECOND";

  std::unique_ptr<sql::PreparedStatement> delete_stmt(connection->prepareStatement(raw_stmt));

  delete_stmt->executeUpdate();

}

void TwitchStreamPlaylist::storeChunks(std::vector<TwitchStreamChunk> &chunks,
                                       std::unique_ptr<sql::Connection> &connection)
{

  for (auto c : chunks)
  {

  }
}