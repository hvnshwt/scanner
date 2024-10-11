cmake_minimum_required(VERSION 3.5.0)
project(scanner VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(
	include
	exchanges
)

find_package(CURL REQUIRED)
find_package(PostgreSQL REQUIRED)
set(HEADER_FILES
	include/database.h
	exchanges/exchange.h
	exchanges/jupiter.h
)

set(CPP_FILES 
	include/database.cpp
	exchanges/exchange.cpp
	exchanges/jupiter.cpp

	main.cpp
)

add_executable(scanner
	${HEADER_FILES}
	${CPP_FILES}
)

target_link_libraries(scanner
	PUBLIC CURL::libcurl
	PostgreSQL::PostgreSQL
	nlohmann_json::nlohmann_json
#   PUBLIC Threads::Threads
)