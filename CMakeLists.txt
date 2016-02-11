cmake_minimum_required(VERSION 3.0)

project(StreamAudioText)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(FFMPEG REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system log program_options date_time regex filesystem thread exception REQUIRED)
find_package(CPPNETLIB REQUIRED)

add_executable(a2t main.cpp TwitchStream.hpp TwitchStream.cpp TwitchStreamPlaylist.hpp TwitchStreamPlaylist.cpp Exception.hpp Utilities.hpp)

include_directories(${Boost_INCLUDE_DIR} ${FFMPEG_INCLUDE_DIR} ${CPPNETLIB_INCLUDE_DIRS})
target_link_libraries(a2t ${Boost_LIBRARIES} ${FFMPEG_LIBRARIES} ${CPPNETLIB_LIBRARIES})
