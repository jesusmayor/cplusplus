cmake_minimum_required (VERSION 2.6)

project (bots)

Find_Package ( SDL REQUIRED )
#Find_Package ( SDL_gfx REQUIRED )
find_package(OpenGL REQUIRED )

add_definitions(-std=c++11)

include_directories(${SDL_INCLUDE_DIR})

add_executable(bots playground.cpp bot.cpp bots.cpp)

target_link_libraries(bots ${SDL_LIBRARY})
target_link_libraries(bots ${OPENGL_LIBRARY})
