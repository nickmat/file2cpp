project(file2cpp)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE Release)

set(BOOST_ROOT $ENV{BOOST}) 
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
find_package(Boost REQUIRED COMPONENTS filesystem) 

# Project header files path
include_directories(${Boost_INCLUDE_DIRS})

# The file2cpp command line program
set(F2C_HEADERS
  ../../src/fcmain.h
)
set(F2C_SOURCES
  ../../src/fcmain.cpp
  ../../src/fctmplate.cpp
  ../../src/fcwrite.cpp
)
add_executable(file2cpp ${F2C_SOURCES} ${F2C_HEADERS})
target_link_libraries(file2cpp ${Boost_LIBRARIES})
