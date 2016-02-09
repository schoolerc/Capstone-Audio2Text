#include <string>

/*
 * \brief Class that represents a specific quality of a twitch stream.
 */
class TwitchStreamPlaylist
{
public:
	enum class StreamQuality
	{
		Mobile,
		Low,
		Medium,
		High,
		Source
	};

    int getBitrate();
    StreamQuality getQuality();
private:
    friend class TwitchStream;

    int _bitrate;
    StreamQuality _quality;

    TwitchStreamPlaylist();
    ~TwitchStreamPlaylist();
};