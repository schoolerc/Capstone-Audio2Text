#include <vector>
#include "TwitchStream.hpp"
#include "TwitchStreamPlaylist.hpp"

class TwitchStreamFactory
{
public:
    TwitchStreamFactory(){}
    ~TwitchStreamFactory(){}

    void addPlaylist(TwitchStreamPlaylist);
    void setName(std::string);

    void clear();

    TwitchStream build();

private:
    std::vector<TwitchStreamPlaylist> _playlists;
    std::string _name;
};