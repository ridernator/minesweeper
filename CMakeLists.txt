cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(minesweeper VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES src/*.cpp)
add_executable(minesweeper ${SRC_FILES})
target_compile_options(minesweeper PRIVATE -Wall -ggdb)

target_include_directories(minesweeper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)                                                   

target_link_libraries(minesweeper PUBLIC ncurses)
