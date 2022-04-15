cmake_minimum_required(VERSION 3.16)
project(zip)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)

set(SOURCE 
    src/main.cpp
)

add_library(zipper
    SHARED
        src/zip.cpp
        src/deflate.cpp
)

target_include_directories(zipper
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/static
)

add_executable(zip ${SOURCE})

target_link_libraries(zip
    PRIVATE
        zipper
)

