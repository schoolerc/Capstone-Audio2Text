#pragma once
#include <string>

/*
 * \brief Class that represents a specific quality of a twitch stream.
 */
class TwitchStreamPlaylist
{
public:
	enum class Quality
	{
		Mobile,
		Low,
		Medium,
		High,
		Source
	};

	struct Resolution
	{
	    int _width;
	    int _height;
	};

    int getBandwith() { return _bandwidth; }
    Resolution getResolution() { return _resolution; }
    Quality getQuality() { return _quality; }

	~TwitchStreamPlaylist();
private:
    int _bandwidth;
    Resolution _resolution;
	Quality _quality;
    std::string _uri;

    TwitchStreamPlaylist();

    friend class TwitchStreamPlaylistFactory;
};