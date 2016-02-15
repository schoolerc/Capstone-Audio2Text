cmake_minimum_required(VERSION 3.0)

project(StreamAudioText C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(PkgConfig)
pkg_check_modules(FFMPEG REQUIRED libavformat)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system log program_options iostreams date_time regex filesystem thread exception REQUIRED)
find_package(RAPIDJSON REQUIRED)
find_package(cppnetlib REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x  -lpthread -lz")

add_executable(a2t main.cpp TwitchStream.hpp TwitchStream.cpp TwitchStreamPlaylist.hpp TwitchStreamPlaylist.cpp Exception.hpp Utilities.hpp Utilities.cpp Twitch.hpp Twitch.cpp TwitchStreamPlaylistFactory.hpp TwitchStreamPlaylistFactory.cpp TwitchStreamFactory.cpp TwitchStreamFactory.hpp TwitchStreamChunk.cpp TwitchStreamChunk.hpp TwitchStreamChunkFactory.hpp TwitchStreamChunkFactory.cpp FFMPEG.hpp)

include_directories(${Boost_INCLUDE_DIR} ${FFMPEG_INCLUDE_DIRS} ${CPPNETLIB_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
target_link_libraries(a2t ${Boost_LIBRARIES} ${FFMPEG_LIBRARIES} ${CPPNETLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${ZLIB_LIBRARIES})