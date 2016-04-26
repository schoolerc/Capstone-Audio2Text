cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

include(cotire)

project(StreamAudioText C CXX)


find_package(PkgConfig)
pkg_check_modules(FFMPEG REQUIRED libavformat)
pkg_check_modules(SPHINX REQUIRED pocketsphinx)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system program_options exception REQUIRED)
find_package(RAPIDJSON REQUIRED)
find_package(cppnetlib REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCES main.cpp TwitchStream.hpp TwitchStream.cpp TwitchStreamPlaylist.hpp TwitchStreamPlaylist.cpp Exception.hpp Utilities.hpp Utilities.cpp Twitch.hpp Twitch.cpp TwitchStreamPlaylistFactory.hpp TwitchStreamPlaylistFactory.cpp TwitchStreamFactory.cpp TwitchStreamFactory.hpp TwitchStreamChunk.cpp TwitchStreamChunk.hpp TwitchStreamChunkFactory.hpp TwitchStreamChunkFactory.cpp prefix.hpp FFMPEG.h)

add_executable(a2t ${SOURCES})

include_directories(${Boost_INCLUDE_DIR} ${FFMPEG_INCLUDE_DIRS} ${CPPNETLIB_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} ${SPHINX_INCLUDE_DIRS})
target_link_libraries(a2t ${FFMPEG_LIBRARIES} ${Boost_LIBRARIES} ${CPPNETLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${ZLIB_LIBRARIES} ${SPHINX_LIBRARIES} bz2 mysqlcppconn)


set_property(DIRECTORY PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH "${FFMPEG_INCLUDE_DIRS}/libavcodec;${FFMPEG_INCLUDE_DIRS}/libavformat;${FFMPEG_INCLUDE_DIRS}/libavutil;${FFMPEG_INCLUDE_DIRS}/libavresample" "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")


cotire(a2t)