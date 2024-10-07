cmake_minimum_required(VERSION 3.5.0)
project(scanner VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)

find_package(PostgreSQL REQUIRED)

add_executable(scanner main.cpp)

target_link_libraries(scanner
  PUBLIC CURL::libcurl
  PostgreSQL::PostgreSQL
#   PUBLIC Threads::Threads
)