cmake_minimum_required(VERSION 3.17.1)

set (ProjectName "RayTracing")

project (${ProjectName} C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED YES) # Force the following command to generate an error when it fails
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS NO)

add_executable(${ProjectName})

target_include_directories(${ProjectName} PUBLIC include)

add_subdirectory(src)
add_subdirectory(include)
