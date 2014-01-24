cmake_minimum_required (VERSION 2.6)

project (MyBotClient)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

Find_Package ( SDL REQUIRED )
find_package(OpenGL REQUIRED )
find_package(Boost 1.40.0 REQUIRED COMPONENTS system thread serialization)

include_directories(${Boost_INCLUDE_DIR})



add_definitions(-std=c++11)

include_directories(${SDL_INCLUDE_DIR})

include_directories(bots)

link_directories( ${link_directories} ./bots ) 

add_executable(MyBotClient A_Star.cpp State_Base.cpp State.cpp ClientIA.cpp FrontendView.cpp ClientConnection.cpp Client.cpp)

target_link_libraries(MyBotClient bots)

target_link_libraries(MyBotClient ${SDL_LIBRARY})
target_link_libraries(MyBotClient ${OPENGL_LIBRARY})
target_link_libraries(MyBotClient ${Boost_LIBRARIES})

FIND_PACKAGE(Doxygen)
if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR 
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()

  configure_file(Doxyfile
                 ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  add_custom_target (Docs ALL 
                                          COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                                          SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
