cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)

project(example CXX)
include_directories(pqpool)

add_executable(example src/main.cxx src/pqpool/connection.cxx src/pqpool/deleter.cxx src/pqpool/pool.cxx)

find_package(PostgreSQL REQUIRED)
target_link_libraries(example PRIVATE PostgreSQL::PostgreSQL)

