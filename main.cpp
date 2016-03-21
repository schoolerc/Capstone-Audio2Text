#include "prefix.hpp"
#include "Exception.hpp"
#include "Twitch.hpp"
#include "Utilities.hpp"

namespace po = boost::program_options;

po::variables_map parse_cmd_args(int argc, const char *argv[])
{
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("channel", po::value<std::string>(), "Name of the channel to stream")
      ("database_host", po::value<std::string>(), "Host of the database to send data to")
      ("database_port", po::value<std::string>(), "Port number to connect to the database on")
      ("db_username", po::value<std::string>(), "Username to access the database with")
      ("db_password", po::value<std::string>(), "Password to access the database with");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  return vm;
}

int main(int argc, const char *argv[])
{
  auto cmd_ln = parse_cmd_args(argc, argv);
  av_register_all();

  try
  {
    sql::Driver *driver = get_driver_instance();
    std::unique_ptr<sql::Connection> connection
        (driver->connect(cmd_ln["database_host"].as<std::string>() + ":" + cmd_ln["database_port"].as<std::string>(),
                         cmd_ln["db_username"].as<std::string>(),
                         cmd_ln["db_password"].as<std::string>()));

    Twitch tv;
    TwitchStream stream = tv.openStream(cmd_ln["channel"].as<std::string>());
    auto playlist = stream.getPlaylist(TwitchStreamPlaylist::Quality::Source);
    playlist.stream(connection);
  }
  catch (boost::exception &ex)
  {
    boost::diagnostic_information(ex);
  }
  catch (std::exception &ex)
  {
    std::cout << ex.what() << std::endl;
  }
  return 0;
}