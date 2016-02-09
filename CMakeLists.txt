cmake_minimum_required(VERSION 3.0)

project(StreamAudioText)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(FFMPEG REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system log program_options REQUIRED)

add_executable(a2t main.cpp HttpClient.cpp HttpClient.hpp HttpRequest.hpp HttpRequest.cpp HttpResponse.cpp HttpResponse.hpp TwitchStream.hpp TwitchStream.cpp TwitchStreamPlaylist.hpp TwitchStreamPlaylist.cpp)

include_directories(${Boost_INCLUDE_DIR} ${FFMPEG_INCLUDE_DIR})
target_link_libraries(a2t ${Boost_LIBRARIES} ${FFMPEG_LIBRARIES})


