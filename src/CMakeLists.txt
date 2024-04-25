cmake_minimum_required(VERSION 3.22)
project(MONKEY-LANG-INTERPRETER)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(interpreter)

target_link_libraries(${PROJECT_NAME} interpreter)