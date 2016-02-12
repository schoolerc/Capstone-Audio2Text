#include <iostream>
#include "Exception.hpp"
#include "Twitch.hpp"
#include <thread>

int main(int argc, const char* argv[])
{
	try
	{
		Twitch tv;
		tv.connectToChannel("Jodenstone");
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	catch (boost::exception& ex)
	{
		boost::diagnostic_information(ex);
	}
	return 0;
}