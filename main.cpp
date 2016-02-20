#include <iostream>
#include "Exception.hpp"
#include "Twitch.hpp"
#include "Utilities.hpp"
#include <thread>
#include "FFMPEG.hpp"
#include <pocketsphinx.h>

int main(int argc, const char *argv[]) {
  av_register_all();


  try {
	Twitch tv;
	TwitchStream stream = tv.openStream(argv[1]);
	auto playlist = stream.getPlaylist(TwitchStreamPlaylist::Quality::Source);
	playlist.stream();
	std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  catch (boost::exception &ex) {
	boost::diagnostic_information(ex);
  }
  catch (std::exception &ex) {
	std::cout << ex.what() << std::endl;
  }
  return 0;
}