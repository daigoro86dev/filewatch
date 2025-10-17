cmake_minimum_required(VERSION 3.10)
project(filewatch C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include headers
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.c")

add_executable(filewatch ${SOURCES})

# Link libraries (inotify is part of libc, no extra link needed)
