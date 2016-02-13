#include <iostream>
#include "Exception.hpp"
#include "Twitch.hpp"
#include <thread>

int main(int argc, const char* argv[])
{
	try
	{
		Twitch tv;
		TwitchStream stream = tv.openStream("clintstevens");

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	catch (boost::exception& ex)
	{
		boost::diagnostic_information(ex);
	}
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
	return 0;
}